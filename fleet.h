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
#ifndef __FLEET_H__
#define __FLEET_H__

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

    explicit Fleet( int initialShipCount );
    virtual ~Fleet() {}

    int   shipCount()   const { return m_shipCount; }
    void  removeShips( int lostShips );

protected:
    int m_shipCount;
};


class AttackFleet : public Fleet
{
    Q_OBJECT
public:
    AttackFleet( Planet *source, Planet *dest, int initialCount, double arrivalTurn  );

    Player  *owner;
    Planet  *destination;
    double   arrivalTurn;
    double   killPercentage;
};


class DefenseFleet : public Fleet
{
    Q_OBJECT
        
public:
    DefenseFleet( Planet *newHome, int initialCount  );

    void absorb( AttackFleet *fleet );
    void become( AttackFleet *fleet );

    void addShips( int newShips );
    
    AttackFleet *spawnAttackFleet( Planet *destination, int shipCount, double arrivalTurn );

    Planet *home;
signals:
    void update();
};

typedef QList<AttackFleet *> AttackFleetList;

#endif
