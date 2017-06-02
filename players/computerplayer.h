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

#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "player.h"


/**
 * This is the base class of all AI players.
 */

class ComputerPlayer : public Player
{
    Q_OBJECT

protected:
    explicit ComputerPlayer(Game *game, const QString &newName, const QColor &color);

    bool isAiPlayer() override;
    void play() override = 0;

signals:

public slots:

};

#endif // COMPUTERPLAYER_H
