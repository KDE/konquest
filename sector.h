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
#ifndef __SECTOR_H__
#define __SECTOR_H__

#include <QObject>
#include <QPoint>

class Map;
class Planet;

// -------------------------------------------------------------------------
// Typedefs
// -------------------------------------------------------------------------

typedef QPoint Coordinate;  // Gotta start using this instead of int x,y crap

// ***************************************************************
// class Sector
// ***************************************************************

class Sector : public QObject
{
    Q_OBJECT

public:

    // constructors
    Sector();
    Sector( Map *parentMap, Coordinate c );
    Sector( const Sector & );

    // assignment operator (makes initialization easy)
    Sector &operator=( const Sector & );

    Coordinate  coord()     const { return m_coord; }
    bool        hasPlanet() const { return m_planet != NULL; }
    Planet     *planet()    const { return m_planet; }
    void        setPlanet( Planet *planet );
    void        removePlanet();

    void        select();


signals:
    void        update();
    void        selected();

protected slots:
    void        childPlanetUpdate( );

    
protected:
    Map        *m_map;
    Coordinate  m_coord;
    Planet     *m_planet;  // a sector has 0 or 1 planets
};


#endif
