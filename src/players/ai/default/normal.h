/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AI_DEFAULT_NORMAL_H
#define AI_DEFAULT_NORMAL_H

#include "default.h"

class AiDefaultNormal : public AiDefault
{
public:
    explicit AiDefaultNormal(Game *game, const QString &newName, const QColor &color);
};

#endif // AI_DEFAULT_NORMAL_H
