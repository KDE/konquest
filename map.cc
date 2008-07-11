/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright Inge Wallin <inge@lysator.liu.se>
    Copyright Pierre Ducroquet <pinaraf@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "map.h"

#include <math.h>
#include "gamecore.h"
#include "map.moc"
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

void Map::resizeMap (int rowsCount, int columnsCount)
{
    m_rows = rowsCount;
    m_columns = columnsCount;
    m_grid.clear();
    m_grid = QList<QList<Sector> >();

    // Initialize the grid of Sectors.
    for( int row = 0; row < rowsCount; row++ ) {
        m_grid << QList<Sector>();
        for( int col = 0; col < columnsCount; col++ ) {
            m_grid[row] << Sector( this, Coordinate( row, col ) );
            connect( &m_grid[row][col], SIGNAL( update() ), 
                     this,              SLOT( childSectorUpdate() ) );
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
        for( y = 0; y < columns(); y++ ) {
            m_grid[x][y].removePlanet();
        }
    emit update();
}

void
Map::populateMap( QList<Player *> &players, Player *neutral,
                  int numNeutralPlanets, QList<Planet *> &thePlanets )
{
    // Store the planet name in a simple char... Nothing more needed.
    //char planetName = 'A';
    char* planetNames = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@()[]|/\\:;,";
    int idx = 0;
    
    // Create a planet for each player.
    foreach(Player *plr, players) {
        QString  newName( planetNames[idx] );
        idx++;
        Sector  *sect      = findRandomFreeSector();
        Planet  *plrPlanet = Planet::createPlayerPlanet( sect, plr, newName );

        thePlanets.append( plrPlanet );
    }

    for( int x = 0; (x < numNeutralPlanets) && (planetNames[idx] != '\0'); x++ ) {
        QString  newName( planetNames[idx] );
        idx++;
        Sector  *sect = findRandomFreeSector();
        Planet  *neutralPlanet = Planet::createNeutralPlanet( sect, neutral, newName );

        thePlanets.append( neutralPlanet );
    }

    emit update();
    emit mapPopulated();
}

double Map::distance( Planet *p1, Planet *p2 )
{
    Coordinate  diff = p1->sector()->coord() - p2->sector()->coord();

    return sqrt( double( ( diff.x() * diff.x() ) 
                         + ( diff.y() * diff.y() ) ) )/2;	// Yes, we're dividing by two. It's not a bug, it's a feature.
}

Sector *Map::findRandomFreeSector()
{
    CoreLogic   cl;
    Coordinate  c;
    
    do {
        c = cl.generatePlanetCoordinates( rows(), columns() );
    } while( m_grid[c.x()][c.y()].hasPlanet() );

    return &m_grid[c.x()][c.y()];
}

void Map::setSelectedSector( Coordinate c )
{
    m_hasSelectedSector = true;
    m_selection = c;

    emit update();
}

void Map::setSelectedSector()
{
    m_hasSelectedSector = false;

    emit update();
}

void Map::childSectorUpdate()
{
    emit update();
}
