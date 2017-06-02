/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>

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

#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QStateMachine>
#include <QFinalState>
#include <QState>

#include <krandomsequence.h>

#include "fleet.h"
#include "players/player.h"
#include "players/neutralplayer.h"
#include "map/map.h"

class KLocalizedString;

struct GameOptions
{
   bool BlindMap, CumulativeProduction, ProductionAfterConquere;
   bool NeutralsShowShips, NeutralsShowStats;
   int  NeutralsProduction;
};

/**
  * This is the main class of the game.
  * It contains most of the game logic, ie. the finite state machine.
  */
class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = 0);

    bool isRunning();

    virtual void start() = 0;
    
    virtual void stop() = 0;

    bool attack( Planet *sourcePlanet, Planet *destPlanet, long long shipCount, bool standingOrder = false);

    static Coordinate generatePlanetCoordinates (int rows, int cols);
    static double generateKillPercentage();
    static int generatePlanetProduction();
    const QList<Planet *>  planets();

    Map *map() { return m_map; }

    Player *currentPlayer() { return m_currentPlayer; }

    const QList<Player *> players() { return m_players; }

    void findWinner();

    GameOptions &options() { return m_options; }

    NeutralPlayer *neutral() { return m_neutral; }

    void setPlayers (const QList<Player *> &players);

    int turnCounter() const { return m_turnCounter; }

    bool doFleetArrival (AttackFleet *fleet);
signals:
    void started();
    void finished();
    void waitingForPlayer(Player *player);
    void gameMsg( const KLocalizedString &msg, Player *player = 0,
                  Planet *planet = 0, Player *planetPlayer = 0 );

private:
    static KRandomSequence random;

    Player *m_currentPlayer;

    void makeKill(Fleet *fleet, Player *player);

public slots:

private slots:
    void newTurn() { m_turnCounter++; }

protected:
    // Points to the Map we're playing on.
    Map     *m_map;
    GameOptions m_options;

    virtual void buildMachine() = 0;
    QFinalState *m_finalState;
    QStateMachine m_gameMachine;
    NeutralPlayer *m_neutral;

    int m_turnCounter;

    QList<Player *> m_players;



    void setCurrentPlayer(Player *player);

    friend class Player;
};

#endif // GAME_H
