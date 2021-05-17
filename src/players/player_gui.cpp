/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "player_gui.h"


PlayerGui::PlayerGui(const QString& guiName) :
    m_guiName(guiName)
{
}


QString
PlayerGui::guiName() const
{
    return m_guiName;
}
