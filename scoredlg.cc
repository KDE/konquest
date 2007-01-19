#include <QHeaderView>
#include <klocale.h>
#include <kinstance.h>
#include <KStandardGuiItem>
#include <kguiitem.h>

#include "scoredlg.h"


ScoreDlg::ScoreDlg( QWidget *parent, const QString& title, QList<Player *> *players )
    : KDialog(parent), m_players(players)
{
    setObjectName( "ScoreDlg" );
    setModal( true );
    setCaption(title);
    setButtons( KDialog::Ok );

    // Create the table.
    m_scoreTable = new QTableWidget( this );
    m_scoreTable->setColumnCount(6);
    QStringList headers;
    headers << i18n("Player") << i18n("Ships\nBuilt") 
	    << i18n("Planets\nConquered") << i18n("Fleets\nLaunched")
	    << i18n("Fleets\nDestroyed") << i18n("Ships\nDestroyed");
    m_scoreTable->setHorizontalHeaderLabels(headers);
    m_scoreTable->verticalHeader()->hide();
    init();

    m_scoreTable->setMinimumSize( m_scoreTable->sizeHint() );
    m_scoreTable->setSelectionMode( QAbstractItemView::NoSelection );
    m_scoreTable->setSortingEnabled(true);
    
    setMainWidget( m_scoreTable );
    connect( this, SIGNAL(okclicked()), this, SLOT(accept()) );

    resize( 580, 140  );
}

ScoreDlg::~ScoreDlg()
{
}


void
ScoreDlg::init()
{
    m_scoreTable->setRowCount(m_players->count());
    int row = 0;
    
    QTableWidgetItem *item;
    foreach (Player *curPlayer, (*m_players)) {
        item = new QTableWidgetItem(curPlayer->name());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 0, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->shipsBuilt());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 1, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->planetsConquered());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 2, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->fleetsLaunched());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 3, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->enemyFleetsDestroyed());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 4, item);
        
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, curPlayer->enemyShipsDestroyed());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_scoreTable->setItem(row, 5, item);
        
        row++;
    }
}

