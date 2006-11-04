#include "player.h"
#include "player.moc"
#include "planet.h"
#include "gamecore.h"

//---------------------------------------------------------------------------
// class Player
//---------------------------------------------------------------------------
Player::Player( QString newName, QColor newColor, int newPlrNum, bool isAi ) : name( newName ), color( newColor ),
playerNum( newPlrNum ), inPlay( true ), aiPlayer( isAi ), shipsBuilt(0), planetsConquered(0), fleetsLaunched(0),
enemyFleetsDestroyed(0), enemyShipsDestroyed(0)
{
}

Player::~Player()
{
}

bool
Player::operator==( const Player &otherPlayer ) const
{
    if( playerNum == otherPlayer.playerNum )
        return true;
    else
        return false;
}

QString &
Player::getName()
{
    return name;
}

QString
Player::getColoredName()
{
    return QString("<font color=\"%1\">%2</font>").arg(color.name(), name);
}

Player *Player::createPlayer( QString newName, QColor color, int playerNum, bool isAi )
{
    return new Player( newName, color, playerNum, isAi );
}

Player *Player::createNeutralPlayer()
{
    return new Player( QString::null, Qt::gray, NEUTRAL_PLAYER_NUMBER, false );
}

QColor &Player::getColor()
{
    return color;
}

bool
Player::isNeutral()
{
    if( playerNum == NEUTRAL_PLAYER_NUMBER ) {
        return true;
    } else {
        return false;
    }
}

bool Player::isInPlay()
{
    return inPlay;
}

void Player::setInPlay( bool status )
{
    inPlay = status;
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
  return aiPlayer;
}
