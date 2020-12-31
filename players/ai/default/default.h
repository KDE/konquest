/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AI_DEFAULT_H
#define AI_DEFAULT_H

#include "../../computerplayer.h"

class AiDefault : public ComputerPlayer
{
protected:
    enum AiLevel { Weak, Normal, Hard };

    explicit AiDefault(Game *game, const QString &newName, const QColor &color, AiLevel level);

    void setAiLevel (AiLevel level) { m_level = level; }

    AiLevel aiLevel() const { return m_level; }

    void play() override;

private:
    AiLevel m_level;
};

#endif // AI_DEFAULT_H
