#ifndef __PLANET_H__
#define __PLANET_H__

#include <QObject>
#include <QList>
#include "fleet.h"

class Player;
class Sector;
class Map;

//**************************************************************
// class Planet
//**************************************************************

class Planet : public QObject
{
    Q_OBJECT

private:

    Planet( QString planetName, Sector &newParentSector,
            Player *initialOwner, int newProd,
            double newKillP, double newMorale );

public:
    virtual ~Planet();

    static Planet *createPlayerPlanet( Sector &parentSector,
                                       Player *initialOwner, QString planetName );
    static Planet *createNeutralPlanet( Sector &parentSector,
                                        Player *initialOwner, QString planetName );

    Sector &getSector() const;
    Player *getPlayer() const;
    const QString &getName() const;
    DefenseFleet &getFleet();

    double getKillPercentage();
    void setKillPercentage( double newValue );
    double getMorale();
    void setMorale( double );
    int getProduction();
    void setProduction( int );

    void select();
    void conquer(  AttackFleet *conqueringFleet );
    void coup( Player *luckyPlayer );
    void turn();

signals:
    void update();
    void selected();

private:
    QString name;
    Player *owner;
    Sector &parentSector;
    DefenseFleet homeFleet;

    double killPercentage;
    double morale;
    int productionRate;
};

typedef QList<Planet *> PlanetList;

#endif
