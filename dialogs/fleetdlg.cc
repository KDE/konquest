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

#include "fleetdlg.h"

#include <QTableWidget>
#include <QHeaderView>

#include <kcomponentdata.h>
#include <kglobal.h>
#include <kpushbutton.h>
#include <KStandardGuiItem>
#include <kguiitem.h>
#include <klocale.h>

#include "planet.h"

FleetDlg::FleetDlg( QWidget *parent,
                    const AttackFleetList &fleets,
                    const AttackFleetList &newFleets,
                    const AttackFleetList &standingOrders)
    : KDialog(parent), m_newFleetList(newFleets), m_standingOrders(standingOrders), m_fleetList(fleets)
{
    setObjectName( QLatin1String( "FleetDlg" ) );
    setModal( true );
    setCaption( i18n("Fleet Overview") );
    setButtons( KDialog::Ok );

    m_fleetTable = new QTableWidget(this);
    m_fleetTable->setColumnCount(7);
    m_fleetTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_fleetTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_fleetTable->setSelectionMode(QAbstractItemView::NoSelection);

    QStringList headerLabels;
    headerLabels
        << QString()
        << i18n("Fleet No.")
        << i18n("Source")
        << i18n("Destination")
        << i18n("Ships")
        << i18n("Kill Percentage")
        << i18n("Arrival Turn");
    m_fleetTable->setHorizontalHeaderLabels(headerLabels);
    m_fleetTable->verticalHeader()->hide();

    m_fleetTable->setMinimumSize( m_fleetTable->sizeHint() );

    setMainWidget( m_fleetTable );
    connect( this, SIGNAL(okClicked()), this, SLOT(accept()) );

    init();

    resize( 580, 140  );

    m_fleetTable->setSortingEnabled(true);
    m_fleetTable->sortItems( 1, Qt::AscendingOrder );
}


void
FleetDlg::init()
{
    AttackFleet *curFleet=0;
    AttackFleetList fleets = m_standingOrders + m_newFleetList + m_fleetList;
    const int standingOrders = m_standingOrders.count();
    const int newFleets = standingOrders + m_newFleetList.count();

    m_fleetTable->setRowCount( fleets.count() );
    QTableWidgetItem *item;

    for( int f = 0; f < fleets.count(); ++f) {
        curFleet = fleets.at(f);

        item = new QTableWidgetItem();
        if( f < newFleets) {
          item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
          item->setCheckState(Qt::Checked);
          if( f < standingOrders )
              item->setText(i18n("Standing order"));
        } else {
          item->setFlags(Qt::ItemIsEnabled);
        }
        m_fleetTable->setItem( f, 0, item );

        item = new QTableWidgetItem(QString::number(f + 1));
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 1, item );

        item = new QTableWidgetItem(curFleet->source->name());
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 2, item );

        item = new QTableWidgetItem(curFleet->destination->name());
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 3, item );

        item = new QTableWidgetItem(QString::number(curFleet->shipCount()));
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 4, item );

        item = new QTableWidgetItem(QString("%1") .arg(KGlobal::locale()->formatNumber(curFleet->source->killPercentage(), 3)));
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 5, item );

        item = new QTableWidgetItem(QString::number(curFleet->arrivalTurn));
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 6, item );
    }
}


AttackFleetList *FleetDlg::uncheckedFleets() {
    AttackFleetList *fleets = new AttackFleetList();
    QTableWidgetItem *item;
    int count = m_fleetTable->rowCount();

    for( int f = 0; f < count; ++f) {
        item = m_fleetTable->item(f,0);
        if( (item->flags() & Qt::ItemIsUserCheckable) &&
            (item->checkState() == Qt::Unchecked) ) {

            const int pos = m_fleetTable->item(f,1)->text().toInt() - 1;
            if (pos >= 0)
            {
                if(pos < m_standingOrders.count())
                    fleets->append( m_standingOrders.at(pos) );
                else
                    fleets->append( m_newFleetList.at(pos - m_standingOrders.count()) );
            }
        }
    }

    return fleets;
}
