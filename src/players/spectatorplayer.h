/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SPECTATORPLAYER_H
#define SPECTATORPLAYER_H

#include "localplayer.h"


/**
 * a spectator player controller
 *
 * A spectator is very much like a local human player, except that it has no
 * planets. It cannot die and it does not count as active player. Its main
 * purpose is to watch the game, for example watch AIs play against each other.
 */

class SpectatorPlayer : public LocalPlayer
{
public:
    explicit SpectatorPlayer(Game *game, const QString &newName, const QColor &color);

    bool isDead() override;
    bool isSpectator() override;
};

#endif // SPECTATORPLAYER_H
