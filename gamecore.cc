#include "gamecore.h"
#include "gamecore.moc"

#include <stddef.h>
#include <math.h>

#include <config.h>

//*******************************************************************
// Game Core Logic
//*******************************************************************

bool CoreLogic::class_init = false;

CoreLogic::CoreLogic()
{
    random.setSeed(0);
}

void
CoreLogic::generatePlanetCoordinates( int &x, int &y )
{
    // 0 - 15
    x = random.getLong(16);
    y = random.getLong(16);
}

double
CoreLogic::generateKillPercentage()
{
    // 0.30 - 0.90
    return 0.30 + random.getDouble()*0.60;
}

int
CoreLogic::generatePlanetProduction()
{
    // 5 - 15
    return 5 + random.getLong(10);
}

double
CoreLogic::generateMorale()
{
    // constant
    return 0.50;
}

double
CoreLogic::distance( Planet *p1, Planet *p2 )
{
    int k = (p1->getSector().getRow() - p2->getSector().getRow()) / 2;
    int l = (p1->getSector().getColumn() - p2->getSector().getColumn()) / 2;

    return sqrt(double((k*k) + (l*l)));
}

double
CoreLogic::roll()
{
    // 0.00 - 1.00
    return random.getDouble();
}

//---------------------------------------------------------------------------
// class Map
//---------------------------------------------------------------------------


Map::Map()
    : QObject( 0, 0 ),  freezeUpdates( false ),
    rows( BOARD_ROWS ), columns( BOARD_COLS ),
    hasSelectedSector( false )
{
   // initialize the grid of Sectors
    for( int x = 0; x < rows; x++ )
    {
        for( int y = 0; y < columns; y++ )
        {
            grid[x][y] = Sector( this, x, y );
            connect( &grid[x][y], SIGNAL( update() ), this, SLOT( childSectorUpdate() ));
        }
    }
}

Map::~Map()
{
}

void
Map::populateMap( PlayerList &players, Player *neutral,
                  int numNeutralPlanets, PlanetList &thePlanets )
{
    Freeze();

    int index = 0;
    QString names( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*(),.<>;:[]{}/?-+\\|" );

    // Create a planet for each player
    Player *plr;
    for( plr = players.first(); plr != 0; plr = players.next() )
    {
        QString newName( names.mid( index++, 1 ) );
        Sector &sect = findRandomFreeSector();
        Planet *plrPlanet = Planet::createPlayerPlanet( sect, plr, newName );

        thePlanets.append( plrPlanet );
    }

    for( int x = 0; x < numNeutralPlanets; x++ )
    {
        QString newName( names.mid( index++, 1 ) );
        Sector &sect = findRandomFreeSector();
        Planet *neutralPlanet = Planet::createNeutralPlanet( sect, neutral, newName );

        thePlanets.append( neutralPlanet );
    }

    Thaw();

    emit update();
}

void
Map::clearMap()
{
    Freeze();

    int x,y;

    for( x = 0; x < rows; x++ )
        for( y = 0; y < columns; y++ )
        {
            grid[x][y].removePlanet();
        }


    Thaw();

    emit update();
}

Sector &
Map::findRandomFreeSector()
{
    CoreLogic cl;

    bool found = false;

    while( !found )
    {
        int x,y;

        cl.generatePlanetCoordinates( x,y );

        if( !grid[x][y].hasPlanet() )
        {
            return grid[x][y];
        }
    }

    // TODO: get rid of this
    return grid[0][0];

}

bool
Map::selectedSector( int &x, int &y ) const
{
    if( hasSelectedSector)
    {
        x = sel_x;
	y = sel_y;
    }

    return hasSelectedSector;

}

void
Map::setSelectedSector( int x, int y )
{
    hasSelectedSector = true;
    sel_x = x;
    sel_y = y;

    emit update();
}

void
Map::setSelectedSector( const Planet &planet )
{
    hasSelectedSector = true;
    sel_x = planet.getSector().getRow();
    sel_y = planet.getSector().getColumn();

    emit update();
}

void
Map::setSelectedSector()
{
    hasSelectedSector = false;

    emit update();
}


void Map::childSectorUpdate()
{
    if( !freezeUpdates )
        emit update();
}

void Map::Freeze()
{
    freezeUpdates = true;
}

void Map::Thaw()
{
    freezeUpdates = false;
}

Sector &Map::getSector( int x, int y )
{
    return grid[x][y];
}

const int Map::getRows() const
{
    return rows;
}

const int Map::getColumns() const
{
    return columns;
}

//---------------------------------------------------------------------------
// class Sector
//---------------------------------------------------------------------------

Sector::Sector()
: QObject(0,0), planet( NULL ), parentMap(NULL ), x(0), y(0)
{}

Sector::Sector( Map *newParentMap, int xPos, int yPos )
: QObject(0,0), planet(NULL), parentMap( newParentMap ), x(xPos), y(yPos)
{
}

Sector::Sector( const Sector & other )
: QObject(0,0), planet(other.planet), parentMap(other.parentMap), x(other.x), y(other.y)
{
}

bool Sector::hasPlanet() const
{
    return (planet != NULL);
}


void Sector::setPlanet( Planet *newPlanet )
{
    planet = newPlanet;

    connect( planet, SIGNAL( update() ), this, SLOT( childPlanetUpdate() ) );

    emit update();
}

Planet *Sector::getPlanet()
{
    return planet;
}

void Sector::removePlanet()
{
    planet = NULL;

    emit update();
}


void Sector::childPlanetUpdate()
{
    emit update();
}

Sector &
Sector::operator=( const Sector &other )
{
    x = other.x;
    y = other.y;
    planet = other.planet;
    parentMap = other.parentMap;

    return *this;
}

void
Sector::select()
{
    parentMap->setSelectedSector( x, y );
    emit selected();
}

int Sector::getRow()
{
    return x;
}

int Sector::getColumn()
{
    return y;
}

//---------------------------------------------------------------------------
// class Planet
//---------------------------------------------------------------------------

Planet::Planet( QString planetName, Sector &newParentSector, Player *initialOwner,
                int newProd, double newKillP, double newMorale )
       : QObject(0,0), name(planetName), owner(initialOwner), parentSector(newParentSector),
         homeFleet( this, newProd ), killPercentage(newKillP), morale( newMorale ), productionRate(newProd)


{
    parentSector.setPlanet( this );
}

Planet::~Planet() {}

Planet *
Planet::createPlayerPlanet( Sector &parentSector, Player *initialOwner, QString planetName )
{
    CoreLogic clogic;

    double morale = clogic.generateMorale();

    return new Planet( planetName, parentSector, initialOwner,
                       10, 0.400, morale );
}

Planet *
Planet::createNeutralPlanet( Sector &parentSector, Player *initialOwner, QString planetName )
{
    CoreLogic clogic;
    double morale = clogic.generateMorale();

    double killP = clogic.generateKillPercentage();

    int productionRate = (int)clogic.generatePlanetProduction();

    return new Planet( planetName, parentSector,
                       initialOwner, productionRate, killP, morale );
}

double
Planet::getKillPercentage()
{
    return killPercentage;
}

void
Planet::setKillPercentage( double newValue )
{
    killPercentage = newValue;

    emit update();
}

double
Planet::getMorale()
{
    return morale;
}

void
Planet::setMorale( double newMorale )
{
    morale = newMorale;
}

int
Planet::getProduction()
{
    return productionRate;
}

void
Planet::setProduction( int newProduction )
{
    productionRate = newProduction;
}

void
Planet::select()
{
    parentSector.select();

    emit selected();
}

DefenseFleet &Planet::getFleet()
{
    return homeFleet;
}

Player *
Planet::getPlayer() const
{
    return owner;
}

const QString &
Planet::getName() const
{
    return name;
}

Sector &
Planet::getSector() const
{
    return parentSector;
}

void
Planet::conquer( AttackFleet *conqueringFleet )
{
    owner = conqueringFleet->owner;
    owner->statPlanetsConquered(1);
    homeFleet.become( conqueringFleet );
}

void
Planet::coup( Player *luckyPlayer )
{
    owner = luckyPlayer;
}

void
Planet::turn()
{
    if( !(owner->isNeutral()) ) {
        homeFleet.addShips( productionRate );
        owner->statShipsBuilt( productionRate );
    } else {
        homeFleet.addShips( 1 );
    }
}

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
    return new Player( QString::null, gray, NEUTRAL_PLAYER_NUMBER, false );
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

//---------------------------------------------------------------------------
// class Fleet
//   \---class AttackFleet
//    \---class DefenseFleet
//---------------------------------------------------------------------------


Fleet::Fleet( int initialShipCount )
: shipCount( initialShipCount )
{
}

int
Fleet::getShipCount()
{
    return shipCount;
}

void
Fleet::removeShips( int lostShips )
{
    shipCount -= lostShips;
}

AttackFleet::AttackFleet( Planet *source, Planet *dest, int initialCount, double arrival )
: Fleet( initialCount ), owner( source->getPlayer() ), destination( dest ), arrivalTurn( arrival ),
killPercentage( source->getKillPercentage() )
{
}

DefenseFleet::DefenseFleet( Planet *newHome, int initialCount ) : Fleet( initialCount ), home( newHome )
{
}

void
DefenseFleet::absorb( AttackFleet *fleet )
{
    shipCount += fleet->getShipCount();
}

void
DefenseFleet::become( AttackFleet *fleet )
{
    shipCount = fleet->getShipCount();
}


AttackFleet *
DefenseFleet::spawnAttackFleet( Planet *dest, int count, double arrivalTurn )
{
    if( shipCount < count ) {
        return NULL;
    }

    AttackFleet *newFleet = new AttackFleet( home, dest, count, arrivalTurn  );

    removeShips( count );

    return newFleet;
}

void
DefenseFleet::addShips( int newShips )
{
    shipCount += newShips;
}

