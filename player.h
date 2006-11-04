#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <QObject>
#include <QColor>
#include "planet.h"
#include "fleet.h"

//**************************************************************
// class Player
//**************************************************************

class Player
{
public:
    Player( QString newName, QColor color, int number, bool isAi );
    virtual ~Player();

    enum { NEUTRAL_PLAYER_NUMBER = -1 };
    
public:
    QString getName();
    QString getColoredName();
    QColor &getColor();
    bool isNeutral();
    AttackFleetList &getAttackList();

    // factory functions
    static Player *createPlayer( QString newName, QColor newColor, int playerNum, bool isAi  );
    static Player *createNeutralPlayer();

    bool NewAttack( Planet *sourcePlanet, Planet *destPlanet, int shipCount, int departureTurn );

    bool operator==( const Player &otherPlayer ) const;

    bool isInPlay();
    void setInPlay( bool );
    
private:
    QString name;
    QColor  color;
    int playerNum;
    bool inPlay;
    bool aiPlayer;

    AttackFleetList attackList;

    // statistics counters
    int shipsBuilt;
    int planetsConquered;
    int fleetsLaunched;
    int enemyFleetsDestroyed;
    int enemyShipsDestroyed;

public:
    void statShipsBuilt( int );
    void statPlanetsConquered( int );
    void statFleetsLaunched( int );
    void statEnemyFleetsDestroyed( int );
    void statEnemyShipsDestroyed( int );

    int getShipsBuilt() { return shipsBuilt; };
    int getPlanetsConquered() { return  planetsConquered; };
    int getFleetsLaunched() { return  fleetsLaunched; };
    int getEnemyFleetsDestroyed() { return  enemyFleetsDestroyed; };
    int getEnemyShipsDestroyed() { return  enemyShipsDestroyed; };
    bool isAiPlayer();

};

typedef QList<Player *> PlayerList;

#endif
