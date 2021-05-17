/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AI_EXAMPLE_H
#define AI_EXAMPLE_H

#include "../../computerplayer.h"


/**
 * This is a passive example AI player.
 *
 * The classes can be used as base for own AI implementations. This
 * implementation does nothing. In other words, this AI player does not expand
 * its empire and just stacks its production for a massive defence of its
 * home world.
 */

class AiExample : public ComputerPlayer
{
public:
    explicit AiExample(Game *game, const QString &newName, const QColor &color);

    void play() override;

};

#endif // AI_EXAMPLE_H
