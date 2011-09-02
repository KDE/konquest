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
#ifndef KONQUEST_PLAYER_H
#define KONQUEST_PLAYER_H


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
    enum AiLevel { Human, ComputerWeak, ComputerNormal, ComputerHard };

    Player( Map *map, const QString &newName, const QColor &color, int number,
            enum AiLevel level);
    virtual ~Player();

    bool operator==( const Player &otherPlayer ) const
        { return m_playerNum == otherPlayer.m_playerNum; }

    enum { NEUTRAL_PLAYER_NUMBER = -1 };
    
    // Getters and Setters for some fundamental properties.
    QString name()        const { return m_name; }
    void setName(QString name) {  m_name = name; }
    int playerNum() const { return m_playerNum; }

    QString coloredName() const;
    QColor& color()             { return m_color; }
    bool    isNeutral()   const { return m_playerNum == NEUTRAL_PLAYER_NUMBER; }
    AttackFleetList &attackList() { return m_attackList; }
    AttackFleetList &newAttacks() { return m_newAttacks; }

    static Player *createNeutralPlayer( Map *map );

    bool NewAttack( Planet *sourcePlanet, Planet *destPlanet, int shipCount, int departureTurn );

    bool isInPlay()               const { return m_inPlay; }
    void setInPlay( bool status )       { m_inPlay = status; }
    bool  isAiPlayer()    const { return m_AiLevel; }
    enum AiLevel  getAiLevel() { return m_AiLevel; }
    void  setAiLevel(enum AiLevel level) { m_AiLevel = level; }
    void  doAiMove( GameLogic *gameLogic);

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

    // sorting predicates
    static bool sortPointerByPlayerNum(const Player* a, const Player* b) { return a->m_playerNum < b->m_playerNum; }

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

    enum AiLevel  m_AiLevel;
};

#endif // KONQUEST_PLAYER_H
