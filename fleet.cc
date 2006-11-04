#include "fleet.h"
#include "fleet.moc"
#include "planet.h"
//---------------------------------------------------------------------------
// class Fleet
//   \---class AttackFleet
//    \---class DefenseFleet
//---------------------------------------------------------------------------


Fleet::Fleet( int initialShipCount )
: shipCount( initialShipCount )
{
}

int
Fleet::getShipCount()
{
    return shipCount;
}

void
Fleet::removeShips( int lostShips )
{
    shipCount -= lostShips;
}

AttackFleet::AttackFleet( Planet *source, Planet *dest, int initialCount, double arrival )
: Fleet( initialCount ), owner( source->getPlayer() ), destination( dest ), arrivalTurn( arrival ),
killPercentage( source->getKillPercentage() )
{
}

DefenseFleet::DefenseFleet( Planet *newHome, int initialCount ) : Fleet( initialCount ), home( newHome )
{
}

void
DefenseFleet::absorb( AttackFleet *fleet )
{
    shipCount += fleet->getShipCount();
}

void
DefenseFleet::become( AttackFleet *fleet )
{
    shipCount = fleet->getShipCount();
}


AttackFleet *
DefenseFleet::spawnAttackFleet( Planet *dest, int count, double arrivalTurn )
{
    if( shipCount < count ) {
        return NULL;
    }

    AttackFleet *newFleet = new AttackFleet( home, dest, count, arrivalTurn  );

    removeShips( count );

    return newFleet;
}

void
DefenseFleet::addShips( int newShips )
{
    shipCount += newShips;
}

