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
    int w = m_standingsTable->verticalHeader()->width();
    int h = m_standingsTable->horizontalHeader()->height();

    for (int col = 0; col < m_standingsTable->columnCount(); ++col) {
        w += m_standingsTable->columnWidth(col);
    }

    for (int row = 0; row < m_standingsTable->rowCount(); ++row) {
        h += m_standingsTable->rowHeight(row);
    }

    /**
     * @todo The size calculated here does not yet prevent scrollbars to be
     * shown for the table. Figure out the offsets needed to be added and remove
     * the hard-coded numbers below!
     */

    return QSize(w, h) + QSize(m_standingsTable->columnCount() - 1, 40);
}


void
StandingsWidget::update(const QList<Player *> players )
{
    m_standingsTable->setRowCount(players.count());
    m_standingsTable->clearContents();
    m_standingsTable->setSortingEnabled(false);

    int row = 0;
    QTableWidgetItem *item;

    foreach (Player *curPlayer, players) {
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->name());
        item->setData(Qt::DecorationRole, curPlayer->color());
        item->setFlags(Qt::ItemIsEnabled);
        m_standingsTable->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->shipsBuilt());
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsEnabled);
        m_standingsTable->setItem(row, 1, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->planetsConquered());
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsEnabled);
        m_standingsTable->setItem(row, 2, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->fleetsLaunched());
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsEnabled);
        m_standingsTable->setItem(row, 3, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->enemyFleetsDestroyed());
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsEnabled);
        m_standingsTable->setItem(row, 4, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->enemyShipsDestroyed());
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsEnabled);
        m_standingsTable->setItem(row, 5, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->turnProduction());
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsEnabled);
        m_standingsTable->setItem(row, 6, item);

        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->turnShips());
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsEnabled);
        m_standingsTable->setItem(row, 7, item);

        ++row;
    }

    m_standingsTable->setSortingEnabled(true);
    m_standingsTable->resizeColumnsToContents();
}


void
StandingsWidget::setupTable()
{
    setObjectName(QLatin1String("widget-standings"));

    QVBoxLayout *main = new QVBoxLayout(this);

    m_standingsTable = new QTableWidget();
    m_standingsTable->setColumnCount(8);
    m_standingsTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_standingsTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_standingsTable->setSelectionMode(QAbstractItemView::NoSelection);

    QStringList headerLabels;
    headerLabels
        << i18nc("The player name", "Player")
        << i18n("Ships\nBuilt")
        << i18n("Planets\nConquered")
        << i18n("Fleets\nLaunched")
        << i18n("Fleets\nDestroyed")
        << i18n("Ships\nDestroyed")
        << i18n("Current\nProduction")
        << i18n("Current\nFleet Size");

    m_standingsTable->setHorizontalHeaderLabels(headerLabels);
    m_standingsTable->verticalHeader()->hide();

    main->addWidget(m_standingsTable);
}
