#include <QLayout>
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <kapplication.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kguiitem.h>

#include "scoredlg.h"


ScoreDlg::ScoreDlg( QWidget *parent, const QString& title, PlayerList *players )
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
    scoreTable->setMinimumSize( scoreTable->sizeHint() );
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

    init();

    resize( 580, 140  );
}

void
ScoreDlg::init()
{
    scoreTable->setRowCount(plrList->count());
    int row = 0;
    
    foreach (Player *curPlayer, (*plrList)) {
        QTableWidgetItem *nameItem = new QTableWidgetItem(curPlayer->name());
        nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 0, nameItem);
        
        QTableWidgetItem *shipsBuiltItem = new QTableWidgetItem(QString::number(curPlayer->getShipsBuilt()));
        shipsBuiltItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 1, shipsBuiltItem);
        
        QTableWidgetItem *planetsConqueredItem = new QTableWidgetItem(QString::number(curPlayer->getPlanetsConquered()));
        planetsConqueredItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 2, planetsConqueredItem);
        
        QTableWidgetItem *fleetsLaunchedItem = new QTableWidgetItem(QString::number(curPlayer->getFleetsLaunched()));
        fleetsLaunchedItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 3, fleetsLaunchedItem);
        
        QTableWidgetItem *enemyFleetsDestroyedItem = new QTableWidgetItem(QString::number(curPlayer->getEnemyFleetsDestroyed()));
        enemyFleetsDestroyedItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 4, enemyFleetsDestroyedItem);
        
        QTableWidgetItem *enemyShipsDestroyedItem = new QTableWidgetItem(QString::number(curPlayer->getEnemyShipsDestroyed()));
        enemyShipsDestroyedItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        scoreTable->setItem(row, 5, enemyShipsDestroyedItem);
        
        row++;
    }
}

