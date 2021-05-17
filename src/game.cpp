/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "game.h"
#include "planet.h"
#include <KLocalizedString>
#include <QDebug>
#include <cmath>

QRandomGenerator Game::random = QRandomGenerator(QRandomGenerator::global()->generate());

Game::Game(QObject *parent) :
    QObject(parent)
{
    m_finalState = new QFinalState();
    m_turnCounter = 0;
    m_map = new Map(10, 10);
    m_gameMachine.addState(m_finalState);
    m_neutral = new NeutralPlayer(this);
    connect(&m_gameMachine, &QStateMachine::finished, this, &Game::finished);
}

Game::~Game()
{
    delete m_map;
}

const QList<Planet*> Game::planets()
{
    return m_map->planets();
}

bool Game::isRunning()
{
    return m_gameMachine.isRunning();
}

Coordinate Game::generatePlanetCoordinates(int x, int y)
{
    return Coordinate(random.bounded(x), random.bounded(y));
}

double Game::generateKillPercentage()
{
    // 0.30 - 0.90
    return 0.30 + random.bounded(0.60);
}

int Game::generatePlanetProduction()
{
    // 5 - 15
    return 5 + random.bounded(10);
}

bool Game::attack(Planet *sourcePlanet, Planet *destPlanet, long long shipCount, bool standingOrder)
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
            Q_EMIT gameMsg(ki18np("Reinforcements (1 ship) have arrived for planet %2.",
                            "Reinforcements (%1 ships) have arrived for planet %2.")
                .subs(fleet->shipCount()), nullptr, fleet->destination);
    } else {
        // let's get ready to rumble...
        AttackFleet   *attacker       = fleet;
        Planet        *attackerPlanet = attacker->source;
        Planet        *defenderPlanet = attacker->destination;
        DefenseFleet  &defender       = defenderPlanet->fleet();

        bool  haveVictor  = false;
        bool  planetHolds = true;

        while( !haveVictor ) {
            double  attackerRoll = random.bounded(1.0);
            double  defenderRoll = random.bounded(1.0);

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
            Q_EMIT gameMsg(ki18n("Planet %2 has held against an attack from %1."),
                         attacker->owner, defenderPlanet);
        } else {
            Player *defender = defenderPlanet->player();
            attacker->owner->statEnemyFleetsDestroyed( 1 );

            defenderPlanet->conquer( attacker );

            Q_EMIT gameMsg(ki18n("Planet %2 has fallen to %1."),
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
    //qDebug() << "Searching for survivors";
    // Check for survivors
    Player *winner = nullptr;
    for (Player *player : players()) {
        if (player->isNeutral() || player->isSpectator()) {
            continue;
        }
        if (!player->isDead()) {
            if (winner) {
                //qDebug() << "Ok, returning 0";
                return;
            } else {
                winner = player;
            }
        }
    }
    //qDebug() << "Ok, returning " << winner;
    if (winner)
    {
        // We got a winner
        //qDebug() << "Trying to stop";
        this->stop();
        Q_EMIT finished();
    }
}
