/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "localplayer_gui.h"
#include "localplayer.h"

#include <KLocalizedString>

LocalPlayerGui::LocalPlayerGui() :
    PlayerGui(i18nc("A human player", "Human (Player)"))
{
}


Player*
LocalPlayerGui::createInstance(Game *game, const QString &newName, const QColor &color) const
{
    return new LocalPlayer(game, newName, color);
}
