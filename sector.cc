#include "sector.h"
#include "sector.moc"
#include "map.h"

//---------------------------------------------------------------------------
// class Sector
//---------------------------------------------------------------------------

Sector::Sector()
  : m_planet( NULL ), 
    m_map(NULL ), 
    m_coord( 0,0 )
{}

Sector::Sector( Map *map, Coordinate coord )
  : m_planet(NULL),
    m_map( map ),
    m_coord( coord )
{
}

Sector::Sector( const Sector & other )
  : QObject( 0 ), 
    m_planet(other.m_planet),
    m_map(other.m_map), 
    m_coord(other.m_coord)
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
