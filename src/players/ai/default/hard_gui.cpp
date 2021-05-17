/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "hard_gui.h"
#include "hard.h"

#include <KLocalizedString>

AiDefaultHardGui::AiDefaultHardGui() :
    PlayerGui(i18n("Default (Defensive)"))
{
}


Player*
AiDefaultHardGui::createInstance(Game *game, const QString &newName, const QColor &color) const
{
    return new AiDefaultHard(game, newName, color);
}
