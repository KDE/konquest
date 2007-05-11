/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright <inge@lysator.liu.se>
    Copyright <pinaraf@gmail.com>

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
#ifndef __MAP_H__
#define __MAP_H__

#include <QObject>
#include <QList>
#include "sector.h"
#include "player.h"

//*****************************************************************
// class Map
//*****************************************************************


class Map : public QObject
{
    Q_OBJECT

public:
    Map(int rows, int cols);
    virtual ~Map();

    int  rows()     { return m_rows;    }
    int  columns()  { return m_columns; }

    void       clearMap();
    void       resizeMap(int rows, int cols);
    void       populateMap( QList<Player *> &players, Player *neutral,
			    int numNeutralPlanets, QList<Planet *> &thePlanets );
    
    double     distance( Planet *p1, Planet *p2 );

    bool hasSelectedSector() const { return m_hasSelectedSector; }
    Coordinate selectedSector() const { return m_selection; }
    void setSelectedSector( Coordinate c );
    void setSelectedSector();

    Sector *sector( Coordinate coord )  { return &m_grid[coord.y()][coord.x()]; }


protected slots:
    void childSectorUpdate();

signals:
    void update();
    void mapPopulated();

private:
    Sector *findRandomFreeSector();
    
protected:
    // A map is a 2-D array of Sectors.
    QList<QList<Sector> > m_grid;
    int  m_rows; // size of grid in sectors
    int  m_columns;

    // This is used to implement a selected sector,
    // one who's boarder flashes.
    bool        m_hasSelectedSector;
    Coordinate  m_selection;
};

#endif
