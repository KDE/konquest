/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "becai_gui.h"
#include "becai.h"

#include <KLocalizedString>

AiBecaiGui::AiBecaiGui() :
    PlayerGui(i18nc("A distinct unique AI playing a balanced strategy", "Becai (Balanced)"))
{
}


Player*
AiBecaiGui::createInstance(Game *game, const QString &newName, const QColor &color) const
{
    return new AiBecai(game, newName, color);
}
