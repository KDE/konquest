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
#ifndef __PLANET_H__
#define __PLANET_H__

#include <QObject>
#include "fleet.h"

class Sector;
class Player;

//**************************************************************
// class Planet
//**************************************************************

class Planet : public QObject
{
    Q_OBJECT

private:

    Planet( const QString &planetName, Sector *sector,
            Player *initialOwner, int newProd,
            double newKillP );

public:
    // FIXME: Nobody inherits Planet.  Why virtual?  /iw
    virtual ~Planet();

    static Planet *createPlayerPlanet( Sector *sector,
                                       Player *initialOwner, const QString &planetName );
    static Planet *createNeutralPlanet( Sector *sector,
                                        Player *initialOwner, const QString &planetName );

    Sector        *sector()     const { return m_sector; }
    Player        *player()     const { return m_owner; }
    const QString &name()       const { return m_name; }
    DefenseFleet  &fleet()            { return m_homeFleet; }



    double         killPercentage() const { return m_killPercentage; }

    int            production() const { return m_productionRate; }
    int            planetLook() const { return m_planetLook; }
    int            ships()      const { return m_showCurShips ? m_homeFleet.shipCount() : m_oldShips; }

    void  showOldShips() { m_showCurShips=false; }
    void  select();
    void  conquer( AttackFleet *conqueringFleet );
    void  turn();

signals:
    void  update();
    void  selected();

private:
    QString       m_name;
    Player       *m_owner;
    Sector       *m_sector;
    DefenseFleet  m_homeFleet;

    double        m_killPercentage;
    int           m_productionRate;
    int           m_planetLook;
    int           m_oldShips;
    bool          m_showCurShips;
};

#endif
