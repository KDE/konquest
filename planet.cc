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
#include "planet.h"
#include "planet.moc"

#include "sector.h"
#include "fleet.h"
#include "player.h"
#include "map.h"
#include "gamecore.h"
#include <krandomsequence.h>

//---------------------------------------------------------------------------
// class Planet
//---------------------------------------------------------------------------

Planet::Planet( const QString &planetName, Sector *sector, Player *initialOwner,
                int newProd, double newKillP )
  : m_name(planetName),
    m_owner(initialOwner),
    m_sector(sector),
    m_homeFleet( this, newProd ),
    m_killPercentage(newKillP),
    m_productionRate(newProd)
{
    KRandomSequence r;
    m_planetLook = r.getLong(9);
    connect(&m_homeFleet, SIGNAL(update()), this, SIGNAL(update()));
    m_sector->setPlanet( this );
}

Planet::~Planet() {}

Planet *
Planet::createPlayerPlanet( Sector *sector, Player *initialOwner, 
			    const QString &planetName )
{
    return new Planet( planetName, sector, initialOwner, 10, 0.400 );
}


Planet *
Planet::createNeutralPlanet( Sector *sector, Player *initialOwner, 
			     const QString &planetName )
{
    CoreLogic  clogic;
    double     killP          = clogic.generateKillPercentage();
    int        productionRate = (int)clogic.generatePlanetProduction();

    return new Planet( planetName, sector,
                       initialOwner, productionRate, killP );
}

void
Planet::select()
{
    m_sector->select();

    emit selected();
}


void
Planet::conquer( AttackFleet *conqueringFleet )
{
    m_owner = conqueringFleet->owner;
    m_owner->statPlanetsConquered(1);
    m_homeFleet.become( conqueringFleet );
}

void
Planet::turn()
{
    if( !(m_owner->isNeutral()) ) {
        m_homeFleet.addShips( m_productionRate );
        m_owner->statShipsBuilt( m_productionRate );
    } else {
        m_homeFleet.addShips( 1 );
    }
    emit update();
}
