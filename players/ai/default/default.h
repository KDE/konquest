/*
    Copyright 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef AI_DEFAULT_H
#define AI_DEFAULT_H

#include "../../computerplayer.h"

class AiDefault : public ComputerPlayer
{
    Q_OBJECT

protected:
    enum AiLevel { Weak, Normal, Hard };

    explicit AiDefault(Game *game, const QString &newName, const QColor &color, AiLevel level);

    void setAiLevel (AiLevel level) { m_level = level; }

    AiLevel aiLevel() const { return m_level; }

    void play() Q_DECL_OVERRIDE;

signals:

public slots:

private:
    AiLevel m_level;
};

#endif // AI_DEFAULT_H
