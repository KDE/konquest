#include <ktablistbox.h>
#include <qlayout.h>
#include <qpushbt.h>
#include <kapp.h>

#include <iostream.h>

#include "scoredlg.h"
#include "scoredlg.moc"

ScoreDlg::ScoreDlg( QWidget *parent, const char *title, PlayerList *players )
    : QDialog(parent, "ScoreDlg", true ), plrList(players)
{
    setCaption( title );

    scoreTable = new KTabListBox( this, 0 );
    scoreTable->setNumCols( 6 );
    scoreTable->setColumn( 0, i18n("Player"), 70 );
    scoreTable->setColumn( 1, i18n("Ships Built"), 70 );
    scoreTable->setColumn( 2, i18n("Planets Conquered"), 120 );
    scoreTable->setColumn( 3, i18n("Fleets Launched"), 110 );
    scoreTable->setColumn( 4, i18n("Fleets Destroyed"), 100 );
    scoreTable->setColumn( 5, i18n("Ships Destroyed"), 100 );

    QPushButton *okButton = new QPushButton( i18n("OK"), this );
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

    for( ;(curPlayer = itr()); ) {
        QString item;

        char sep = scoreTable->separator();
        
        item.sprintf( "%s%c%d%c%d%c%d%c%d%c%d",
                      (const char *)curPlayer->getName(),
                      sep,
                      curPlayer->getShipsBuilt(),
                      sep,
                      curPlayer->getPlanetsConquered(),
                      sep,
                      curPlayer->getFleetsLaunched(),
                      sep,
                      curPlayer->getEnemyFleetsDestroyed(),
                      sep,
                      curPlayer->getEnemyShipsDestroyed()
                    );

        scoreTable->insertItem( item );
    }
}

