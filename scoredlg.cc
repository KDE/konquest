#include <qlayout.h>
#include <kapplication.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>

#include "scoredlg.h"

ScoreDlgListViewItem::ScoreDlgListViewItem(QListView *parent, QString s1, QString s2, QString s3, QString s4, QString s5, QString s6) : QListViewItem(parent, s1, s2, s3, s4, s5, s6)
{
}

int ScoreDlgListViewItem::compare(QListViewItem *i, int col, bool) const
{
	if (col == 0)
	{
		if (text(col) > i -> text(col)) return 1;
		else if (text(col) < i -> text(col)) return -1;
		else return 0;
	}
	else
	{
		if (text(col).toInt() > i -> text(col).toInt()) return 1;
		else if (text(col).toInt() < i -> text(col).toInt()) return -1;
		else return compare(i, 0, true);
	}
}


ScoreDlg::ScoreDlg( QWidget *parent, const QString& title, PlayerList *players )
    : QDialog(parent, "ScoreDlg", true ), plrList(players)
{
    setCaption( kapp->makeStdCaption(title) );

    scoreTable = new KListView( this, 0 );
    scoreTable->addColumn(i18n("Player"));
    scoreTable->addColumn(i18n("Ships Built"));
    scoreTable->addColumn(i18n("Planets Conquered"));
    scoreTable->addColumn(i18n("Fleets Launched"));
    scoreTable->addColumn(i18n("Fleets Destroyed"));
    scoreTable->addColumn(i18n("Ships Destroyed"));
    scoreTable->setMinimumSize( scoreTable->sizeHint() );

    KPushButton *okButton = new KPushButton( KStdGuiItem::ok(), this );
    okButton->setMinimumSize( okButton->sizeHint() );
    okButton->setDefault(true);

    QVBoxLayout *layout1 = new QVBoxLayout( this );
    QHBoxLayout *layout2 = new QHBoxLayout;

    layout1->addWidget( scoreTable, 1 );
    layout1->addLayout( layout2 );

    layout2->addStretch( 2 );
    layout2->addWidget( okButton );
    layout2->addStretch( 2 );

    connect( okButton, SIGNAL(clicked()), this, SLOT(accept()) );

    init();

    resize( 580, 140  );
}

void
ScoreDlg::init()
{
    Player *curPlayer;
    PlayerListIterator itr( *plrList );

    for( ;(curPlayer = itr()); )
        new ScoreDlgListViewItem(scoreTable,
                                 curPlayer->getName(),
                                 QString("%1").arg(curPlayer->getShipsBuilt()),
                                 QString("%1").arg(curPlayer->getPlanetsConquered()),
                                 QString("%1").arg(curPlayer->getFleetsLaunched()),
                                 QString("%1").arg(curPlayer->getEnemyFleetsDestroyed()),
                                 QString("%1").arg(curPlayer->getEnemyShipsDestroyed()));
}

