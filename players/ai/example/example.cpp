/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "example.h"

AiExample::AiExample(
        Game *game,
        const QString &newName,
        const QColor &color)
    : ComputerPlayer(
        game,
        newName,
        color)
{
}


void
AiExample::play()
{
    Q_EMIT donePlaying();
}
