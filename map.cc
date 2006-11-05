#include "map.h"
#include "map.moc"

//---------------------------------------------------------------------------
// class Map
//---------------------------------------------------------------------------


Map::Map()
    : freezeUpdates( false ),
    m_rows( BOARD_ROWS ), m_columns( BOARD_COLS ),
    hasSelectedSector( false )
{
   // initialize the grid of Sectors
    for( int x = 0; x < columns(); x++ )
    {
        for( int y = 0; y < rows(); y++ )
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
