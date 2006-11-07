#include <math.h>
#include "gamecore.h"
#include "map.h"
#include "map.moc"

//---------------------------------------------------------------------------
// class Map
//---------------------------------------------------------------------------


Map::Map()
  : m_rows( BOARD_ROWS ),
    m_columns( BOARD_COLS ),
    m_hasSelectedSector( false )
{
   // initialize the grid of Sectors
    for( int col = 0; col < columns(); col++ )
    {
        for( int row = 0; row < rows(); row++ )
        {
            m_grid[row][col] = Sector( this, Coordinate(row, col) );
            connect( &m_grid[row][col], SIGNAL( update() ), this, SLOT( childSectorUpdate() ));
        }
    }
}

Map::~Map()
{
}


void
Map::clearMap()
{
    int x,y;

    for( x = 0; x < rows(); x++ )
        for( y = 0; y < columns(); y++ )
        {
            m_grid[y][x].removePlanet();
        }
    emit update();
}

void
Map::populateMap( QList<Player *> &players, Player *neutral,
                  int numNeutralPlanets, QList<Planet *> &thePlanets )
{
    int index = 0;
    QString names( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*(),.<>;:[]{}/?-+\\|" );

    // Create a planet for each player
    foreach(Player *plr, players) {
        QString newName( names.mid( index++, 1 ) );
        Sector *sect = findRandomFreeSector();
        Planet *plrPlanet = Planet::createPlayerPlanet( sect, plr, newName );

        thePlanets.append( plrPlanet );
    }

    for( int x = 0; x < numNeutralPlanets; x++ )
    {
        QString newName( names.mid( index++, 1 ) );
        Sector *sect = findRandomFreeSector();
        Planet *neutralPlanet = Planet::createNeutralPlanet( sect, neutral, newName );

        thePlanets.append( neutralPlanet );
    }
    emit update();
}

double Map::distance( Planet *p1, Planet *p2 )
{
    Coordinate  diff = p1->sector()->coord() - p2->sector()->coord();
    return sqrt(double((diff.x()*diff.x()) + (diff.y()*diff.y())))/2;
}


Sector *
Map::findRandomFreeSector()
{
    CoreLogic cl;
    Coordinate c;

    do
    {
        c = cl.generatePlanetCoordinates();
    } while( m_grid[c.y()][c.x()].hasPlanet() );

    return &m_grid[c.y()][c.x()];
}

bool
Map::selectedSector( Coordinate &c ) const
{
    if( m_hasSelectedSector)
    {
        c = m_selection;
    }

    return m_hasSelectedSector;
}

void
Map::setSelectedSector( Coordinate c )
{
    m_hasSelectedSector = true;
    m_selection = c;

    emit update();
}

void
Map::setSelectedSector()
{
    m_hasSelectedSector = false;

    emit update();
}

void Map::childSectorUpdate()
{
    emit update();
}

