#include <math.h>

#include "map.h"
#include "map.moc"

//---------------------------------------------------------------------------
// class Map
//---------------------------------------------------------------------------


Map::Map()
  : freezeUpdates( false ),
    m_rows( BOARD_ROWS ),
    m_columns( BOARD_COLS ),
    hasSelectedSector( false )
{
   // initialize the grid of Sectors
    for( int col = 0; col < columns(); col++ )
    {
        for( int row = 0; row < rows(); row++ )
        {
            grid[row][col] = Sector( this, Coordinate(row, col) );
            connect( &grid[row][col], SIGNAL( update() ), this, SLOT( childSectorUpdate() ));
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

    for( x = 0; x < rows(); x++ )
        for( y = 0; y < columns(); y++ )
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

double Map::distance( Planet *p1, Planet *p2 )
{
    Coordinate  diff = p1->sector().coord() - p2->sector().coord();
    return sqrt(double((diff.x()*diff.x()) + (diff.y()*diff.y())))/2;
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
    sel = planet.sector().coord();

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
