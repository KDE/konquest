/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_SECTOR_H
#define KONQUEST_SECTOR_H

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
    bool        hasPlanet() const { return m_planet != nullptr; }
    Planet     *planet()    const { return m_planet; }
    void        setPlanet( Planet *planet );
    void        removePlanet();

Q_SIGNALS:
    void        update();

protected:
    void        childPlanetUpdate( );

    Map        *m_map;
    Coordinate  m_coord;
    Planet     *m_planet;  // a sector has 0 or 1 planets
};


#endif // KONQUEST_SECTOR_H
