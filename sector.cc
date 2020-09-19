/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>

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

#include "sector.h"
#include "planet.h"
#include "map/map.h"

//---------------------------------------------------------------------------
// class Sector
//---------------------------------------------------------------------------

Sector::Sector()
  : m_map(nullptr ),
    m_coord( 0,0 ),
    m_planet( nullptr )
{}

Sector::Sector( Map *map, Coordinate coord )
  : m_map( map ),
    m_coord( coord ),
    m_planet(nullptr)
{
}

Sector::Sector( const Sector & other )
  : QObject( nullptr ),
    m_map(other.m_map), 
    m_coord(other.m_coord),
    m_planet(other.m_planet)
{
}


void Sector::setPlanet( Planet *planet )
{
    m_planet = planet;

    connect(m_planet, &Planet::update, this, &Sector::childPlanetUpdate);

    Q_EMIT update();
}

void Sector::removePlanet()
{
    m_planet = nullptr;

    Q_EMIT update();
}


void Sector::childPlanetUpdate()
{
    Q_EMIT update();
}

Sector &
Sector::operator=( const Sector &other )
{
    m_coord  = other.m_coord;
    m_planet = other.m_planet;
    m_map    = other.m_map;

    return *this;
}
