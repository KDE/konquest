/*
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PLAYER_GUI_H
#define PLAYER_GUI_H

#include <QColor>
#include <QString>

#include "player.h"

class PlayerGui
{
public:
    explicit PlayerGui(const QString &guiName);
    virtual ~PlayerGui() {};
    virtual Player* createInstance(Game *game, const QString &newName, const QColor &color) const = 0;
    QString guiName() const;

private:
    QString m_guiName;
};

#endif // PLAYER_GUI_H
