#include <math.h>

#include <klocale.h>

#include "gamecore.h"
#include "gamelogic.h"
#include "player.h"
#include "map.h"

#include "gamelogic.moc"

#include <QtDebug>


/*********************************************************************
 Game Logic
*********************************************************************/

GameLogic::GameLogic( QObject *parent )
    : QObject( parent ), gameInProgress( false ), queueMessages(false), messageQueue()
{
    QPalette blackPal;

    neutralPlayer = Player::createNeutralPlayer(map);
    map = new Map;
}


GameLogic::~GameLogic()
{
    // Nothing much to do yet
}


//************************************************************************
// To the end turn processing (resolve combat, etc.)
//************************************************************************


void
GameLogic::nextTurn()
{
    resolveShipsInFlight();

    scanForSurvivors();

    // advance to first living player
    while( (*currentPlayer) && !(*currentPlayer)->isInPlay() ) {
    	++currentPlayer;
    };

    // advance turn counter
    m_turnNumber++;
    
    // update the planets
    foreach (Planet *planet, planets) {
        planet->turn();
    }

    Player *winner = findWinner();
    if (winner) {
        gameOver();
	emit gameOver(winner);
    }

    if( winner || (m_turnNumber >= m_lastTurn) ) {
        // Game over, man! Game over.

    }
}


//************************************************************************
// Determine the fate of the ships in transit
//************************************************************************


void
GameLogic::resolveShipsInFlight()
{
    AttackFleetList  arrivingShips;

    foreach (Player *player, players) {
        foreach (AttackFleet *fleet, player->attackList()) {
            double  fleetArrivalTurn = floor(fleet->arrivalTurn);

            if( m_turnNumber == int (fleetArrivalTurn) ) {
                doFleetArrival( fleet );
                player->attackList().remove( fleet );
                delete fleet;
            }
        }
    }
}


Player *
GameLogic::findWinner()
{
    Player *winner = 0;

    foreach (Player *player, players) {
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
    foreach (player, players) {
        if( player->isInPlay() ) {
            activePlayers.append( player );
            player->setInPlay( false );
        } else {
            inactivePlayers.append( player );
        }
    }


    // iterate through the list of planets and
    // mark their owners in play
    foreach (Planet *planet, planets) {
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

    if( (*arrivingFleet->owner) == (*arrivingFleet->destination->player())) {
        if (!arrivingFleet->owner->isAiPlayer()) {
        	arrivingFleet->destination->fleet().absorb(arrivingFleet);

        	emit gameMsg(ki18np("Reinforcements (1 ship) have arrived for planet %2.",
				    "Reinforcements (%n ships) have arrived for planet %2.")
			     .subs(arrivingFleet->shipCount()),
			     0, arrivingFleet->destination);
        }
    } else {

        // let's get ready to rumble...

        CoreLogic cl;
        AttackFleet &attacker = *arrivingFleet;
        DefenseFleet &defender = arrivingFleet->destination->fleet();
        Planet &prizePlanet = *(arrivingFleet->destination);

        bool haveVictor = false;
        bool planetHolds = true;

        while( !haveVictor ) {
            double attackerRoll = cl.roll();
            double defenderRoll = cl.roll();

            if( defenderRoll < prizePlanet.killPercentage() ) {
                attacker.removeShips( 1 );
            }

            if( attacker.shipCount() <= 0 ) {
                haveVictor  = true;
                planetHolds = true;
                continue;
            }

            if( attackerRoll < attacker.killPercentage ) {
                defender.removeShips( 1 );
                attacker.owner->statEnemyShipsDestroyed( 1 );
            }

            if( defender.shipCount() <= 0 ) {
                haveVictor  = true;
                planetHolds = false;
            }
        }

        if( planetHolds ) {
            prizePlanet.player()->statEnemyFleetsDestroyed(1);
            emit gameMsg(ki18n("Planet %2 has held against an attack from %1."),
			 attacker.owner, &prizePlanet);
        } else {
            Player *defender = prizePlanet.player();
            attacker.owner->statEnemyFleetsDestroyed( 1 );

            arrivingFleet->destination->conquer( arrivingFleet );

            emit gameMsg(ki18n("Planet %2 has fallen to %1."),
			 attacker.owner, &prizePlanet, defender);
        }
    }
}


//************************************************************************
// Set up the game board for a new game
//************************************************************************


void
GameLogic::startNewGame()
{
    shutdownGame();

    if( gameInProgress )
        return;

    currentPlayer = players.begin();

    m_turnNumber = 1;
}


//************************************************************************
// Shut down the current game
//************************************************************************

void
GameLogic::shutdownGame()
{
    if( !gameInProgress )
        return;

    gameOver();
}

void
GameLogic::gameOver()
{
    cleanupGame();
}

void
GameLogic::cleanupGame()
{
    map->clearMap();

    planets.clear();
    players.clear();
}


//************************************************************************
// Player clicked the 'End Turn' button
//************************************************************************


void
GameLogic::nextPlayer()
{
    // end turn and advance to next player
    do {
        ++currentPlayer;
    } while (currentPlayer != players.end()
	     && !(*currentPlayer)->isInPlay());

    if( currentPlayer == players.end() ) {
        // end of player list, new turn
        currentPlayer = players.begin();
        nextTurn();
    }
}
