/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "becai.h"

#include <cfloat>
#include <cmath>

#include <QDebug>
#include <QMap>
#include <QMultiMap>
#include <QtAlgorithms>

#include "../../../game.h"
#include "../../../planet.h"


AiBecai::AiBecai(
        Game *game,
        const QString &newName,
        const QColor &color)
    : ComputerPlayer(
        game,
        newName,
        color)
{
}


/**
 * This internal helper class is for attack target categorisation.
 */

class TargetPlanet
{
public:
    TargetPlanet(Planet *planet, int minimumAttackFleetToConquerPlanet, double distance);
    Planet* planet() const { return m_planet; }
    int minimumAttackFleetToConquerPlanet() const { return m_minimumAttackFleetToConquerPlanet; }
    double distance() const { return m_distance; }

private:
    Planet* m_planet;
    int m_minimumAttackFleetToConquerPlanet;
    double m_distance;
};


TargetPlanet::TargetPlanet(Planet *planet, int minimumAttackFleetToConquerPlanet, double distance) :
    m_planet(planet),
    m_minimumAttackFleetToConquerPlanet(minimumAttackFleetToConquerPlanet),
    m_distance(distance)
{
}


/**
 * Get the minimum defence fleet size for the given planet.
 *
 * Protect planets with higher kill percentage better. Do the same for
 * planets with higher production.
 */

int
AiBecai::getMinimumDefenceFleetSize(Planet *planet, int minimumBaseDefenceFleetSize, double averageOwnKillPercentage, double averageOwnProduction)
{
    return std::ceil(minimumBaseDefenceFleetSize * planet->killPercentage() / averageOwnKillPercentage * planet->production() / averageOwnProduction);
}


void
AiBecai::play()
{
    int totalTotalPlanets = 0;

    int totalOwnProduction = 0;
    int totalOwnPlanets = 0;
    long long totalOwnFleet = 0;

    int totalEnemyProduction = 0;
    int totalEnemyPlanets = 0;
    int totalEnemyDefence = 0;

    // The first and nearly most important part is to analyze the current game
    // situation. For this, different metrics are calculated.

    /**
     * @todo Move these metrics into the general player class so that they can
     * be calculated and displayed to the human player for its own empire one
     * day. Or maybe but them into an own class.
     */

    for (Fleet *fleet : this->attackList()) {
        totalOwnFleet += fleet->shipCount();
    }

    // We need to know the average planet defence fleet size. So we get a list
    // of all planet defence fleet sizes, and based on that, calculate the
    // average.

    QList<int> nonOwnPlanetDefenceFleetSizeList;

    // We like to protect high kill percentage planets more than others. So we
    // need to know our own average.

    double totalKillPercentage = 0;

    for (Planet *planet : m_game->planets()) {
        totalTotalPlanets += 1;

        if (planet->player() == this) {

            // We found a planet of us.

            totalOwnProduction += planet->production();
            totalOwnPlanets += 1;
            totalOwnFleet += planet->fleet().shipCount();

            totalKillPercentage += planet->killPercentage();
        }
        else {
            if (planet->player()->isNeutral() != true) {

                // We found a non-neutral enemy planet.

                totalEnemyProduction += planet->production();
                totalEnemyPlanets += 1;
                totalEnemyDefence += planet->fleet().shipCount();
            }

            nonOwnPlanetDefenceFleetSizeList.push_back(planet->fleet().shipCount());
        }
    }

    // Ignore the upper and lower 10% of non-own planet defence fleet sizes and
    // create the arithmetic average. In an FFA game with multiple enemies, we
    // have an advantage if we can keep defence at a higher level than others.

    std::sort(nonOwnPlanetDefenceFleetSizeList.begin(), nonOwnPlanetDefenceFleetSizeList.end());

    for (int i = std::floor((double) nonOwnPlanetDefenceFleetSizeList.size() / 10); i > 0; --i) {
        nonOwnPlanetDefenceFleetSizeList.takeFirst();
        nonOwnPlanetDefenceFleetSizeList.takeLast();
    }

    int averageNonOwnPlanetDefenceFleetSize = 0;

    for (const int &fleetSize : qAsConst(nonOwnPlanetDefenceFleetSizeList)) {
        averageNonOwnPlanetDefenceFleetSize += fleetSize;
    }

    if (!nonOwnPlanetDefenceFleetSizeList.isEmpty()) {
        averageNonOwnPlanetDefenceFleetSize = std::ceil((double) averageNonOwnPlanetDefenceFleetSize / nonOwnPlanetDefenceFleetSizeList.size());
    }

    if (totalOwnPlanets == 0) {

        // It is perfectly valid to have no own planets, but to still be alive.
        // This is the case of having at least one fleet still traveling.
        // We can abort here as there is nothing we can do (and to avoid
        // division by zero later on).

        Q_EMIT donePlaying();
        return;
    }

    double averageOwnKillPercentage = totalKillPercentage / totalOwnPlanets;
    double averageOwnProduction = (double) totalOwnProduction / totalOwnPlanets;

    // The minimum defence should depend on the game situation.
    // Values that define the game situation are:
    // - number of enemy/neutral planets (and their defence)
    // - number of own planets compared to non-neutral enemy planets
    // - number of enemies still active in the game

    // The first idea is to allocate a certain fixed percentage of our total
    // fleet for defence distributed across all our planets. But in the
    // beginning, we need less defence, and later on, in mid-game, we need more,
    // and in end-game, when we are about to finish off the remaining enemy
    // planets, less again. So the percentage should be dynamic rather than
    // fixed.

    // But keep in mind that 1) having less defence in beginning makes us a good
    // long-range target and 2) in non-cumultative games, lots of neutral
    // planets are never conquered, in case of a higher (3+) neutral production!

    // So use a fixed percentage of our total fleet for defence but adjust it
    // with a dynamic factor based on game situation. Reduce it the more planets
    // we have, compared to non-neutral planets (planets occupied by any player
    // including us). Actually it is the production that counts and not the
    // planets, but planets are good enough for us here for the moment.

    int minimumBaseDefenceFleetSize = (int) std::ceil(((totalOwnFleet / 2.0) / totalOwnPlanets) * (1 - ((double) totalOwnPlanets / (totalOwnPlanets + totalEnemyPlanets))));
    int originalMinimumBaseDefenceFleetSize = minimumBaseDefenceFleetSize;

    // Try to keep the defence above the universe average so that our planets
    // are less attractive to attack than other enemy planets (for further
    // enemies). However, there is no need to stay significantly above that
    // average. Remember, if we bind our ships for defence, they cannot attack.

    // If the average enemy defence fleet size is above our minimum defence
    // fleet size, simply ignore that fact. This happens if there is only one
    // enemy planet left with very high defence (for example the example AI).

    int minimumCappedBaseDefenceFleetSize = 1.5 * averageNonOwnPlanetDefenceFleetSize;

    // This value is a good indicator, but has one slight problem: If the enemy
    // really just towers on his initial home planet, we never get a fleet large
    // enough to conquer it. So limit the own defence fleet further capped on
    // the damage the enemy can do.

    int enemyAttackPerOwnPlanet = std::ceil((double) totalEnemyDefence / totalOwnPlanets);

    //qDebug() << "total own production: " << totalOwnProduction;
    //qDebug() << "total own fleet: " << totalOwnFleet;
    //qDebug() << "total own planets: " << totalOwnPlanets;
    //qDebug() << "total total planets: " << totalTotalPlanets;
    //qDebug() << "total enemy production: " << totalEnemyProduction;
    //qDebug() << "total enemy defence: " << totalEnemyDefence;
    //qDebug() << "total enemy planets: " << totalEnemyPlanets;
    //qDebug() << "minimum base defence fleet size: " << minimumBaseDefenceFleetSize;
    //qDebug() << "minimum capped base defence fleet size: " << minimumCappedBaseDefenceFleetSize;
    //qDebug() << "enemy attack per own planet: " << enemyAttackPerOwnPlanet;
    //qDebug() << "average non-own planet defence fleet size: " << averageNonOwnPlanetDefenceFleetSize;
    //qDebug() << "average own kill percentage: " << averageOwnKillPercentage;
    //qDebug() << "average own production: " << averageOwnProduction;

    if (minimumBaseDefenceFleetSize > minimumCappedBaseDefenceFleetSize) {
        //qDebug() << "Reducing minimum base defence fleet size from " << minimumBaseDefenceFleetSize << " to " << minimumCappedBaseDefenceFleetSize << " (cap).";
        minimumBaseDefenceFleetSize = minimumCappedBaseDefenceFleetSize;
    }

    if (minimumBaseDefenceFleetSize > enemyAttackPerOwnPlanet) {
        //qDebug() << "Reducing minimum base defence fleet size from " << minimumBaseDefenceFleetSize << " to " << enemyAttackPerOwnPlanet << " (enemy attack per own planet).";
        minimumBaseDefenceFleetSize = enemyAttackPerOwnPlanet;

        if (minimumBaseDefenceFleetSize < averageOwnProduction) {

            // We are now in a very end-game situation. The enemy has hardly any
            // planets anymore so our required defence can be lowered as the
            // enemy can hardly damage us. Still, if our defence gets reduced
            // by too much, even a very small enemy fleet can conquer a planet.
            // So keep up a minimum fleet hopefully large enough to destroy any
            // (small) incoming enemy fleet.

            //qDebug() << "Minimum base defence is too low. Resetting to original minimum base defence fleet size!";
            minimumBaseDefenceFleetSize = originalMinimumBaseDefenceFleetSize;
        }
    }

    if (totalEnemyPlanets == 0) {

        // This is a very special case. The enemy has no planets anymore but is
        // still alive, so it has fleets attacking us. As we do not know where
        // they will attack, evenly distribute our fleet among all our planets.

        // The actual problem is that in this situation, the calculated
        // minimum base defence fleet size is zero.

        //qDebug() << "No enemy planets found! Evenly distribute our fleet among our planets.";
        minimumBaseDefenceFleetSize = totalOwnFleet / totalOwnPlanets;
    }

    for (Planet *home : m_game->planets()) {
        if (home->player() != this) {
            continue;
        }

        int minimumDefenceFleetSize = getMinimumDefenceFleetSize(home, minimumBaseDefenceFleetSize, averageOwnKillPercentage, averageOwnProduction);
        int surplusFleetSize = home->ships() - minimumDefenceFleetSize; // can be negative

        Planet* closestUpstreamPlanet = home;
        double closestUpstreamDistance = DBL_MAX;

        Planet* closestSupportPlanet = home;
        double closestSupportDistance = DBL_MAX;

        QMultiMap<double, TargetPlanet> targetList;

        for (Planet *other : m_game->planets()) {
            if (other->player() == this) {
                if (other != home) {

                    // We found one of our own planets which is not the current
                    // planet.

                    double distance = m_game->map()->distance(home, other);

                    if (distance <= closestUpstreamDistance) {

                        // The planet is closer. Check if it has a higher
                        // kill percentage. We just want to find the closest
                        // planet with a higher kill percentage.

                        if (other->killPercentage() > home->killPercentage()) {
                            closestUpstreamPlanet = other;
                            closestUpstreamDistance = distance;
                        }
                    }

                    if (distance <= closestSupportDistance) {

                        // The planet is closer. Check if it needs support.

                        if (other->ships() < getMinimumDefenceFleetSize(other, minimumBaseDefenceFleetSize, averageOwnKillPercentage, averageOwnProduction)) {
                            closestSupportPlanet = other;
                            closestSupportDistance = distance;
                        }
                    }
                }
            }
            else {

                // We found an enemy planet.

                // Do not send a fleet to the planet if a fleet is already on
                // its way.

                bool found = false;

                for (AttackFleet *fleet : this->attackList() + this->newAttacks()) {
                    if (fleet->destination == other) {
                        found = true;
                        break;
                    }
                }

                if (found != true) {
                    double distance = m_game->map()->distance(home, other);

                    int production;
                    bool isNeutral;

                    if (other->player()->isNeutral()) {
                        production = m_game->options().NeutralsProduction;
                        isNeutral = true;
                    }
                    else {
                        production = other->production();
                        isNeutral = false;
                    }

                    double killPercentageOther = other->killPercentage();
                    double killPercentageHome = home->killPercentage();

                    // Avoid division by zero. Right now, these cases are only
                    // possible by manually altering game conditions at the
                    // beginning.

                    if (killPercentageOther < 0.1) {
                        killPercentageOther = 0.1;
                    }

                    if (killPercentageHome < 0.1) {
                        killPercentageOther = 0.1;
                    }

                    /**
                     * @todo The production increment in cumultative games is
                     * not considered here.
                     */

                    int minimumAttackFleetToConquerPlanet = std::ceil((other->ships() + std::ceil(distance) * production) * killPercentageOther / killPercentageHome);

                    if (minimumAttackFleetToConquerPlanet == 0) {

                        // In case neutral planets have a production of zero,
                        // we would need a fleet of size zero to conquer them.
                        // As this does not work we use a minimum attack fleet
                        // size of one.

                        ++minimumAttackFleetToConquerPlanet;
                    }

                    // Prefer closer targets to further away targets (distance
                    // penalty).

                    /**
                     * @todo Instead of a distance penalty, simply consider the
                     * production we could get from the closer planet due to
                     * saved transit time compared to what we save in attack
                     * ships.
                     */

                    /**
                     * @todo Maybe not take the weakest we can get (first on
                     * list), but the strongest we can get.
                     */

                    /**
                     * @todo The universe is flat, so prefer planets at the
                     * edge. They have less closer neightbours than centrally
                     * located planets!
                     */

                    /**
                     * @todo If we have a choice, attack the strongest player.
                     * This way our empire can expand and at the end, we do not
                     * have to face fighting a superpower.
                     */

                    double planetScore = minimumAttackFleetToConquerPlanet * distance;

                    // Try to prefer higher production and higher kill percentage.

                    planetScore *= 1 / (killPercentageOther / averageOwnKillPercentage);
                    planetScore *= 1 / (other->production() / averageOwnProduction);

                    // Prefer attacking non-neutral planets.
                    // Neutrals don't harm us, enemies do.

                    if (isNeutral) {

                        // Penalty for neutral planets!
                        // Multiply the score with the defence so that closer
                        // planets are higher ranked (smaller value).

                        targetList.insert(1.1 * planetScore, TargetPlanet(other, minimumAttackFleetToConquerPlanet, distance));
                    }
                    else {
                        targetList.insert(1.0 * planetScore, TargetPlanet(other, minimumAttackFleetToConquerPlanet, distance));
                    }
                }
            }
        }

        // Process the first few possible targets from the target list. As this
        // is a priority list, only consider the first few ones and do not
        // process possible targets with low priority (high score).

        QMapIterator<double, TargetPlanet> targetListIt(targetList);

        int skipCount = 3;

        while (targetListIt.hasNext() && (skipCount > 0)) {
            targetListIt.next();

            // Always decrement the skip counter. If an attack can be launched
            // successfully, simply increment it again as this is no skip then.

            --skipCount;

            Planet *attackPlanet = targetListIt.value().planet();
            int minimumAttackFleetToConquerPlanet = targetListIt.value().minimumAttackFleetToConquerPlanet();
            double distance = targetListIt.value().distance();

            if (closestUpstreamPlanet != home) {

                // We know that there is an own planet with higher kill
                // percentage. Do not attack other planet if that one is quite
                // far away compared to upstream planet.

                if (distance > 2 * closestUpstreamDistance) {
                    continue;
                }
            }

            if (surplusFleetSize > 0) {

                // We actually have more ships than required for defence,
                // so we can start an attack.

                if (surplusFleetSize > minimumAttackFleetToConquerPlanet) {

                    // We now know that we can conquer the planet with our home
                    // fleet. Now, figure out how many ships we need and
                    // actually want to send.

                    // Send as many ships as needed to conquer the planet and to
                    // build up a "proper" defence fleet. If we do not have that
                    // many ships, send our whole surplus fleet.

                    /**
                     * @todo Maybe first check if we can attack all targets, and
                     * if we still have a surplus, use that one distributed
                     * across all targets!
                     */

                    int attackFleetSize = 0;
                    int minimumActualDefenceFleetSize = getMinimumDefenceFleetSize(attackPlanet, minimumBaseDefenceFleetSize, averageOwnKillPercentage, averageOwnProduction);

                    if (surplusFleetSize > minimumAttackFleetToConquerPlanet + minimumActualDefenceFleetSize) {
                        attackFleetSize = minimumAttackFleetToConquerPlanet + minimumActualDefenceFleetSize;
                    }
                    else
                    if (surplusFleetSize > minimumAttackFleetToConquerPlanet) {

                        // We actually can conquer the planet. So send as much
                        // as we have of surplus.

                        attackFleetSize = surplusFleetSize;
                    }

                    if (attackFleetSize > 0) {
                        //qDebug() << "Attacking " << attackPlanet->name() << " from " << home->name() << " with " << attackFleetSize << ".";
                        m_game->attack(home, attackPlanet, attackFleetSize);

                        surplusFleetSize -= attackFleetSize;
                        ++skipCount;
                    }
                }
            }
            else {

                // Either our surplus fleet size is negative and we actually
                // need supply, or our surplus fleet is not large enough
                // to successfully conquer this planet.

            }
        }

        // If we still have surplus now, this is likely because we are out of
        // close targets. Either use surplus for support or send it upstream.

        if (closestSupportPlanet != home) {
            if (closestSupportDistance < 2 * closestUpstreamDistance) {

                // Send ships to support the other planet, but only if that
                // planet is not much further away than our upstream planet.

                // And only send support if not another planet sent support
                // already. So for now, just check if there is an incoming
                // friendly attack fleet.

                bool skip = false;

                for (AttackFleet *fleet : this->attackList() + this->newAttacks()) {
                    if ((fleet->destination == closestSupportPlanet) && (fleet->owner == this)) {
                        skip = true;
                        break;
                    }
                }

                if (!skip) {

                    // Additionally add as much as one round of production.

                    // Or better not, as that just makes this planet look like
                    // requiring support from others.

                    // surplusFleetSize += home->production();

                    if (surplusFleetSize > 0) {

                        // Send complete surplus fleet. This could be more than
                        // the support planet actually needs (or less), but for
                        // now this is good enough.

                        //qDebug() << "Supporting (support) " << closestSupportPlanet->name() << " from " << home->name() << " with " << surplusFleetSize << ".";
                        m_game->attack(home, closestSupportPlanet, surplusFleetSize);

                        // We do not have any surplus fleet anymore to send
                        // upsteam.

                        surplusFleetSize = 0;
                    }
                }
            }
        }

        if (closestUpstreamPlanet != home) {

            // Send ships in larger chunks. Having defence on our planets makes
            // them less attractive for enemy attacks. And having more than the
            // minimum defence allows giving support to other planets, if
            // needed.

            if (surplusFleetSize > 3 * home->production()) { // > 0) {
                //qDebug() << "Supporting (upstream) " << closestUpstreamPlanet->name() << " from " << home->name() << " with " << surplusFleetSize << ".";
                m_game->attack(home, closestUpstreamPlanet, surplusFleetSize);
            }
        }
    }

    Q_EMIT donePlaying();
}
