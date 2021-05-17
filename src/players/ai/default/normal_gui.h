/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AI_DEFAULT_NORMAL_GUI_H
#define AI_DEFAULT_NORMAL_GUI_H

#include "../../player_gui.h"


class AiDefaultNormalGui : public PlayerGui
{
public:
    explicit AiDefaultNormalGui();
    Player* createInstance(Game *game, const QString &newName, const QColor &color) const override;
};

#endif // AI_DEFAULT_NORMAL_GUI_H
