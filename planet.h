#ifndef __PLANET_H__
#define __PLANET_H__

#include <QObject>
#include <QList>
#include "fleet.h"

class Sector;
class Player;
class Map;

//**************************************************************
// class Planet
//**************************************************************

class Planet : public QObject
{
    Q_OBJECT

private:

    Planet( QString planetName, Sector *sector,
            Player *initialOwner, int newProd,
            double newKillP );

public:
    // FIXME: Nobody inherits Planet.  Why virtual?  /iw
    virtual ~Planet();

    static Planet *createPlayerPlanet( Sector *sector,
                                       Player *initialOwner, QString planetName );
    static Planet *createNeutralPlanet( Sector *sector,
                                        Player *initialOwner, QString planetName );

    Sector        *sector()     const { return m_sector; }
    Player        *player()     const { return m_owner; }
    const QString &name()       const { return m_name; }
    DefenseFleet  &fleet()            { return m_homeFleet; }



    double         killPercentage() const { return m_killPercentage; }

    void           setKillPercentage( double newValue );
    int            production() const { return m_productionRate; }
    void           setProduction( int );

    void  select();
    void  conquer( AttackFleet *conqueringFleet );
    void  coup( Player *luckyPlayer );
    void  turn();

signals:
    void  update();
    void  selected();

private:
    QString       m_name;
    Player       *m_owner;
    Sector       *m_sector;
    DefenseFleet  m_homeFleet;

    double        m_killPercentage;
    int           m_productionRate;
};

#endif
