#include "sector.h"
#include "sector.moc"
#include "map.h"

//---------------------------------------------------------------------------
// class Sector
//---------------------------------------------------------------------------

Sector::Sector()
: planet( NULL ), parentMap(NULL ), c( 0,0 )
{}

Sector::Sector( Map *newParentMap, Coordinate _c )
: planet(NULL), parentMap( newParentMap ), c(_c)
{
}

Sector::Sector( const Sector & other )
: QObject( 0 ), planet(other.planet), parentMap(other.parentMap), c(other.c)
{
}

bool Sector::hasPlanet() const
{
    return (planet != NULL);
}


void Sector::setPlanet( Planet *newPlanet )
{
    planet = newPlanet;

    connect( planet, SIGNAL( update() ), this, SLOT( childPlanetUpdate() ) );

    emit update();
}

Planet *Sector::getPlanet()
{
    return planet;
}

void Sector::removePlanet()
{
    planet = NULL;

    emit update();
}


void Sector::childPlanetUpdate()
{
    emit update();
}

Sector &
Sector::operator=( const Sector &other )
{
    c = other.c;
    planet = other.planet;
    parentMap = other.parentMap;

    return *this;
}

void
Sector::select()
{
    parentMap->setSelectedSector( c );
    emit selected();
}

Coordinate Sector::getCoord()
{
    return c;
}
