/*
    Copyright 2013 Alexander Schuch <aschuch247@gmail.com>

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
    Q_OBJECT

public:
    explicit SpectatorPlayer(Game *game, const QString &newName, const QColor &color);

    bool isDead() override;
    bool isSpectator() override;

signals:

public slots:

};

#endif // SPECTATORPLAYER_H
