#include "planet.h"
#include "planet.moc"
#include "fleet.h"
#include "player.h"
#include "gamecore.h"

//---------------------------------------------------------------------------
// class Planet
//---------------------------------------------------------------------------

Planet::Planet( QString planetName, Sector &newParentSector, Player *initialOwner,
                int newProd, double newKillP, double newMorale )
       : name(planetName), owner(initialOwner), parentSector(newParentSector),
         homeFleet( this, newProd ), killPercentage(newKillP), morale( newMorale ), productionRate(newProd)


{
    parentSector.setPlanet( this );
}

Planet::~Planet() {}

Planet *
Planet::createPlayerPlanet( Sector &parentSector, Player *initialOwner, QString planetName )
{
    CoreLogic clogic;

    double morale = clogic.generateMorale();

    return new Planet( planetName, parentSector, initialOwner,
                       10, 0.400, morale );
}

Planet *
Planet::createNeutralPlanet( Sector &parentSector, Player *initialOwner, QString planetName )
{
    CoreLogic clogic;
    double morale = clogic.generateMorale();

    double killP = clogic.generateKillPercentage();

    int productionRate = (int)clogic.generatePlanetProduction();

    return new Planet( planetName, parentSector,
                       initialOwner, productionRate, killP, morale );
}

double
Planet::getKillPercentage()
{
    return killPercentage;
}

void
Planet::setKillPercentage( double newValue )
{
    killPercentage = newValue;

    emit update();
}

double
Planet::getMorale()
{
    return morale;
}

void
Planet::setMorale( double newMorale )
{
    morale = newMorale;
}

int
Planet::getProduction()
{
    return productionRate;
}

void
Planet::setProduction( int newProduction )
{
    productionRate = newProduction;
}

void
Planet::select()
{
    parentSector.select();

    emit selected();
}

DefenseFleet &Planet::getFleet()
{
    return homeFleet;
}

Player *
Planet::getPlayer() const
{
    return owner;
}

const QString &
Planet::getName() const
{
    return name;
}

Sector &
Planet::getSector() const
{
    return parentSector;
}

void
Planet::conquer( AttackFleet *conqueringFleet )
{
    owner = conqueringFleet->owner;
    owner->statPlanetsConquered(1);
    homeFleet.become( conqueringFleet );
}

void
Planet::coup( Player *luckyPlayer )
{
    owner = luckyPlayer;
}

void
Planet::turn()
{
    if( !(owner->isNeutral()) ) {
        homeFleet.addShips( productionRate );
        owner->statShipsBuilt( productionRate );
    } else {
        homeFleet.addShips( 1 );
    }
}
