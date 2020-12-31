/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "localplayer.h"
#include <QDebug>

LocalPlayer::LocalPlayer(Game *game, const QString &newName, const QColor &color) :
    Player(game, newName, color)
{
}


void LocalPlayer::play()
{
    //qDebug() << name() << "::play";
    Q_EMIT canPlay();
}


void LocalPlayer::done()
{
    //qDebug() << name() << "::done";
    Q_EMIT donePlaying();
}
