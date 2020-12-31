/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef NEUTRALPLAYER_H
#define NEUTRALPLAYER_H

#include "player.h"


/**
 * The neutral player is the owner of neutral planets. From a game mechanics
 * point of view, a neutral player's turn starts a new game turn. Attacks and
 * production are handled here.
 */

class NeutralPlayer : public Player
{
public:
    explicit NeutralPlayer(Game *game);

    bool isDead() override;

    bool isNeutral() override;

protected:
    void play() override;
};

#endif // NEUTRALPLAYER_H
