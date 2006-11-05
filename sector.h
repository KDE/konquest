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

    bool hasPlanet() const;
    void setPlanet( Planet *newPlanet );
    Planet *getPlanet();
    void removePlanet();

    void select();

    Coordinate getCoord();

signals:
    void update();
    void selected();

protected slots:
    void childPlanetUpdate( );

    
protected:
    Map        *parentMap;
    Coordinate  c;
    Planet     *planet;  // a sector has 0 or 1 planets
};


#endif
