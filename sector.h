#ifndef __SECTOR_H__
#define __SECTOR_H__

#include <QObject>
#include "gamecore.h"

// ***************************************************************
// class Sector
// ***************************************************************

class Sector : public QObject
{
    Q_OBJECT

public:

    // constructors
    Sector();
    Sector( Map *parentMap, Coordinate c );
    Sector( const Sector & );

    // assignment operator (makes initialization easy)
    Sector &operator=( const Sector & );

    Coordinate  coord()     const { return m_coord; }
    bool        hasPlanet() const { return m_planet != NULL; }
    Planet     *planet()    const { return m_planet; }
    void        setPlanet( Planet *planet );
    void        removePlanet();

    void        select();


signals:
    void update();
    void selected();

protected slots:
    void childPlanetUpdate( );

    
protected:
    Map        *m_map;
    Coordinate  m_coord;
    Planet     *m_planet;  // a sector has 0 or 1 planets
};


#endif
