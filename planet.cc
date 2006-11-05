#include "sector.h"
#include "planet.h"
#include "planet.moc"
#include "fleet.h"
#include "player.h"
#include "map.h"
#include "gamecore.h"

//---------------------------------------------------------------------------
// class Planet
//---------------------------------------------------------------------------

Planet::Planet( QString planetName, Sector *sector, Player *initialOwner,
                int newProd, double newKillP )
  : m_name(planetName),
    m_owner(initialOwner),
    m_sector(sector),
    m_homeFleet( this, newProd ),
    m_killPercentage(newKillP),
    m_productionRate(newProd)
{
    m_sector->setPlanet( this );
}

Planet::~Planet() {}


Planet *
Planet::createPlayerPlanet( Sector *sector, Player *initialOwner, 
			    QString planetName )
{
    return new Planet( planetName, sector, initialOwner, 10, 0.400 );
}


Planet *
Planet::createNeutralPlanet( Sector *sector, Player *initialOwner, 
			     QString planetName )
{
    CoreLogic  clogic;
    double     killP          = clogic.generateKillPercentage();
    int        productionRate = (int)clogic.generatePlanetProduction();

    return new Planet( planetName, sector,
                       initialOwner, productionRate, killP );
}


void
Planet::setKillPercentage( double newValue )
{
    m_killPercentage = newValue;

    emit update();
}

void
Planet::setProduction( int newProduction )
{
    m_productionRate = newProduction;
}


void
Planet::select()
{
    m_sector->select();

    emit selected();
}


void
Planet::conquer( AttackFleet *conqueringFleet )
{
    m_owner = conqueringFleet->owner;
    m_owner->statPlanetsConquered(1);
    m_homeFleet.become( conqueringFleet );
}

void
Planet::coup( Player *luckyPlayer )
{
    m_owner = luckyPlayer;
}

void
Planet::turn()
{
    if( !(m_owner->isNeutral()) ) {
        m_homeFleet.addShips( m_productionRate );
        m_owner->statShipsBuilt( m_productionRate );
    } else {
        m_homeFleet.addShips( 1 );
    }
}
