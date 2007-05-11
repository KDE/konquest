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

#include "fleet.h"
#include "fleet.moc"
#include "planet.h"

//---------------------------------------------------------------------------
// class Fleet
//   \---class AttackFleet
//    \---class DefenseFleet
//---------------------------------------------------------------------------


Fleet::Fleet( int initialShipCount )
  : m_shipCount( initialShipCount )
{
}

void
Fleet::removeShips( int lostShips )
{
    m_shipCount -= lostShips;
}

AttackFleet::AttackFleet( Planet *source, Planet *dest, int initialCount, double arrival )
: Fleet( initialCount ), owner( source->player() ), destination( dest ), arrivalTurn( arrival ),
killPercentage( source->killPercentage() )
{
}

DefenseFleet::DefenseFleet( Planet *newHome, int initialCount ) : Fleet( initialCount ), home( newHome )
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
DefenseFleet::spawnAttackFleet( Planet *dest, int count, double arrivalTurn )
{
    if( m_shipCount < count ) {
        return NULL;
    }

    AttackFleet *newFleet = new AttackFleet( home, dest, count, arrivalTurn  );

    removeShips( count );

    emit update();
    
    return newFleet;
}

void
DefenseFleet::addShips( int newShips )
{
    m_shipCount += newShips;
}

