/*
    Copyright Pierre Ducroquet <pinaraf@pinaraf.info>

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

#include "player.h"
#include <QDebug>
#include "../game.h"
#include "../planet.h"

Player::Player(Game *game, const QString &newName, const QColor &color) :
    QState(&(game->m_gameMachine)),
    m_game(game),
    m_name(newName),
    m_color(color),
    m_shipsBuilt(0),
    m_planetsConquered(0),
    m_fleetsLaunched(0),
    m_enemyFleetsDestroyed(0),
    m_enemyShipsDestroyed(0)
{
}

void Player::onEntry(QEvent *event)
{
    if (isNeutral())
        m_game->newTurn();

    qDebug() << "Entering state for player " << m_name;
    qDebug() << this->metaObject()->className();
    m_game->setCurrentPlayer(this);
    Q_UNUSED(event);
    if (isDead())
        emit donePlaying();
    else
        play();
}

void Player::onExit(QEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "Exiting state for player " << m_name;
    qDebug() << "We are moving our new attacks to our attacks";
    m_attackList += m_newAttacks;
    statFleetsLaunched(m_newAttacks.size());
    m_newAttacks.clear();
}

bool Player::isDead()
{
    if (m_attackList.count() > 0)
        return false;
    foreach (Planet *planet, m_game->planets())
        if (planet->player() == this)
            return false;
    return true;
}

bool Player::isAiPlayer()
{
    return true;
}

bool Player::isNeutral()
{
    return false;
}

QString Player::coloredName() const
{
    return QString("<font color=\"%1\">%2</font>").arg(m_color.name(), m_name);
}

void Player::attackDone(AttackFleet *fleet)
{
    m_attackList.removeAll(fleet);
}

void Player::addAttackFleet(AttackFleet *fleet)
{
    m_newAttacks << fleet;
}

void Player::cancelNewAttack(AttackFleet *fleet)
{
    if (!m_newAttacks.contains(fleet))
        return;
    fleet->source->fleet().absorb(fleet);
    m_newAttacks.removeAll(fleet);
    fleet->deleteLater();
}
