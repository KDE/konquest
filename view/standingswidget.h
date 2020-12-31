/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>
    SPDX-FileCopyrightText: 2013 Alexander Schuch <aschuch247@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_STANDINGSWIDGET_H
#define KONQUEST_STANDINGSWIDGET_H

#include <QWidget>

#include "../players/player.h"


class QTableWidget;


class StandingsWidget : public QWidget
{

public:
    explicit StandingsWidget(QWidget *parent);
    explicit StandingsWidget(QWidget *parent, const QList<Player *> &players);
    ~StandingsWidget();

    QSize sizeHint() const override;
    void update(const QList<Player *> &players);

private:
    void setupTable();

    QTableWidget *m_standingsTable;
};

#endif // KONQUEST_STANDINGSWIDGET_H
