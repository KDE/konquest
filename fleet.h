#ifndef __FLEET_H__
#define __FLEET_H__

#include <QObject>

class Player;
class Planet;

//**********************************************************
// class Fleet
// +--- class AttackFleet
// +--- class DefenseFleet
//**********************************************************

class Fleet : public QObject
{
    Q_OBJECT
        
public:

    explicit Fleet( int initialShipCount );
    virtual ~Fleet() {}

    int   shipCount()   const { return m_shipCount; }
    void  removeShips( int lostShips );

protected:
    int m_shipCount;
};


class AttackFleet : public Fleet
{
    Q_OBJECT
public:
    AttackFleet( Planet *source, Planet *dest, int initialCount, double arrivalTurn  );

    Player  *owner;
    Planet  *destination;
    double   arrivalTurn;
    double   killPercentage;
};


class DefenseFleet : public Fleet
{
    Q_OBJECT
        
public:
    DefenseFleet( Planet *newHome, int initialCount  );

    void absorb( AttackFleet *fleet );
    void become( AttackFleet *fleet );

    void addShips( int newShips );
    
    AttackFleet *spawnAttackFleet( Planet *destination, int shipCount, double arrivalTurn );

    Planet *home;
signals:
    void update();
};

typedef QList<AttackFleet *> AttackFleetList;

#endif
