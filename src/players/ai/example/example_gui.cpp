/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "example_gui.h"

#include <KLocalizedString>

#include "example.h"


AiExampleGui::AiExampleGui() :
    PlayerGui(i18n("Example (Passive)"))
{
}


Player*
AiExampleGui::createInstance(Game *game, const QString &newName, const QColor &color) const
{
    return new AiExample(game, newName, color);
}
