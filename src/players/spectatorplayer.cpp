/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "spectatorplayer.h"

SpectatorPlayer::SpectatorPlayer(
        Game *game,
        const QString &newName,
        const QColor &color)
    : LocalPlayer(
        game,
        newName,
        color)
{
}


bool SpectatorPlayer::isDead()
{
    return false;
}


bool SpectatorPlayer::isSpectator()
{
    return true;
}
