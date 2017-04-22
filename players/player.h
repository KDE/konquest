/*
    Copyright 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

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
    void setName(QString name) {  m_name = name; }

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

    AttackFleetList attackList() { return m_attackList; }
    AttackFleetList newAttacks() { return m_newAttacks; }
    AttackFleetList standingOrders() { return m_standingOrders; }
    void attackDone(AttackFleet *fleet);

    void addAttackFleet(AttackFleet *fleet);
    void addStandingOrder(AttackFleet *fleet);
    void cancelNewAttack(AttackFleet *fleet);
    void deleteStandingOrders(Planet *planet);

protected:
    virtual void play() = 0;
    virtual void onEntry (QEvent *event);
    virtual void onExit (QEvent *event);
    Game *m_game;

signals:
    void donePlaying();

public slots:

private slots:

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
