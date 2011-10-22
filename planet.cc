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

#include "planet.h"
#include "sector.h"
#include "fleet.h"
#include "game.h"
#include "players/player.h"
#include <krandomsequence.h>
#include <QDebug>

//---------------------------------------------------------------------------
// class Planet
//---------------------------------------------------------------------------

Planet::Planet( const QString &planetName, Sector *sector, Player *initialOwner,
                int newProd, double newKillP )
  : m_name(planetName),
    m_owner(initialOwner),
    m_sector(sector),
    m_homeFleet( this, 0 ),
    m_killPercentage(newKillP),
    m_productionRate(newProd), m_originalProductionRate(newProd),
    m_oldShips(newProd),
    m_showCurShips(true),
    m_justconquered(false)
{
    KRandomSequence r;
    m_planetLook = r.getLong(9);
    connect(&m_homeFleet, SIGNAL(update()), this, SIGNAL(update()));
    m_sector->setPlanet( this );
}

Planet::~Planet() {  m_sector->removePlanet(); }


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
    double     killP          = Game::generateKillPercentage();
    int        productionRate = (int) Game::generatePlanetProduction();

    return new Planet( planetName, sector,
                       initialOwner, productionRate, killP );
}

void
Planet::conquer( AttackFleet *conqueringFleet )
{
    m_owner->deleteStandingOrders(this);
    m_owner = conqueringFleet->owner;
    m_owner->statPlanetsConquered(1);
    m_homeFleet.become( conqueringFleet );
    m_productionRate = m_originalProductionRate;
    m_justconquered = true;
}

void
Planet::turn(const GameOptions &options)
{
    qDebug() << "Planet::turn...";

    if (options.ProductionAfterConquere || !m_justconquered) {
        if (m_owner->isNeutral() )
            m_homeFleet.addShips( options.NeutralsProduction );
        else {
            m_homeFleet.addShips( m_productionRate );
            m_owner->statShipsBuilt( m_productionRate );
        }
      
        if (options.CumulativeProduction)
            m_productionRate++;
    }

    m_oldShips = m_homeFleet.shipCount();
    m_showCurShips = true;
    m_justconquered = false;
    emit update();
}
