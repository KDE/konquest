#include <klistview.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <kapp.h>
#include <klocale.h>

#include "scoredlg.h"
#include "scoredlg.moc"

ScoreDlg::ScoreDlg( QWidget *parent, const QString& title, PlayerList *players )
    : QDialog(parent, "ScoreDlg", true ), plrList(players)
{
    setCaption( kapp->makeStdCaption(title) );

    scoreTable = new KListView( this, 0 );
    scoreTable->addColumn(i18n("Player"), 70 );
    scoreTable->addColumn(i18n("Ships Built"), 70 );
    scoreTable->addColumn(i18n("Planets Conquered"), 120 );
    scoreTable->addColumn(i18n("Fleets Launched"), 110 );
    scoreTable->addColumn(i18n("Fleets Destroyed"), 100 );
    scoreTable->addColumn(i18n("Ships Destroyed"), 100 );

    QPushButton *okButton = new QPushButton( i18n("&OK"), this );
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
ScoreDlg::init( void )
{
    Player *curPlayer;
    PlayerListIterator itr( *plrList );

    for( ;(curPlayer = itr()); )
        (void) new QListViewItem(scoreTable,
                                 curPlayer->getName(),
                                 QString("%1").arg(curPlayer->getShipsBuilt()),
                                 QString("%1").arg(curPlayer->getPlanetsConquered()),
                                 QString("%1").arg(curPlayer->getFleetsLaunched()),
                                 QString("%1").arg(curPlayer->getEnemyFleetsDestroyed()),
                                 QString("%1").arg(curPlayer->getEnemyShipsDestroyed()));
}

