/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_PLANET_H
#define KONQUEST_PLANET_H

#include <QObject>
#include "fleet.h"

class Sector;
class Player;
struct GameOptions;

//**************************************************************
// class Planet
//**************************************************************

class Planet : public QObject
{
    Q_OBJECT

public:
    Planet( const QString &planetName, Sector *sector,
            Player *initialOwner, int newProd,
            double newKillP );

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
    void           setKillPercentage(double value) { m_killPercentage = value; }

    int            production() const { return m_productionRate; }
    void           setProduction(int value)
    {
       m_originalProductionRate = m_productionRate = value;
    }

    int            planetLook() const { return m_planetLook; }
    int            ships()      const { return m_showCurShips ? m_homeFleet.shipCount() : m_oldShips; }

    void  showOldShips() { m_showCurShips=false; }
    void  conquer( AttackFleet *conqueringFleet );
    void  setOwner(Player* player) { m_owner = player; }
    void  turn(const GameOptions &);

    //void select() { Q_ASSERT(false); }
Q_SIGNALS:
    void  update();

private:
    QString       m_name;
    Player       *m_owner;
    Sector       *m_sector;
    DefenseFleet  m_homeFleet;

    double        m_killPercentage;
    int           m_productionRate, m_originalProductionRate;
    int           m_planetLook;
    int           m_oldShips;
    bool          m_showCurShips;
    bool          m_justconquered;
};

#endif // KONQUEST_PLANET_H
