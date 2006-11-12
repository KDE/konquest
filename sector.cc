#include "sector.h"
#include "sector.moc"
#include "map.h"
#include <QtDebug>

//---------------------------------------------------------------------------
// class Sector
//---------------------------------------------------------------------------

Sector::Sector()
  : m_map(NULL ),
    m_coord( 0,0 ),
    m_planet( NULL )
{}

Sector::Sector( Map *map, Coordinate coord )
  : m_map( map ),
    m_coord( coord ),
    m_planet(NULL)
{
}

Sector::Sector( const Sector & other )
  : QObject( 0 ), 
    m_map(other.m_map), 
    m_coord(other.m_coord),
    m_planet(other.m_planet)
{
}


void Sector::setPlanet( Planet *planet )
{
    m_planet = planet;

    connect( m_planet, SIGNAL( update() ), this, SLOT( childPlanetUpdate() ) );

    emit update();
}

void Sector::removePlanet()
{
    m_planet = NULL;

    emit update();
}


void Sector::childPlanetUpdate()
{
    emit update();
}

Sector &
Sector::operator=( const Sector &other )
{
    m_coord  = other.m_coord;
    m_planet = other.m_planet;
    m_map    = other.m_map;

    return *this;
}

void
Sector::select()
{
    m_map->setSelectedSector( m_coord );

    emit selected();
}
