/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "player.h"


/**
 * This is the base class of all AI players.
 */

class ComputerPlayer : public Player
{
protected:
    explicit ComputerPlayer(Game *game, const QString &newName, const QColor &color);

    bool isAiPlayer() override;
    void play() override = 0;
};

#endif // COMPUTERPLAYER_H
