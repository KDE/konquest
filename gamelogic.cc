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
#include "gamelogic.h"

#include <klocale.h>

#include "gamecore.h"
#include "player.h"
#include "map.h"

#include "gamelogic.moc"

/*********************************************************************
 Game Logic
*********************************************************************/

GameLogic::GameLogic( QObject *parent )
    : QObject( parent )
{
    m_map           = new Map(16, 16);
    m_neutralPlayer = Player::createNeutralPlayer(m_map);
    m_blindbreak    = false;

    cleanupGame();
}


GameLogic::~GameLogic()
{
    delete m_map;
}


//************************************************************************
// To the end turn processing (resolve combat, etc.)
//************************************************************************


// FIXME: Rename into resolveTurn(), and create a new newTurn() method.

void
GameLogic::nextTurn()
{
    // Resolve things from last turn.
    resolveShipsInFlight();

    // See if any players are left.
    scanForSurvivors();

    // advance to first living player
    while( (*m_currentPlayer) && !(*m_currentPlayer)->isInPlay() )
    	++m_currentPlayer;

    // advance turn counter
    // FIXME: Emit a newTurn() signal.
    m_turnNumber++;
    
    // update the planets
    foreach (Planet *planet, m_map->planets())
        planet->turn(&m_options);
}


//************************************************************************
// Determine the fate of the ships in transit
//************************************************************************


void
GameLogic::resolveShipsInFlight()
{
    AttackFleetList  arrivingShips;

    foreach (Player *player, m_players) {
        player->attackList() += player->newAttacks();
        player->statFleetsLaunched( player->newAttacks().size() );
        player->newAttacks().clear();
        foreach (AttackFleet *fleet, player->attackList()) {
            if( m_turnNumber == fleet->arrivalTurn ) {
                doFleetArrival( fleet );
                player->attackList().removeAll( fleet );
                delete fleet;
            }
        }
    }
}

QList<Planet *>
GameLogic::planets()
{
    return m_map->planets();
}

int
GameLogic::humanPlayerCount()
{
    int count = 0;
    foreach(Player *player, m_players)
        count += !player->isAiPlayer();
    return count;
}

Player *
GameLogic::findWinner()
{
    Player *winner = 0;

    foreach (Player *player, m_players) {
        if (player->isInPlay()) {
	    // If there are more than one player alive, then there is
	    // no winner.
            if (winner != 0)
                return 0;

	    // Tentatively set this player as winner.
            winner = player;
        }
        else if (player->attackList().count() != 0)
            // There is still a fleet attacking, so wait for its arrival...
            return 0;
    }

    return winner;
}



//************************************************************************
// check to see any players have been eliminated
//************************************************************************


void
GameLogic::scanForSurvivors()
{
    QList<Player *>  activePlayers;
    QList<Player *>  inactivePlayers;

    // Insert all of the active players into a special list,
    // then deactivate them
    Player *player;
    foreach (player, m_players) {
        if( player->isInPlay() ) {
            activePlayers.append( player );
            player->setInPlay( false );
        } else {
            inactivePlayers.append( player );
        }
    }


    // iterate through the list of planets and
    // mark their owners in play
    foreach (Planet *planet, m_map->planets()) {
        planet->player()->setInPlay( true );
    }

    foreach (player, activePlayers) {
        if( !player->isInPlay() ) {
            // Player has bitten the dust
            emit gameMsg(ki18n("The once mighty empire of %1 has fallen in ruins."), player);
        }
    }

    foreach (player, inactivePlayers) {
        if( player->isInPlay() ) {
            // Player has bitten the dust
            emit gameMsg(ki18n("The fallen empire of %1 has staggered back to life."), player);
        }
    }
}


//************************************************************************
// handle the arrival of a fleet at a planet
//************************************************************************


void
GameLogic::doFleetArrival( AttackFleet *arrivingFleet )
{
    // Check to see of (fleet owner) == (planet owner)
    // if the planet and fleet owner are the same, then merge the fleets
    // otherwise attack.

    if( *arrivingFleet->owner == *arrivingFleet->destination->player() ) {
        arrivingFleet->destination->fleet().absorb(arrivingFleet);
	if ( !arrivingFleet->owner->isAiPlayer() )
	    emit gameMsg(ki18np("Reinforcements (1 ship) have arrived for planet %2.",
                            "Reinforcements (%1 ships) have arrived for planet %2.")
                .subs(arrivingFleet->shipCount()),
                0, arrivingFleet->destination);
    } else {

        // let's get ready to rumble...

        CoreLogic      cl;
        AttackFleet   &attacker       = *arrivingFleet;
        Planet        &attackerPlanet = *(attacker.source);
        Planet        &defenderPlanet = *(attacker.destination);
        DefenseFleet  &defender       = defenderPlanet.fleet();

        bool  haveVictor  = false;
        bool  planetHolds = true;

        while( !haveVictor ) {
            double  attackerRoll = cl.roll();
            double  defenderRoll = cl.roll();

            /* special case if both have 0 kill percentages */
            if( defenderPlanet.killPercentage() == 0 &&
                attackerPlanet.killPercentage() == 0) {
                if(attackerRoll <  defenderRoll )
                    MakeKill(defender, *attackerPlanet.player());
                else
                    MakeKill(attacker, *defenderPlanet.player());
            }
 
            if( defenderRoll < defenderPlanet.killPercentage() )
                MakeKill(attacker, *defenderPlanet.player());
 
            if( attacker.shipCount() <= 0 ) {
                haveVictor  = true;
                planetHolds = true;
                continue;
            }

            if( attackerRoll < attackerPlanet.killPercentage() )
                MakeKill(defender, *attackerPlanet.player());

            if( defender.shipCount() <= 0 ) {
                haveVictor  = true;
                planetHolds = false;
            }
        }

        if( planetHolds ) {
            defenderPlanet.player()->statEnemyFleetsDestroyed(1);
            emit gameMsg(ki18n("Planet %2 has held against an attack from %1."),
			 attacker.owner, &defenderPlanet);
        } else {
            Player  *defender = defenderPlanet.player();
            attacker.owner->statEnemyFleetsDestroyed( 1 );

            defenderPlanet.conquer( &attacker );

            emit gameMsg(ki18n("Planet %2 has fallen to %1."),
			 attacker.owner, &defenderPlanet, defender);
        }
    }
}

void
GameLogic::MakeKill(Fleet &fleet, Player &player)
{
    fleet.removeShips( 1 );
    player.statEnemyShipsDestroyed( 1 );
}

//************************************************************************
// Set up the game board for a new game
//************************************************************************


void
GameLogic::startNewGame()
{
    // Setup for a new game to start playing.
    m_currentPlayer = m_players.begin();

    m_turnNumber    = 0;

    nextTurn();
    emit beginTurn();
}


//************************************************************************
// Shut down the current game
//************************************************************************

void
GameLogic::cleanupGame()
{
    m_map->clearMap();

    m_players.clear();
}


// The current player has done its orders, and it's the next players
// turn.
//

void
GameLogic::nextPlayer()
{
    foreach (Planet *planet, m_map->planets()) {
        if(planet->player() == *m_currentPlayer) {
            planet->showOldShips();
        }
    }
    // End turn and advance to next player.
    do {
        ++m_currentPlayer;
    } while ( m_currentPlayer != m_players.end()
	      && !(*m_currentPlayer)->isInPlay() );

    if( m_currentPlayer == m_players.end() ) {
        emit endTurn();

        // end of player list, new turn
        m_currentPlayer = m_players.begin();

        nextTurn();
        emit beginTurn();
    }
}
