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

    return newFleet;
}

void
DefenseFleet::addShips( int newShips )
{
    m_shipCount += newShips;
}

