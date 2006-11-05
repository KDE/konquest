#include "gamecore.h"
#include "gamecore.moc"

#include <stddef.h>
#include <math.h>

#include <config.h>
#include <iostream>
#include "planet.h"
#include "player.h"

//*******************************************************************
// Game Core Logic
//*******************************************************************

CoreLogic::CoreLogic()
{
    random.setSeed(0);
}

Coordinate
CoreLogic::generatePlanetCoordinates()
{
    // 0 - 15
    return Coordinate(random.getLong(16), random.getLong(16));
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
    Coordinate diff = p1->sector().getCoord() - p2->sector().getCoord();
    
    diff /= 2; // Why do this?

    return sqrt(double((diff.x()*diff.x()) + (diff.y()*diff.y())));
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
    : freezeUpdates( false ),
    rows( BOARD_ROWS ), columns( BOARD_COLS ),
    hasSelectedSector( false )
{
   // initialize the grid of Sectors
    for( int x = 0; x < columns; x++ )
    {
        for( int y = 0; y < rows; y++ )
        {
            grid[y][x] = Sector( this, Coordinate(y, x) );
            connect( &grid[y][x], SIGNAL( update() ), this, SLOT( childSectorUpdate() ));
        }
    }
}

Map::~Map()
{
}


void
Map::clearMap()
{
    Freeze();

    int x,y;

    for( x = 0; x < rows; x++ )
        for( y = 0; y < columns; y++ )
        {
            grid[y][x].removePlanet();
        }

    Thaw();

    emit update();
}


void
Map::populateMap( PlayerList &players, Player *neutral,
                  int numNeutralPlanets, PlanetList &thePlanets )
{
    Freeze();

    int index = 0;
    QString names( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*(),.<>;:[]{}/?-+\\|" );

    // Create a planet for each player
    foreach(Player *plr, players) {
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



Sector &
Map::findRandomFreeSector()
{
    CoreLogic cl;
    Coordinate c;

    do
    {
        c = cl.generatePlanetCoordinates();
    } while( grid[c.y()][c.x()].hasPlanet() );

    return grid[c.y()][c.x()];
}

bool
Map::selectedSector( Coordinate &c ) const
{
    if( hasSelectedSector)
    {
        c = sel;
    }

    return hasSelectedSector;
}

void
Map::setSelectedSector( Coordinate c )
{
    hasSelectedSector = true;
    sel = c;

    emit update();
}

void
Map::setSelectedSector( const Planet &planet )
{
    hasSelectedSector = true;
    sel = planet.sector().getCoord();

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

Sector &Map::getSector( Coordinate c )
{
    return grid[c.y()][c.x()];
}


//---------------------------------------------------------------------------
// class Sector
//---------------------------------------------------------------------------

Sector::Sector()
: planet( NULL ), parentMap(NULL ), c( 0,0 )
{}

Sector::Sector( Map *newParentMap, Coordinate _c )
: planet(NULL), parentMap( newParentMap ), c(_c)
{
}

Sector::Sector( const Sector & other )
: QObject( 0 ), planet(other.planet), parentMap(other.parentMap), c(other.c)
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
    c = other.c;
    planet = other.planet;
    parentMap = other.parentMap;

    return *this;
}

void
Sector::select()
{
    parentMap->setSelectedSector( c );
    emit selected();
}

Coordinate Sector::getCoord()
{
    return c;
}
