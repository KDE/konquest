#ifndef __FLEET_H__
#define __FLEET_H__

#include <QObject>
#include <Q3PtrList>

class Player;
class Planet;

//**********************************************************
// class Fleet
// \--- class AttackFleet
//  \--- class DefenseFleet
//**********************************************************

class Fleet : public QObject
{
    Q_OBJECT
        
public:

    Fleet( int initialShipCount );
    virtual ~Fleet() {};

    int getShipCount();
    void removeShips( int lostShips );

protected:
    int shipCount;
};

class AttackFleet : public Fleet
{

public:
    AttackFleet( Planet *source, Planet *dest, int initialCount, double arrivalTurn  );

    Player *owner;
    Planet *destination;
    double arrivalTurn;
    double killPercentage;

};


class DefenseFleet : public Fleet
{
        
public:
    DefenseFleet( Planet *newHome, int initialCount  );

    void absorb( AttackFleet *fleet );
    void become( AttackFleet *fleet );

    void addShips( int newShips );
    
    AttackFleet *spawnAttackFleet( Planet *destination, int shipCount, double arrivalTurn );

    Planet *home;

};

typedef Q3PtrList<AttackFleet> AttackFleetList;
typedef Q3PtrListIterator<AttackFleet> AttackFleetListIterator;

#endif
