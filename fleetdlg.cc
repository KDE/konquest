#include <klistview.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <kapplication.h>
#include <klocale.h>
#include <math.h>
#include <kglobal.h>

#include "fleetdlg.h"
#include "fleetdlg.moc"

FleetDlg::FleetDlg( QWidget *parent, AttackFleetList *fleets )
    : QDialog(parent, "FleetDlg", true ), fleetList(fleets)
{
    setCaption( kapp->makeStdCaption(i18n("Fleet Overview")) );

    fleetTable = new KListView( this, 0 );
    fleetTable->addColumn(i18n("Fleet No."), 70 );
    fleetTable->addColumn(i18n("Destination"), 80 );
    fleetTable->addColumn(i18n("Ships"), 60 );
    fleetTable->addColumn(i18n("Kill Percentage"), 100 );
    fleetTable->addColumn(i18n("Arrival Turn"), 90 );

    QPushButton *okButton = new QPushButton( i18n("&OK"), this );
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
}

void
FleetDlg::init( void )
{
    AttackFleet *curFleet;
    AttackFleetListIterator nextFleet( *fleetList );
    int fleetNumber = 0;
    
    while( (curFleet = nextFleet())) {
        fleetNumber++;
        (void) new QListViewItem(fleetTable,
                                 QString("%1").arg(fleetNumber),
                                 QString("%1").arg(curFleet->destination->getName()),
                                 QString("%1").arg(curFleet->getShipCount()),
                                 QString("%1").arg(KGlobal::locale()->formatNumber(curFleet->killPercentage, 3)),
                                 QString("%1").arg((int)ceil(curFleet->arrivalTurn)));
    }
}
