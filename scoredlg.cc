#include <QLayout>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <kapplication.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kguiitem.h>

#include "scoredlg.h"


ScoreDlg::ScoreDlg( QWidget *parent, const QString& title, QList<Player *> *players )
    : QDialog(parent), plrList(players)
{
    setObjectName( "ScoreDlg" );
    setModal( true );
    setWindowTitle( KInstance::makeStdCaption(title) );

    scoreTable = new QTableWidget( this );
    scoreTable->setColumnCount(6);
    QStringList headers;
    headers << i18n("Player") << i18n("Ships Built") << i18n("Planets Conquered");
    headers << i18n("Fleets Launched") << i18n("Fleets Destroyed") << i18n("Ships Destroyed");
    scoreTable->setHorizontalHeaderLabels(headers);
    scoreTable->verticalHeader()->hide();
    init();
    scoreTable->setMinimumSize( scoreTable->sizeHint() );
    scoreTable->setSelectionMode( QAbstractItemView::NoSelection );
    scoreTable->setSortingEnabled(true);

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

    resize( 580, 140  );
}

void
ScoreDlg::init()
{
    scoreTable->setRowCount(plrList->count());
    int row = 0;
    
    QTableWidgetItem *item;
    foreach (Player *curPlayer, (*plrList)) {
        item = new QTableWidgetItem(curPlayer->name());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 0, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->shipsBuilt());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 1, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->planetsConquered());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 2, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->fleetsLaunched());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 3, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->enemyFleetsDestroyed());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 4, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->enemyShipsDestroyed());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 5, item);
        
        row++;
    }
}

