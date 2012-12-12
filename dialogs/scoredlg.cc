/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>

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
#include "scoredlg.h"

#include <klocale.h>
#include <kcomponentdata.h>
#include <KStandardGuiItem>
#include <kguiitem.h>

#include <QHeaderView>

ScoreDlg::ScoreDlg( QWidget *parent, const QString& title, QList<Player *> players )
    : KDialog(parent), m_players(players)
{
    setObjectName( QLatin1String( "ScoreDlg" ) );
    setModal( true );
    setCaption(title);
    setButtons( KDialog::Ok );

    // Create the table.
    m_scoreTable = new QTableWidget( this );
    m_scoreTable->setColumnCount(6);
    QStringList headers;
    headers << i18nc("The player name", "Player") << i18n("Ships\nBuilt") 
	    << i18n("Planets\nConquered") << i18n("Fleets\nLaunched")
	    << i18n("Fleets\nDestroyed") << i18n("Ships\nDestroyed");
    m_scoreTable->setHorizontalHeaderLabels(headers);
    m_scoreTable->verticalHeader()->hide();
    init();

    m_scoreTable->setMinimumSize( m_scoreTable->sizeHint() );
    m_scoreTable->setSelectionMode( QAbstractItemView::NoSelection );
    m_scoreTable->setSortingEnabled(true);
    
    setMainWidget( m_scoreTable );
    connect( this, SIGNAL(okClicked()), this, SLOT(accept()) );

    resize( 580, 140  );
}

ScoreDlg::~ScoreDlg()
{
}


void
ScoreDlg::init()
{
    m_scoreTable->setRowCount(m_players.count());
    int row = 0;
    
    QTableWidgetItem *item;
    foreach (Player *curPlayer, m_players) {
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
        
        row++;
    }
}

