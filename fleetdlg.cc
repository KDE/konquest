#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

#include <kinstance.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kguiitem.h>
#include <klocale.h>

#include <math.h> // for ceil

#include "fleetdlg.h"

FleetDlg::FleetDlg( QWidget *parent, AttackFleetList *fleets )
    : QDialog(parent), fleetList(fleets)
{
    setObjectName( "FleetDlg" );
    setModal( true );
    setWindowTitle( KInstance::makeStdCaption(i18n("Fleet Overview")) );

    fleetTable = new QTableWidget( this );
    fleetTable->setColumnCount( 5 );
    QStringList labels;
    labels << i18n("Fleet No.") << i18n("Destination") << i18n("Ships");
    labels << i18n("Kill Percentage") << i18n("Arrival Turn");
    fleetTable->setHorizontalHeaderLabels( labels );
    fleetTable->verticalHeader()->hide();
    fleetTable->setMinimumSize( fleetTable->sizeHint() );
    fleetTable->setSelectionMode( QAbstractItemView::NoSelection );

    KPushButton *okButton = new KPushButton( KStdGuiItem::ok(), this );
    okButton->setMinimumSize( okButton->sizeHint() );
    okButton->setDefault(true);

    QVBoxLayout *layout1 = new QVBoxLayout( this );
    QHBoxLayout *layout2 = new QHBoxLayout;

    layout1->addWidget( fleetTable, 1 );
    layout1->addLayout( layout2 );

    layout2->addStretch( 2 );
    layout2->addWidget( okButton );
    layout2->addStretch( 2 );

    connect( okButton, SIGNAL(clicked()), this, SLOT(accept()) );

    init();

    resize( 580, 140  );

    fleetTable->setSortingEnabled(true);
    fleetTable->sortItems( 0, Qt::AscendingOrder );
}

void
FleetDlg::init()
{
    AttackFleet *curFleet=0;

    fleetTable->setRowCount( fleetList->count() );
    QTableWidgetItem *item;
    
    for( unsigned f=0; f< fleetList->count(); ++f)
    {
        curFleet = fleetList->at(f);
        
        item = new QTableWidgetItem(QString::number(f+1));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        fleetTable->setItem( f, 0, item );
        
        item = new QTableWidgetItem(curFleet->destination->getName());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        fleetTable->setItem( f, 1, item );
        
        item = new QTableWidgetItem(QString::number(curFleet->getShipCount()));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        fleetTable->setItem( f, 2, item );
        
        item = new QTableWidgetItem(QString("%1").arg(KGlobal::locale()->formatNumber(curFleet->killPercentage, 3)));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        fleetTable->setItem( f, 3, item );
        
        item = new QTableWidgetItem(QString::number((int)ceil(curFleet->arrivalTurn)));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        fleetTable->setItem( f, 4, item );
    }
}
