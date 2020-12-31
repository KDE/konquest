/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "map.h"
#include "../game.h"
#include "../planet.h"
#include <cmath>


Map::Map(int rowsCount, int colsCount) :
    m_rows(rowsCount),
    m_columns(colsCount)
{
    resizeMap(rowsCount, colsCount);
}


Map::~Map()
{
}


void
Map::resizeMap (int rowsCount, int columnsCount)
{
    m_rows = rowsCount;
    m_columns = columnsCount;
    m_grid.clear();
    m_grid = QList<QList<Sector> >();

    // Initialize the grid of Sectors.
    for (int row = 0; row < rowsCount; row++) {
        m_grid << QList<Sector>();
        for (int col = 0; col < columnsCount; col++) {
            m_grid[row] << Sector(this, Coordinate(row, col));
            connect(&m_grid[row][col], &Sector::update, this, &Map::childSectorUpdate);
        }
    }
}


void
Map::addPlanet(Sector *sector, Player *player, int production, double killpercentage)
{
    new Planet(UniquePlanetName(), sector, player, production, killpercentage);
}


Planet*
Map::addPlayerPlanetSomewhere(Player *player)
{
    Sector *sector = findRandomFreeSector();
    if (!sector)
        return nullptr;
    return Planet::createPlayerPlanet(sector, player, UniquePlanetName());
}


Planet*
Map::addNeutralPlanetSomewhere(Player *neutral)
{
    Sector *sector = findRandomFreeSector();
    if (!sector)
        return nullptr;
    return Planet::createNeutralPlanet(sector, neutral, UniquePlanetName());
}


bool
Map::removePlayerPlanet(Player *player)
{
    for (Planet *planet : planets()) {
        if (planet->player() == player) {
            delete planet;
            return true;
        }
    }
    return false;
}


void
Map::removePlayerPlanets(Player *player)
{
    while(removePlayerPlanet(player)) ;
}


void
Map::turnOverPlayerPlanets(Player* owner, Player* newOwner)
{
    for (Planet *planet : planets()) {
        if (planet->player() == owner) {
            planet->setOwner(newOwner);
        }
    }
}


int
Map::playerPlanetCount(Player *player)
{
    int count = 0;
    for (Planet *planet : planets()) {
        if (planet->player() == player) {
            count++;
        }
    }
    return count;
}


void
Map::clearMap()
{
    for (int x = 0; x < rows(); ++x) {
        for (int y = 0; y < columns(); ++y) {
             delete m_grid[x][y].planet();
        }
    }

    Q_EMIT update();
}


QString
Map::UniquePlanetName(void)
{
    char c = 'A';
again:
    for (Planet *planet : planets()) {
        if (planet->name().at(0).toLatin1() == c) {
            c++;
            goto again;
        }
    }
    return QChar::fromLatin1(c);
}


void
Map::populateMap(const QList<Player *> &players, Player *neutral, int numNeutralPlanets)
{
    // Create a planet for each player.
    for (Player *player : players) {
        addPlayerPlanetSomewhere(player);
    }

    for (int x = 0; x < numNeutralPlanets; ++x) {
        Sector *sector = findRandomFreeSector();
        if (sector) {
            Planet::createNeutralPlanet(sector, neutral, UniquePlanetName());
        }
    }

    Q_EMIT update();
}


double
Map::distance(Planet *p1, Planet *p2)
{
    Coordinate  diff = p1->sector()->coord() - p2->sector()->coord();

    return sqrt(double((diff.x() * diff.x()) + (diff.y() * diff.y()))) / 2;	// Yes, we're dividing by two. It's not a bug, it's a feature.
}


Sector*
Map::findRandomFreeSector()
{
    for (const QList<Sector> &i : qAsConst(m_grid)) {
        for (const Sector &j : i) {
            if (!j.hasPlanet()) {
                goto freesectorexists;
            }
        }
    }

    return nullptr;

freesectorexists:

    Coordinate  c;

    do {
        c = Game::generatePlanetCoordinates(rows(), columns());
    } while (m_grid[c.x()][c.y()].hasPlanet());

    return &m_grid[c.x()][c.y()];
}


void
Map::childSectorUpdate()
{
    Q_EMIT update();
}


const QList <Planet*>
Map::planets()
{
    QList <Planet*>planets;
    for (const QList<Sector> &i : qAsConst(m_grid)) {
        for (const Sector &j : i) {
            if (j.hasPlanet()) {
                planets += j.planet();
            }
        }
    }
    return planets;
}
