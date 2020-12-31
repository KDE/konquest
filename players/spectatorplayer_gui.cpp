/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "spectatorplayer_gui.h"
#include "spectatorplayer.h"

#include <KLocalizedString>

SpectatorPlayerGui::SpectatorPlayerGui() :
    PlayerGui(i18nc("A human spectator", "Human (Spectator)"))
{
}


Player*
SpectatorPlayerGui::createInstance(Game *game, const QString &newName, const QColor &color) const
{
    return new SpectatorPlayer(game, newName, color);
}
