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
    Q_OBJECT
        
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
    Q_OBJECT
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
signals:
    void update();
};

typedef QList<AttackFleet *> AttackFleetList;

#endif // KONQUEST_FLEET_H
