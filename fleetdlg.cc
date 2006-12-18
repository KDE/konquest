#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

#include <kinstance.h>
#include <kglobal.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kguiitem.h>
#include <klocale.h>

#include <math.h> // for ceil

#include "fleetdlg.h"
#include "planet.h"


FleetDlg::FleetDlg( QWidget *parent, AttackFleetList *fleets )
    : KDialog(parent), m_fleetList(fleets)
{
    setObjectName( "FleetDlg" );
    setModal( true );
    setCaption( i18n("Fleet Overview") );

    m_fleetTable = new QTableWidget( this );
    m_fleetTable->setColumnCount( 5 );
    QStringList labels;
    labels << i18n("Fleet No.")       << i18n("Destination") << i18n("Ships")
	   << i18n("Kill Percentage") << i18n("Arrival Turn");
    m_fleetTable->setHorizontalHeaderLabels( labels );
    m_fleetTable->verticalHeader()->hide();
    m_fleetTable->setMinimumSize( m_fleetTable->sizeHint() );
    m_fleetTable->setSelectionMode( QAbstractItemView::NoSelection );

    KPushButton *okButton = new KPushButton( KStdGuiItem::ok(), this );
    okButton->setMinimumSize( okButton->sizeHint() );
    okButton->setDefault(true);

    QVBoxLayout *layout1 = new QVBoxLayout( this );
    QHBoxLayout *layout2 = new QHBoxLayout;

    layout1->addWidget( m_fleetTable, 1 );
    layout1->addLayout( layout2 );

    layout2->addStretch( 2 );
    layout2->addWidget( okButton );
    layout2->addStretch( 2 );

    connect( okButton, SIGNAL(clicked()), this, SLOT(accept()) );

    init();

    resize( 580, 140  );

    m_fleetTable->setSortingEnabled(true);
    m_fleetTable->sortItems( 0, Qt::AscendingOrder );
}

void
FleetDlg::init()
{
    AttackFleet  *curFleet=0;

    m_fleetTable->setRowCount( m_fleetList->count() );
    QTableWidgetItem *item;
    
    for( int f = 0; f < m_fleetList->count(); ++f) {
        curFleet = m_fleetList->at(f);
        
        item = new QTableWidgetItem(QString::number(f + 1));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_fleetTable->setItem( f, 0, item );
        
        item = new QTableWidgetItem(curFleet->destination->name());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_fleetTable->setItem( f, 1, item );
        
        item = new QTableWidgetItem(QString::number(curFleet->shipCount()));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_fleetTable->setItem( f, 2, item );
        
        item = new QTableWidgetItem(QString("%1") .arg(KGlobal::locale()->formatNumber(curFleet->killPercentage, 3)));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_fleetTable->setItem( f, 3, item );
        
        item = new QTableWidgetItem(QString::number((int)ceil(curFleet->arrivalTurn)));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_fleetTable->setItem( f, 4, item );
    }
}
