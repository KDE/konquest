#ifndef __PLAYER_H__
#define __PLAYER_H__

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
    static Player *createPlayer( QString newName, QColor newColor, int playerNum, bool isAi  );
    static Player *createNeutralPlayer();

    bool NewAttack( Planet *sourcePlanet, Planet *destPlanet, int shipCount, int departureTurn );

    bool isInPlay()               const { return m_inPlay; }
    void setInPlay( bool status )       { m_inPlay = status; }
    bool isAiPlayer()             const { return m_aiPlayer; }
    

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
    // Some fundamental properties.
    QString  m_name;
    QColor   m_color;
    int      m_playerNum;
    bool     m_inPlay;
    bool     m_aiPlayer;

    // Attack fleets sent by this player that are still moving
    AttackFleetList  m_attackList;

    // statistics counters
    int  m_shipsBuilt;
    int  m_planetsConquered;
    int  m_fleetsLaunched;
    int  m_enemyFleetsDestroyed;
    int  m_enemyShipsDestroyed;
};


typedef QList<Player *> PlayerList;


#endif
