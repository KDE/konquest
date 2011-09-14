/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright Inge Wallin <inge@lysator.liu.se>
    Copyright Pierre Ducroquet <pinaraf@gmail.com>

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
                    const AttackFleetList &newFleets )
    : KDialog(parent), m_newFleetList(newFleets), m_fleetList(fleets)
{
    setObjectName( QLatin1String( "FleetDlg" ) );
    setModal( true );
    setCaption( i18n("Fleet Overview") );
    setButtons( KDialog::Ok );

    m_fleetTable = new QTableWidget( this );
    m_fleetTable->setColumnCount( 6 );
    QStringList labels;
    labels << QString() << i18n("Fleet No.")       << i18n("Destination") << i18n("Ships")
	   << i18n("Kill Percentage") << i18n("Arrival Turn");
    m_fleetTable->setHorizontalHeaderLabels( labels );
    m_fleetTable->verticalHeader()->hide();
    m_fleetTable->setMinimumSize( m_fleetTable->sizeHint() );
    m_fleetTable->setSelectionMode( QAbstractItemView::NoSelection );

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
    AttackFleetList fleets = m_newFleetList + m_fleetList;
    int newFleets = m_newFleetList.count();

    m_fleetTable->setRowCount( fleets.count() );
    QTableWidgetItem *item;
    
    for( int f = 0; f < fleets.count(); ++f) {
        curFleet = fleets.at(f);
        
        item = new QTableWidgetItem();
        if( f < newFleets) {
          item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
          item->setCheckState(Qt::Checked);
        } else {
          item->setFlags(Qt::ItemIsEnabled);
        }
        m_fleetTable->setItem( f, 0, item );

        item = new QTableWidgetItem(QString::number(f + 1));
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 1, item );
        
        item = new QTableWidgetItem(curFleet->destination->name());
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 2, item );
        
        item = new QTableWidgetItem(QString::number(curFleet->shipCount()));
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 3, item );
        
        item = new QTableWidgetItem(QString("%1") .arg(KGlobal::locale()->formatNumber(curFleet->source->killPercentage(), 3)));
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 4, item );
        
        item = new QTableWidgetItem(QString::number(curFleet->arrivalTurn));
        item->setFlags(Qt::ItemIsEnabled);
        m_fleetTable->setItem( f, 5, item );
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

            int pos = m_fleetTable->item(f,1)->text().toInt();
            if (pos > 0)
                fleets->append( m_newFleetList.at(pos-1) );
        }
    }

    return fleets;
}
