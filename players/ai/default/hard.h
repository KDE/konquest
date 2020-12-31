/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AI_DEFAULT_HARD_H
#define AI_DEFAULT_HARD_H

#include "default.h"

class AiDefaultHard : public AiDefault
{
public:
    explicit AiDefaultHard(Game *game, const QString &newName, const QColor &color);

};

#endif // AI_DEFAULT_HARD_H
