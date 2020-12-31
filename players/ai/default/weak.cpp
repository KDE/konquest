/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "weak.h"

AiDefaultWeak::AiDefaultWeak(
        Game *game,
        const QString &newName,
        const QColor &color)
    : AiDefault(
        game,
        newName,
        color,
        AiDefault::Weak)
{
}
