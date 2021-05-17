/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "planet.h"
#include "sector.h"
#include "fleet.h"
#include "game.h"
#include "players/player.h"
#include <QRandomGenerator>
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
    m_planetLook = QRandomGenerator::global()->bounded(10);
    connect(&m_homeFleet, &DefenseFleet::update, this, &Planet::update);
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
    //qDebug() << "Planet::turn...";

    if (options.ProductionAfterConquere || !m_justconquered) {
        if (m_owner->isNeutral()) {
            m_homeFleet.addShips(options.NeutralsProduction);
            m_owner->statShipsBuilt(options.NeutralsProduction);
        }
        else {
            m_homeFleet.addShips(m_productionRate);
            m_owner->statShipsBuilt(m_productionRate);
        }

        m_owner->statShipCount(m_homeFleet.shipCount());

        if (options.CumulativeProduction) {
            m_productionRate++;
        }
    }

    m_oldShips = m_homeFleet.shipCount();
    m_showCurShips = true;
    m_justconquered = false;
    Q_EMIT update();
}
