/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright <inge@lysator.liu.se>
    Copyright <pinaraf@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef __PLAYER_H__
#define __PLAYER_H__


#include <QColor>

#include "planet.h"
#include "fleet.h"

class GameLogic;
class Map;

//**************************************************************
// class Player
//**************************************************************

class Player
{
public:
    Player( Map *map, const QString &newName, const QColor &color, int number );
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
    AttackFleetList &newAttacks() { return m_newAttacks; }

    // factory functions
    static Player *createPlayer( Map *map, const QString &name, 
				 const QColor &color, 
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
    // Fleets to send at the end of this turn
    AttackFleetList  m_newAttacks;

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
        AIPlayer( Map *map, const QString &name, const QColor &color, int number );
        virtual ~AIPlayer();
    
        virtual bool  isAiPlayer() const { return true; }
    
        void  doMove( GameLogic *gameLogic);
        
        int getAiLevel() { return m_AiLevel; }
    private:
        int m_AiLevel;
};


#endif
