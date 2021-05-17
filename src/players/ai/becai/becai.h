/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AI_BECAI_H
#define AI_BECAI_H

#include "../../computerplayer.h"


/**
 * This is quite a challenging AI.
 */

class AiBecai : public ComputerPlayer
{
public:
    explicit AiBecai(Game *game, const QString &newName, const QColor &color);

    void play() override;

private:
    int getMinimumDefenceFleetSize(Planet *planet, int minimumBaseDefenceFleetSize, double averageOwnKillPercentage, double averageOwnProduction);
};

#endif // AI_BECAI_H
