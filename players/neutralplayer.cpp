/*
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

#include "neutralplayer.h"
#include "../game.h"
#include "../planet.h"
#include <QColor>
#include <QDebug>
#include <QApplication>

NeutralPlayer::NeutralPlayer(Game *game) :
    Player(game, QString(), Qt::gray)
{
}

bool NeutralPlayer::isNeutral()
{
    return true;
}

bool NeutralPlayer::isDead()
{
    return false;
}

void NeutralPlayer::play()
{
    //qDebug() << "NeutralPlayer::play";

    // Go on each attack...
    foreach (Player *player, m_game->players()) {
        player->resetTurnStats();

        foreach (AttackFleet *fleet, player->attackList()) {
            if (m_game->doFleetArrival(fleet)) {
                player->attackDone(fleet);
                fleet->deleteLater();
            }
            else {

                // Only add the number of ships of the fleet to the player's
                // total fleet size if the fleet does not arrive this turn.

                player->statShipCount(fleet->shipCount());
            }
        }
    }

    // Go over each planet, adding its ships
    foreach (Planet *planet, m_game->map()->planets()) {
        //qDebug() << "Turn for planet " << planet->name();
        planet->turn(m_game->options());
    }

    m_game->findWinner();

    // After all that mess... It's done
    emit(donePlaying());
}
