/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright Inge Wallin <inge@lysator.liu.se>
    Copyright Pierre Ducroquet <pinaraf@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "game.h"
#include "players/player.h"
#include "planet.h"
#include <KLocalizedString>
#include <KDebug>
#include <cmath>

KRandomSequence Game::random = KRandomSequence();

Game::Game(QObject *parent) :
    QObject(parent)
{
    m_finalState = new QFinalState();
    m_turnCounter = 0;
    m_map = new Map(10, 10);
    m_gameMachine.addState(m_finalState);
    m_neutral = new NeutralPlayer(this);
    connect(&m_gameMachine, SIGNAL(finished()), this, SIGNAL(finished()));
}

QList<Planet*> Game::planets()
{
    return m_map->planets();
}

bool Game::isRunning()
{
    return m_gameMachine.isRunning();
}

Coordinate Game::generatePlanetCoordinates(int x, int y)
{
    return Coordinate(random.getLong(x), random.getLong(y));
}

double Game::generateKillPercentage()
{
    // 0.30 - 0.90
    return 0.30 + random.getDouble()*0.60;
}

int Game::generatePlanetProduction()
{
    // 5 - 15
    return 5 + random.getLong(10);
}

bool Game::attack(Planet *sourcePlanet, Planet *destPlanet, int shipCount, bool standingOrder)
{
    int arrival = int(std::ceil(m_map->distance(sourcePlanet, destPlanet))) + m_turnCounter;
    if(standingOrder)
    {
        m_currentPlayer->addStandingOrder(new AttackFleet(sourcePlanet, destPlanet, shipCount, arrival));
        return true;
    }
    else
    {
        AttackFleet *fleet = sourcePlanet->fleet().spawnAttackFleet(destPlanet, shipCount, arrival);
        if (fleet) {
            m_currentPlayer->addAttackFleet(fleet);
            return true;
        }
        return false;
    }
}

void Game::setPlayers(const QList<Player *> &players)
{
    m_players = players;
}

void Game::setCurrentPlayer(Player *player)
{
    m_currentPlayer = player;
}

bool Game::doFleetArrival(AttackFleet *fleet)
{
    // First, sanity check
    if (fleet->arrivalTurn != m_turnCounter)
        return false;

    // Check to see of (fleet owner) == (planet owner)
    // if the planet and fleet owner are the same, then merge the fleets
    // otherwise attack.
    if( fleet->owner == fleet->destination->player()) {
        fleet->destination->fleet().absorb(fleet);
        if ( !fleet->owner->isAiPlayer() )
            emit gameMsg(ki18np("Reinforcements (1 ship) have arrived for planet %2.",
                            "Reinforcements (%1 ships) have arrived for planet %2.")
                .subs(fleet->shipCount()), 0, fleet->destination);
    } else {
        // let's get ready to rumble...
        AttackFleet   *attacker       = fleet;
        Planet        *attackerPlanet = attacker->source;
        Planet        *defenderPlanet = attacker->destination;
        DefenseFleet  &defender       = defenderPlanet->fleet();

        bool  haveVictor  = false;
        bool  planetHolds = true;

        while( !haveVictor ) {
            double  attackerRoll = random.getDouble();
            double  defenderRoll = random.getDouble();

            /* special case if both have 0 kill percentages */
            if( defenderPlanet->killPercentage() == 0 &&
                attackerPlanet->killPercentage() == 0) {
                if(attackerRoll <  defenderRoll )
                    makeKill(&defender, attackerPlanet->player());
                else
                    makeKill(attacker, defenderPlanet->player());
            }

            if( defenderRoll < defenderPlanet->killPercentage() )
                makeKill(attacker, defenderPlanet->player());

            if( attacker->shipCount() <= 0 ) {
                haveVictor  = true;
                planetHolds = true;
                continue;
            }
            if( attackerRoll < attackerPlanet->killPercentage() )
                makeKill(&defender, attackerPlanet->player());

            if( defender.shipCount() <= 0 ) {
                haveVictor  = true;
                planetHolds = false;
            }
        }

        if( planetHolds ) {
            defenderPlanet->player()->statEnemyFleetsDestroyed(1);
            emit gameMsg(ki18n("Planet %2 has held against an attack from %1."),
                         attacker->owner, defenderPlanet);
        } else {
            Player *defender = defenderPlanet->player();
            attacker->owner->statEnemyFleetsDestroyed( 1 );

            defenderPlanet->conquer( attacker );

            emit gameMsg(ki18n("Planet %2 has fallen to %1."),
                         attacker->owner, defenderPlanet, defender);
        }
    }
    return true;
}

void Game::makeKill(Fleet *fleet, Player *player)
{
    fleet->removeShips( 1 );
    player->statEnemyShipsDestroyed( 1 );
}

void Game::findWinner()
{
    kDebug() << "Searching for survivors";
    // Check for survivors
    Player *winner = 0;
    foreach (Player *player, m_players) {
        if (player->isNeutral()) {
            continue;
        }
        if (!player->isDead()) {
            if (winner) {
                kDebug() << "Ok, returning 0";
                return;
            } else {
                winner = player;
            }
        }
    }
    kDebug() << "Ok, returning " << winner;
    if (winner)
    {
        // We got a winner
        kDebug() << "Trying to stop";
        this->stop();
        emit(finished());
    }
}
