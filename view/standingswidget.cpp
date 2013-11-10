/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>
    Copyright 2013 Alexander Schuch <aschuch247@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "standingswidget.h"

#include <QHeaderView>
#include <QTableWidget>
#include <QVBoxLayout>

#include <klocale.h>
#include <kcomponentdata.h>
#include <KStandardGuiItem>
#include <kguiitem.h>


StandingsWidget::StandingsWidget(QWidget *parent) :
    QWidget(parent)
{
    setupTable();
}


/**
 * @note Use "delegating constructor" once C++11 is required by KDE.
 */

StandingsWidget::StandingsWidget(QWidget *parent, const QList<Player *> players) :
    // StandingsWidget(parent)
    QWidget(parent)
{
    setupTable();
    update(players);
}


StandingsWidget::~StandingsWidget()
{
}


QSize
StandingsWidget::sizeHint() const
{
    int w = m_scoreTable->verticalHeader()->width();
    int h = m_scoreTable->horizontalHeader()->height();

    for (int col = 0; col < m_scoreTable->columnCount(); ++col) {
        w += m_scoreTable->columnWidth(col);
    }

    for (int row = 0; row < m_scoreTable->rowCount(); ++row ) {
        h += m_scoreTable->rowHeight(row);
    }

    /**
     * @todo The size calculated here does not yet prevent scrollbars to be
     * shown for the table. Figure out the offsets needed to be added and remove
     * the hard-coded numbers below!
     */

    return QSize(w, h) + QSize(20, 40);
}


void
StandingsWidget::update(const QList<Player *> players )
{
    m_scoreTable->setRowCount(players.count());
    m_scoreTable->clearContents();

    m_scoreTable->setSortingEnabled(false);

    int row = 0;

    QTableWidgetItem *item;

    foreach (Player *curPlayer, players) {
        item = new QTableWidgetItem(curPlayer->name());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->shipsBuilt());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 1, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->planetsConquered());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 2, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->fleetsLaunched());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 3, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->enemyFleetsDestroyed());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 4, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->enemyShipsDestroyed());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 5, item);

        ++row;
    }

    m_scoreTable->setSortingEnabled(true);
    m_scoreTable->resizeColumnsToContents();
}


void
StandingsWidget::setupTable()
{
    setObjectName(QLatin1String("widget-standings"));

    QVBoxLayout *main = new QVBoxLayout(this);

    m_scoreTable = new QTableWidget();
    m_scoreTable->setColumnCount(6);
    m_scoreTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_scoreTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_scoreTable->setSelectionMode(QAbstractItemView::NoSelection);

    QStringList headers;
    headers
        << i18nc("The player name", "Player")
        << i18n("Ships\nBuilt")
        << i18n("Planets\nConquered")
        << i18n("Fleets\nLaunched")
        << i18n("Fleets\nDestroyed")
        << i18n("Ships\nDestroyed");
    m_scoreTable->setHorizontalHeaderLabels(headers);
    m_scoreTable->verticalHeader()->hide();

    main->addWidget(m_scoreTable);
}
