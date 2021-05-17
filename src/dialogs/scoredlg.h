/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_SCOREDLG_H
#define KONQUEST_SCOREDLG_H

#include <QDialog>

#include "../players/player.h"
#include "../view/standingswidget.h"


class ScoreDlg : public QDialog
{

public:
    ScoreDlg( QWidget *parent, const QString& title, 
              const QList<Player *> &players );
    ~ScoreDlg();

private:
    StandingsWidget *m_standingsWidget;

};

#endif // KONQUEST_SCOREDLG_H
