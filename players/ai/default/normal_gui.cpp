/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "normal_gui.h"
#include "normal.h"

#include <KLocalizedString>

AiDefaultNormalGui::AiDefaultNormalGui() :
    PlayerGui(i18n("Default (Offensive)"))
{
}


Player*
AiDefaultNormalGui::createInstance(Game *game, const QString &newName, const QColor &color) const
{
    return new AiDefaultNormal(game, newName, color);
}
