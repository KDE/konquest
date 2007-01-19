#include <math.h>

#include <qdebug.h>

#include "map.h"
#include "player.h"
#include "planet.h"
#include "gamelogic.h"


//---------------------------------------------------------------------------
// class Player
//---------------------------------------------------------------------------

Player::Player( Map *map, const QString &newName, QColor color, int newPlrNum )
  : m_map( map ),
    m_name( newName ), m_color( color ),
    m_playerNum( newPlrNum ),
    m_inPlay( true ),
    m_shipsBuilt(0),
    m_planetsConquered(0),
    m_fleetsLaunched(0),
    m_enemyFleetsDestroyed(0),
    m_enemyShipsDestroyed(0)
{
}

Player::~Player()
{
}


QString
Player::coloredName() const
{
    return QString("<font color=\"%1\">%2</font>").arg(m_color.name(), m_name);
}


Player *Player::createPlayer( Map *map, const QString &name, QColor color,
			      int playerNum, bool isAi )
{
    Player *pl;

    if ( isAi )
	pl = new AIPlayer( map, name, color, playerNum );
    else
	pl = new Player( map, name, color, playerNum );

    if ( pl->isAiPlayer() != isAi )
	qDebug("Player %s: wanted %d, got %d", name.latin1(), isAi, !isAi );

    return pl;
}


Player *Player::createNeutralPlayer( Map *map )
{
    return new Player( map, QString::null, Qt::gray, NEUTRAL_PLAYER_NUMBER );
}


bool
Player::NewAttack( Planet *sourcePlanet, Planet *destPlanet,
		   int shipCount, int turn )
{
    double       arrival = m_map->distance( sourcePlanet, destPlanet ) + turn;
    AttackFleet *fleet;

    fleet = sourcePlanet->fleet().spawnAttackFleet( destPlanet, shipCount, 
						    arrival );
    if( fleet ) {
        m_attackList.append(fleet);

        statFleetsLaunched( 1 );

        return true;
    }

    return false;
}


// ================================================================
//                      class AIPlayer
// ================================================================



AIPlayer::AIPlayer( Map *map, const QString &name, QColor color, int number )
    : Player( map, name, color, number )
{
    m_AiLevel = 0; // Default
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::doMove(GameLogic *gameLogic)
{
    int      ships, minimumShips, shipCountFactor;
    Planet  *target = 0;
    switch (getAiLevel())
    {
        case 1: // Offensive
            minimumShips = 10;
            shipCountFactor = 2;
            break;
        case 2: // Defensive
            minimumShips = 30;
            shipCountFactor = 3;
            break;
        default: // 0... default
            minimumShips = 20;
            shipCountFactor = 2;
    }
    foreach (Planet *home, *gameLogic->planets()) {
	if (home->player() == gameLogic->currentPlayer()) {
	    bool  hasAttack = false;
	    ships = (int)floor(home->fleet().shipCount() * 0.7 );
                
	    if (ships >= minimumShips) {
		Planet  *attack;
		double  minDistance = 100;
                    
		foreach (attack, *gameLogic->planets()) {
		    bool    skip = false;
		    double  dist = gameLogic->map()->distance( home, attack );
                        
		    if (dist < minDistance
			&&  attack->player() != gameLogic->currentPlayer()
			&& attack->fleet().shipCount() < ships ) {
			foreach (AttackFleet *curFleet, gameLogic->currentPlayer()->attackList()) {
			    if (curFleet->destination == attack) {
				skip = true;
			    }
			}
			if (skip) 
			    continue;
                            
			target      = attack;
			hasAttack   = true;
			minDistance = dist;
		    }
		}

		if (hasAttack) {
		    //sendAttackFleet( home, target, ships );
		    gameLogic->currentPlayer()
			->NewAttack( home, target, ships, 
				     gameLogic->turnNumber() );
		} else {
		    minDistance = 100;
		    int shipsToSend = 0;
		    bool hasDestination = false;
                        
		    foreach (attack, *gameLogic->planets()) {
			bool    skip = false;
			double  dist = gameLogic->map()->distance( home, attack );
			int     homeships = (int)floor(home->fleet().shipCount() * 0.5 );
                            
			if (dist < minDistance
			    && attack->player() == gameLogic->currentPlayer()
			    && attack->fleet().shipCount() < homeships ) {
			    foreach (AttackFleet *curFleet,
				     gameLogic->currentPlayer()->attackList()) {
				if (curFleet->destination == attack) {
				    skip = true;
				}
			    }
			    if (skip)
				continue;
                                
			    shipsToSend = (int)floor( double(home->fleet().shipCount()
							     - attack->fleet().shipCount()) / shipCountFactor);
                                
			    target         = attack;
			    hasDestination = true;
			    minDistance    = dist;
			}
		    }
		    if (hasDestination) {
			//FIXME: Move sendAttackFleet from GameView 
			//       into GameLogic.
			//sendAttackFleet( home, target, shipsToSend );
			gameLogic->currentPlayer()
			    ->NewAttack( home, target, shipsToSend, 
					 gameLogic->turnNumber() );
		    }
		}
	    }
	}
    }
}
