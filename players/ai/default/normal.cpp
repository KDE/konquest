/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "normal.h"

AiDefaultNormal::AiDefaultNormal(
        Game *game,
        const QString &newName,
        const QColor &color)
    : AiDefault(
        game,
        newName,
        color,
        AiDefault::Normal)
{
}
