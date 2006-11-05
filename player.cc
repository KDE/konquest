#include "player.h"
#include "planet.h"
#include "gamecore.h"

//---------------------------------------------------------------------------
// class Player
//---------------------------------------------------------------------------
Player::Player( QString newName, QColor newColor, int newPlrNum, bool isAi )
  : m_name( newName ), m_color( newColor ),
    m_playerNum( newPlrNum ),
    m_inPlay( true ),
    m_aiPlayer( isAi ),
 shipsBuilt(0), planetsConquered(0), fleetsLaunched(0),
enemyFleetsDestroyed(0), enemyShipsDestroyed(0)
{
}

Player::~Player()
{
}

bool
Player::operator==( const Player &otherPlayer ) const
{
    if( m_playerNum == otherPlayer.m_playerNum )
        return true;
    else
        return false;
}

QString
Player::coloredName() const
{
    return QString("<font color=\"%1\">%2</font>").arg(m_color.name(), m_name);
}

Player *Player::createPlayer( QString newName, QColor color, int playerNum, bool isAi )
{
    return new Player( newName, color, playerNum, isAi );
}

Player *Player::createNeutralPlayer()
{
    return new Player( QString::null, Qt::gray, NEUTRAL_PLAYER_NUMBER, false );
}

void Player::setInPlay( bool status )
{
    m_inPlay = status;
}

AttackFleetList &
Player::getAttackList()
{
    return attackList;
}

bool
Player::NewAttack( Planet *sourcePlanet, Planet *destPlanet, int shipCount, int turn )
{
    CoreLogic cl;

    double arrival = cl.distance( sourcePlanet, destPlanet ) + turn;

    AttackFleet *fleet = sourcePlanet->getFleet().spawnAttackFleet( destPlanet, shipCount, arrival );


    if( fleet ) {
        attackList.append(fleet);

        statFleetsLaunched( 1 );

        return true;
    }

    return false;
}

// Player Statistics collection
void Player::statShipsBuilt( int x )
{
    shipsBuilt += x;
}

void Player::statPlanetsConquered( int x )
{
    planetsConquered += x;
}

void Player::statFleetsLaunched( int x )
{
    fleetsLaunched += x;
}

void Player::statEnemyFleetsDestroyed( int x )
{
    enemyFleetsDestroyed += x;
}

void Player::statEnemyShipsDestroyed( int x )
{
    enemyShipsDestroyed += x;
}

bool Player::isAiPlayer() {
  return m_aiPlayer;
}
