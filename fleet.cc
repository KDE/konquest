/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "fleet.h"
#include "planet.h"

//---------------------------------------------------------------------------
// class Fleet
//   \---class AttackFleet
//    \---class DefenseFleet
//---------------------------------------------------------------------------


Fleet::Fleet( long long initialShipCount )
  : m_shipCount( initialShipCount )
{
}

void
Fleet::removeShips( long long lostShips )
{
    m_shipCount -= lostShips;
}

AttackFleet::AttackFleet( Planet *src, Planet *dest, long long initialCount, int arrival )
: Fleet( initialCount ), owner( src->player() ), source( src ), destination( dest ), arrivalTurn( arrival )
{
}

DefenseFleet::DefenseFleet( Planet *newHome, long long initialCount ) : Fleet( initialCount ), home( newHome )
{
}

void
DefenseFleet::absorb( AttackFleet *fleet )
{
    m_shipCount += fleet->shipCount();
}

void
DefenseFleet::become( AttackFleet *fleet )
{
    m_shipCount = fleet->shipCount();
}


AttackFleet *
DefenseFleet::spawnAttackFleet( Planet *dest, long long count, int arrivalTurn )
{
    if( m_shipCount < count ) {
        return nullptr;
    }

    AttackFleet *newFleet = new AttackFleet( home, dest, count, arrivalTurn  );

    removeShips( count );

    Q_EMIT update();
    
    return newFleet;
}

void
DefenseFleet::addShips( long long newShips )
{
    m_shipCount += newShips;

    if(m_shipCount < 0) /* to allow for negative production planets */
        m_shipCount = 0;
}

