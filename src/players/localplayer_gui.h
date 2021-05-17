/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LOCAL_PLAYER_GUI_H
#define LOCAL_PLAYER_GUI_H

#include "player_gui.h"


class LocalPlayerGui : public PlayerGui
{
public:
    explicit LocalPlayerGui();
    Player* createInstance(Game *game, const QString &newName, const QColor &color) const override;
};

#endif // LOCAL_PLAYER_GUI_H
