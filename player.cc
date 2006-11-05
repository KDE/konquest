#include "map.h"
#include "player.h"
#include "planet.h"

//---------------------------------------------------------------------------
// class Player
//---------------------------------------------------------------------------

Player::Player( Map *map, QString newName, QColor newColor, int newPlrNum, 
		bool isAi )
  : m_map( map ),
    m_name( newName ), m_color( newColor ),
    m_playerNum( newPlrNum ),
    m_inPlay( true ),
    m_aiPlayer( isAi ),
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


Player *Player::createPlayer( Map *map, QString newName, QColor color, int playerNum, 
			      bool isAi )
{
    return new Player( map, newName, color, playerNum, isAi );
}

Player *Player::createNeutralPlayer( Map *map )
{
    return new Player( map, QString::null, Qt::gray, NEUTRAL_PLAYER_NUMBER, 
		       false );
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
