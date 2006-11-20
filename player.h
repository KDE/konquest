#ifndef __PLAYER_H__
#define __PLAYER_H__


#include <QColor>

#include "planet.h"
#include "fleet.h"

class GameLogic;


//**************************************************************
// class Player
//**************************************************************

class Player
{
public:
    Player( Map *map, const QString &newName, QColor color, int number );
    virtual ~Player();

    bool operator==( const Player &otherPlayer ) const
        { return m_playerNum == otherPlayer.m_playerNum; }

    enum { NEUTRAL_PLAYER_NUMBER = -1 };
    
    // Getters for some fundamental properties.
    QString name()        const { return m_name; }
    QString coloredName() const;
    QColor& color()             { return m_color; }
    bool    isNeutral()   const { return m_playerNum == NEUTRAL_PLAYER_NUMBER; }
    AttackFleetList &attackList() { return m_attackList; }

    // factory functions
    static Player *createPlayer( Map *map, const QString &name, 
				 QColor color, 
				 int playerNum, bool isAi  );
    static Player *createNeutralPlayer( Map *map );

    bool NewAttack( Planet *sourcePlanet, Planet *destPlanet, int shipCount, int departureTurn );

    bool isInPlay()               const { return m_inPlay; }
    void setInPlay( bool status )       { m_inPlay = status; }
    virtual bool  isAiPlayer()    const { return false; }
    

    // Statistics collection
    void statShipsBuilt( int x )           { m_shipsBuilt           += x; }
    void statPlanetsConquered( int x )     { m_planetsConquered     += x; }
    void statFleetsLaunched( int x )       { m_fleetsLaunched       += x; }
    void statEnemyFleetsDestroyed( int x ) { m_enemyFleetsDestroyed += x; }
    void statEnemyShipsDestroyed( int x )  { m_enemyShipsDestroyed  += x; }

    int  shipsBuilt()           const { return m_shipsBuilt;           }
    int  planetsConquered()     const { return m_planetsConquered;     }
    int  fleetsLaunched()       const { return m_fleetsLaunched;       }
    int  enemyFleetsDestroyed() const { return m_enemyFleetsDestroyed; }
    int  enemyShipsDestroyed()  const { return m_enemyShipsDestroyed;  }

private:
    // Points to the Map we're playing on.
    Map     *m_map;

    // Some fundamental properties.
    QString  m_name;
    QColor   m_color;
    int      m_playerNum;
    bool     m_inPlay;

    // Attack fleets sent by this player that are still moving
    AttackFleetList  m_attackList;

    // statistics counters
    int  m_shipsBuilt;
    int  m_planetsConquered;
    int  m_fleetsLaunched;
    int  m_enemyFleetsDestroyed;
    int  m_enemyShipsDestroyed;
};


class AIPlayer : public Player
{
public:
    AIPlayer( Map *map, const QString &name, QColor color, int number );
    virtual ~AIPlayer();

    virtual bool  isAiPlayer() const { return true; }

    void  doMove( GameLogic *gameLogic);
};


#endif
