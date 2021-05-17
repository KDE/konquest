/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "computerplayer.h"
#include "../game.h"

#include <cmath>
#include <cfloat>

ComputerPlayer::ComputerPlayer(Game *game,
                               const QString &newName,
                               const QColor &color)
    : Player(game, newName, color)
{
}


bool ComputerPlayer::isAiPlayer()
{
    return true;
}
