/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "weak_gui.h"
#include "weak.h"

#include <KLocalizedString>

AiDefaultWeakGui::AiDefaultWeakGui() :
    PlayerGui(i18n("Default (Weak)"))
{
}


Player*
AiDefaultWeakGui::createInstance(Game *game, const QString &newName, const QColor &color) const
{
    return new AiDefaultWeak(game, newName, color);
}
