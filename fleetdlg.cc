#include <qlayout.h>
#include <kapplication.h>
#include <klocale.h>
#include <math.h>
#include <kglobal.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>

#include "fleetdlg.h"

FleetDlgListViewItem::FleetDlgListViewItem(QListView *parent, QString s1, QString s2, QString s3, QString s4, QString s5) : QListViewItem(parent, s1, s2, s3, s4, s5)
{
}

int FleetDlgListViewItem::compare(QListViewItem *i, int col, bool) const
{
	if (col == 1)
	{
		if (text(col) > i -> text(col)) return 1;
		else if (text(col) < i -> text(col)) return -1;
		else return compare(i, 0, true);
	}
	else if (col == 3)
	{
		if (text(col).toDouble() > i -> text(col).toDouble()) return 1;
		else if (text(col).toDouble() < i -> text(col).toDouble()) return -1;
		else return compare(i, 0, true);
	}
	else
	{
		if (text(col).toInt() > i -> text(col).toInt()) return 1;
		else if (text(col).toInt() < i -> text(col).toInt()) return -1;
		else return compare(i, 0, true);
	}
}


FleetDlg::FleetDlg( QWidget *parent, AttackFleetList *fleets )
    : QDialog(parent, "FleetDlg", true ), fleetList(fleets)
{
    setCaption( kapp->makeStdCaption(i18n("Fleet Overview")) );

    fleetTable = new KListView( this, 0 );
    fleetTable->addColumn(i18n("Fleet No."));
    fleetTable->addColumn(i18n("Destination"));
    fleetTable->addColumn(i18n("Ships"));
    fleetTable->addColumn(i18n("Kill Percentage"));
    fleetTable->addColumn(i18n("Arrival Turn"));
    fleetTable->setMinimumSize( fleetTable->sizeHint() );

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
}

void
FleetDlg::init()
{
    AttackFleet *curFleet;
    AttackFleetListIterator nextFleet( *fleetList );
    int fleetNumber = 0;
    
    while( (curFleet = nextFleet())) {
        fleetNumber++;
        new FleetDlgListViewItem(fleetTable,
                                 QString("%1").arg(fleetNumber),
                                 curFleet->destination->getName(),
                                 QString("%1").arg(curFleet->getShipCount()),
                                 QString("%1").arg(KGlobal::locale()->formatNumber(curFleet->killPercentage, 3)),
                                 QString("%1").arg((int)ceil(curFleet->arrivalTurn)));
    }
}
