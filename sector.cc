/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
