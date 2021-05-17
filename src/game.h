/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QStateMachine>
#include <QFinalState>
#include <QState>
#include <QRandomGenerator>

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
    explicit Game(QObject *parent = nullptr);
    ~Game();

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
Q_SIGNALS:
    void started();
    void finished();
    void gameMsg( const KLocalizedString &msg, Player *player = nullptr,
                  Planet *planet = nullptr, Player *planetPlayer = nullptr );

private:
    static QRandomGenerator random;

    Player *m_currentPlayer;

    void makeKill(Fleet *fleet, Player *player);


private:
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
