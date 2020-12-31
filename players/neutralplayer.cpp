/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "neutralplayer.h"
#include "../game.h"
#include "../planet.h"
#include <QDebug>

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
    for (Player *player : m_game->players()) {
        player->resetTurnStats();

        for (AttackFleet *fleet : player->attackList()) {
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
    for (Planet *planet : m_game->map()->planets()) {
        //qDebug() << "Turn for planet " << planet->name();
        planet->turn(m_game->options());
    }

    m_game->findWinner();

    // After all that mess... It's done
    Q_EMIT donePlaying();
}
