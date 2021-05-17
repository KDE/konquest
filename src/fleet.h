/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_FLEET_H
#define KONQUEST_FLEET_H

#include <QObject>

class Player;
class Planet;

//**********************************************************
// class Fleet
// +--- class AttackFleet
// +--- class DefenseFleet
//**********************************************************

class Fleet : public QObject
{
public:

    explicit Fleet( long long initialShipCount );
    virtual ~Fleet() {}

    long long   shipCount()   const { return m_shipCount; }
    void  removeShips( long long lostShips );

protected:
    long long m_shipCount;
};


class AttackFleet : public Fleet
{
public:
    AttackFleet( Planet *src, Planet *dest, long long initialCount, int arrivalTurn );

    Player  *owner;
    Planet  *source;
    Planet  *destination;
    int      arrivalTurn;
};


class DefenseFleet : public Fleet
{
    Q_OBJECT

public:
    DefenseFleet( Planet *newHome, long long initialCount  );

    void absorb( AttackFleet *fleet );
    void become( AttackFleet *fleet );

    void addShips( long long newShips );
    
    AttackFleet *spawnAttackFleet( Planet *destination, long long shipCount, int arrivalTurn );

    Planet *home;
Q_SIGNALS:
    void update();
};

typedef QList<AttackFleet *> AttackFleetList;

#endif // KONQUEST_FLEET_H
