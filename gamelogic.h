/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright Inge Wallin <inge@lysator.liu.se>
    Copyright Pierre Ducroquet <pinaraf@gmail.com>

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
#ifndef KONQUEST_GAMELOGIC_H
#define KONQUEST_GAMELOGIC_H

struct GameOptions
{
   bool BlindMap, CumulativeProduction, ProductionAfterConquere;
   bool NeutralsShowShips, NeutralsShowStats;
   int  NeutralsProduction;
};


#include "planet.h"
#include "player.h"


class KLocalizedString;

class Player;


//************************************************************************
// GameLogic Object
//************************************************************************

class GameLogic : public QObject
{
    Q_OBJECT

public:
    explicit  GameLogic( QObject *parent );
    virtual  ~GameLogic();

    // Getters about the game state
    int               turnNumber()    const { return m_turnNumber; }
    Map              *map()           const { return m_map; }
    GameOptions      &options()             { return m_options; }
    QList<Player *>  *players()             { return &m_players; }
    Player           *currentPlayer() const
    {
        if(m_blindbreak)
            return NULL;
        return *m_currentPlayer;
    }
    void              setBlindBreak(bool value) { m_blindbreak = value; }

    QList<Planet *>  planets();

    int               humanPlayerCount();

    Player           *findWinner();
    void              resolveShipsInFlight();
    void              scanForSurvivors();

    void              startNewGame();
    void              nextTurn();
    void              nextPlayer();

    void              cleanupGame();

signals:
    void              beginTurn ();
    void              endTurn ();
    void              gameOver(Player *winner);
    void              gameMsg( const KLocalizedString &msg, Player *player = 0,
			       Planet *planet = 0, Player *planetPlayer = 0 );

private:

    // helper functions
    void              doFleetArrival( AttackFleet *arrivingFleet );
    void              MakeKill(Fleet &, Player &);
    
private:

    //***************************************************************
    // Game state
    //***************************************************************

    Map                       *m_map;
    GameOptions                m_options;
    QList<Player *>            m_players;
    Player                    *m_neutralPlayer;
    bool                       m_blindbreak;

    // Loop variables for the game play.
    int                        m_turnNumber;
    QList<Player *>::Iterator  m_currentPlayer;
};


#endif  // KONQUEST_GAMELOGIC_H
