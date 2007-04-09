#include <math.h>
#include "gamecore.h"
#include "map.h"
#include "map.moc"
#include <QtDebug>

//---------------------------------------------------------------------------
// class Map
//---------------------------------------------------------------------------


Map::Map(int rowsCount, int colsCount)
  : m_rows( rowsCount ),
    m_columns( colsCount ),
    m_hasSelectedSector( false )
{
    resizeMap(rowsCount, colsCount);
}

void Map::resizeMap (int rowsCount, int columnsCount) {
    m_rows = rowsCount;
    m_columns = columnsCount;
    m_grid.clear();
    m_grid = QList<QList<Sector> >();
    // initialize the grid of Sectors
    for( int row = 0; row < rowsCount; row++ )
    {
        m_grid << QList<Sector>();
        for( int col = 0; col < columnsCount; col++ )
        {
            m_grid[row] << Sector( this, Coordinate(row, col) );
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
            m_grid[x][y].removePlanet();
        }
    emit update();
}

void
Map::populateMap( QList<Player *> &players, Player *neutral,
                  int numNeutralPlanets, QList<Planet *> &thePlanets )
{
    // Store the planet name in a simple char... Nothing more needed.
    char index = 64;

    // Create a planet for each player
    foreach(Player *plr, players) {
        QString newName( index++ );
        Sector *sect = findRandomFreeSector();
        Planet *plrPlanet = Planet::createPlayerPlanet( sect, plr, newName );

        thePlanets.append( plrPlanet );
    }

    for( int x = 0; x < numNeutralPlanets; x++ )
    {
        QString newName( index++ );
        Sector *sect = findRandomFreeSector();
        Planet *neutralPlanet = Planet::createNeutralPlanet( sect, neutral, newName );

        thePlanets.append( neutralPlanet );
    }
    emit update();
    emit mapPopulated();
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
        c = cl.generatePlanetCoordinates(rows(), columns());
    } while( m_grid[c.x()][c.y()].hasPlanet() );

    return &m_grid[c.x()][c.y()];
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

