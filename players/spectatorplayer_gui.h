/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SPECTATOR_PLAYER_GUI_H
#define SPECTATOR_PLAYER_GUI_H

#include "player_gui.h"


class SpectatorPlayerGui : public PlayerGui
{
public:
    explicit SpectatorPlayerGui();
    Player* createInstance(Game *game, const QString &newName, const QColor &color) const override;
};

#endif // SPECTATOR_PLAYER_GUI_H
