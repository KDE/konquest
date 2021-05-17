/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <QColor>
#include <QState>
#include "../fleet.h"

class Game;
class Planet;

class Player : public QState
{
    Q_OBJECT
public:
    explicit Player(Game *game, const QString &newName, const QColor &color);

    virtual bool isDead();

    virtual bool isNeutral();
    virtual bool isAiPlayer();
    virtual bool isSpectator();

    // Getters and Setters for some fundamental properties.
    QString name()        const { return m_name; }
    void setName(const QString &name) {  m_name = name; }

    QString coloredName() const;
    QColor& color()             { return m_color; }

    Game *game() const { return m_game; }

    // Statistics collection
    void statShipsBuilt( long long x )           { m_turnProduction += x; m_shipsBuilt += x; }
    void statPlanetsConquered( long long x )     { m_planetsConquered     += x; }
    void statFleetsLaunched( long long x )       { m_fleetsLaunched       += x; }
    void statEnemyFleetsDestroyed( long long x ) { m_enemyFleetsDestroyed += x; }
    void statEnemyShipsDestroyed( long long x )  { m_enemyShipsDestroyed  += x; }

    long long  shipsBuilt()           const { return m_shipsBuilt;           }
    long long  planetsConquered()     const { return m_planetsConquered;     }
    long long  fleetsLaunched()       const { return m_fleetsLaunched;       }
    long long  enemyFleetsDestroyed() const { return m_enemyFleetsDestroyed; }
    long long  enemyShipsDestroyed()  const { return m_enemyShipsDestroyed;  }

    void resetTurnStats();
    void statShipCount(long long x) { m_turnShips += x; }

    long long turnProduction() const { return m_turnProduction; }
    long long turnShips() const { return m_turnShips; }

    const AttackFleetList attackList() { return m_attackList; }
    const AttackFleetList newAttacks() { return m_newAttacks; }
    const AttackFleetList standingOrders() { return m_standingOrders; }
    void attackDone(AttackFleet *fleet);

    void addAttackFleet(AttackFleet *fleet);
    void addStandingOrder(AttackFleet *fleet);
    void cancelNewAttack(AttackFleet *fleet);
    void deleteStandingOrders(Planet *planet);

protected:
    virtual void play() = 0;
    void onEntry (QEvent *event) override;
    void onExit (QEvent *event) override;
    Game *m_game;

Q_SIGNALS:
    void donePlaying();

private:
    // Attack fleets sent by this player that are still moving
    AttackFleetList m_attackList;
    // Fleets to send at the end of this turn
    AttackFleetList m_newAttacks;
    AttackFleetList m_standingOrders;

    // Some fundamental properties.
    QString  m_name;
    QColor   m_color;

    // statistics counters
    long long  m_shipsBuilt;
    long long  m_planetsConquered;
    long long  m_fleetsLaunched;
    long long  m_enemyFleetsDestroyed;
    long long  m_enemyShipsDestroyed;

    long long m_turnProduction; ///< number of ships produced in this turn
    long long m_turnShips; ///< number of all available player ships in this turn

    /**
     * @todo This is a bad GUI hack. The game selection grid is handled by just
     * a list of player instances. This property stores the GUI name of the class
     * used to create this player. For example a player of "AiDefaultWeak" class
     * will be created via "AiDefaultWeakGui" with the GUI name of "Default Weak".
     * That GUI name is stored here.
     */

    QString m_guiName;

public:
    void setGuiName(const QString &guiName) {
        m_guiName = guiName;
    }

    QString guiName() const {
        return m_guiName;
    }
};

#endif // PLAYER_H
