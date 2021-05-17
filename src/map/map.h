/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_MAP_H
#define KONQUEST_MAP_H

#include <QList>
#include <QObject>

#include "sector.h"
#include "../players/player.h"


class Map : public QObject
{
    Q_OBJECT

public:
    Map(int rows, int cols);
    virtual ~Map();

    int  rows()     { return m_rows;    }
    int  columns()  { return m_columns; }

    void    addPlanet(Sector *sector, Player *player, int production, double killpercentage);
    Planet* addPlayerPlanetSomewhere(Player *player);
    Planet* addNeutralPlanetSomewhere(Player *neutral);
    bool    removePlayerPlanet(Player *player);
    void    removePlayerPlanets(Player *player);
    int     playerPlanetCount(Player *player);
    void    turnOverPlayerPlanets(Player* owner, Player* newOwner);

    void    clearMap();
    void    resizeMap(int rows, int cols);

    void    populateMap( const QList<Player*> &players, Player *neutral, int numNeutralPlanets);

    double  distance( Planet *p1, Planet *p2 );

    Sector *sector(Coordinate coord)
    {
        if (coord.y() >= 0 && coord.y() < m_grid.size()) {
            if (coord.x() >= 0 && coord.x() < m_grid[coord.y()].size()) {
                return &m_grid[coord.y()][coord.x()];
            }
        }

        return nullptr;
    }

    const QList<Planet*> planets();

protected:
    void childSectorUpdate();

Q_SIGNALS:
    void update();

private:
    Sector* findRandomFreeSector();
    QString UniquePlanetName(void);

protected:
    // A map is a 2-D array of Sectors.
    QList<QList<Sector> > m_grid;
    int  m_rows; // size of grid in sectors
    int  m_columns;
};

#endif // KONQUEST_MAP_H
