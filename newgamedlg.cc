#include <q3header.h>
#include <QLayout>
#include <QColor>
#include <QLabel>
#include <qslider.h>
#include <qevent.h>
#include <qnamespace.h>
#include <q3listview.h>
#include <QPushButton>
#include <QLineEdit>
#include <QPixmap>

#include <kapplication.h>
#include <kconfig.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kglobal.h>

#include "newgamedlg.h"
#include "newgamedlg.moc"
#include "player.h"
#include "map.h"

/*************************************************************************
 New Game Dialog Members
 ************************************************************************/

NewGameDlg::NewGameDlg( QWidget *parent, Map *pmap, QList<Player *> *players,
                        Player *neutralPlayer, QList<Planet *> *planets )
    : KDialog( parent),
      plrList(players), plnetList(planets), neutral(neutralPlayer),
      map(pmap)
{
    setButtons(KDialog::Ok|KDialog::Default|KDialog::Cancel);
    setDefaultButton(KDialog::NoDefault);
    showButtonSeparator(true);
    setCaption(i18n("Start New Game"));
    w = new NewGameDlgUI(this);
    w->map->setMap(map);
    w->listPlayers->header()->hide();
//    w->listPlayers->setMinimumSize( 100, 150 );
    w->listPlayers->setSortColumn(-1);
    w->listPlayers->setHScrollBarMode(Q3ScrollView::AlwaysOff);
    w->sliderPlayers->setMinimumWidth(270);
    w->sliderPlanets->setMinimumWidth(270);

    w->newPlayer->setMaxLength( 8 );

    connect(w->sliderPlayers, SIGNAL(valueChanged(int)), this, SLOT(slotPlayerCount(int)));
    connect(w->sliderPlanets, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(w->rejectMap, SIGNAL(clicked()), this, SLOT(slotNewMap()));
    connect(w->newPlayer, SIGNAL(textChanged(const QString &)), this, SLOT(slotNewPlayer()));
    connect(w->newPlayer, SIGNAL(returnPressed()), this, SLOT(slotAddPlayer()));
    connect(w->addPlayer, SIGNAL(clicked()), this, SLOT(slotAddPlayer()));

    init();

    setMainWidget(w);
}

void
NewGameDlg::slotDefault()
{
    w->sliderPlayers->setValue(2);
    w->sliderPlanets->setValue(3);

    w->listPlayers->clear();

    setPlayerCount(2);
    
    updateMiniMap();
    updateLabels();
}

void
NewGameDlg::init()
{
    KConfig *config = KGlobal::config();
    config->setGroup("Game");
    int nrOfPlayers = config->readEntry("NrOfPlayers",0);
    if (nrOfPlayers < 2)
       nrOfPlayers = 2;
    if (nrOfPlayers > MAX_PLAYERS)
       nrOfPlayers = MAX_PLAYERS;

    int nrOfPlanets = config->readEntry("NrOfPlanets", 3);
       
    w->sliderPlayers->setValue(nrOfPlayers);
    w->sliderPlanets->setValue(nrOfPlanets);
    setPlayerCount(nrOfPlayers);
    slotNewPlayer();
    
    // Restore player names
    int plrNum = 0;
    for( Q3ListViewItem *item = w->listPlayers->firstChild(); 
         item; item = item->nextSibling(), plrNum++ )
    {
       QString key = QString("Player_%1").arg(plrNum);
       
       QString playerName = config->readEntry(key,QString());
       if (playerName.isEmpty())
          continue;

       item->setText(2, "H"); // Human
       item->setText(1, i18n("Human Player"));
       item->setText(0, playerName);
    }

    updateMiniMap();
    updateLabels();
}

void
NewGameDlg::slotNewPlayer()
{
    w->addPlayer->setEnabled(!w->newPlayer->text().isEmpty());
}

void
NewGameDlg::slotAddPlayer()
{
    QString playerName = w->newPlayer->text();
    if (playerName.isEmpty())
       return;
       
    Q3ListViewItem *item;
    do
    {
       item = w->listPlayers->firstChild();
       while( item )
       {
          if (item->text(2) == "A")
             break;

          item = item->nextSibling();
       }
       if (!item)
       {
          int nrPlayers = w->listPlayers->childCount();
          if (nrPlayers >= MAX_PLAYERS)
             return; // Too bad
          nrPlayers++;
          w->sliderPlayers->setValue(nrPlayers);
          setPlayerCount(nrPlayers);
       }
    }
    while(!item);
    
    item->setText(2, "H"); // Human
    item->setText(1, i18n("Human Player"));
    item->setText(0, playerName);
    
    w->newPlayer->setText(QString::null);
    
    updateMiniMap();
    updateLabels();
}

void
NewGameDlg::setPlayerCount(int playerCount)
{
    QColor PlayerColors[MAX_PLAYERS] = { QColor( 130, 130, 255 ), Qt::yellow, Qt::red, Qt::green,
    		Qt::white, Qt::cyan, Qt::magenta, QColor( 235, 153, 46 ),
		QColor( 106, 157, 104 ),  QColor( 131, 153, 128) };
       
    int i = 0;
    Q3ListViewItem *lastItem = 0;
    Q3ListViewItem *item = 0;
    Q3ListViewItem *nextItem = w->listPlayers->firstChild();
    while( (item = nextItem) )
    {
       nextItem = item->nextSibling();
       if (i >= playerCount)
       {
          delete item;
       }
       else
       {
          lastItem = item;
       }
       i++;
    }
    
    while(w->listPlayers->childCount() < playerCount)
    {
       QString playerName = i18nc("Generated AI player name", "Comp%1", i+1);
       QPixmap pm(16,16);
       QColor color(PlayerColors[i]);
       pm.fill(color);
       Q3ListViewItem *item = new Q3ListViewItem(w->listPlayers, lastItem, playerName, i18n("Computer Player"), "A", color.name());
       item->setPixmap(0, pm);
       lastItem = item;
       i++;
    }
}

void
NewGameDlg::slotPlayerCount(int playerCount)
{
    if (w->listPlayers->childCount() == playerCount)
       return;
       
    setPlayerCount(playerCount);
       
    updateMiniMap();
    updateLabels();
}

void
NewGameDlg::slotTurns()
{
    updateLabels();
}

void
NewGameDlg::slotNewMap()
{
    updateMiniMap();
    updateLabels();
}

int
NewGameDlg::turns()
{
    return 42;
}

void
NewGameDlg::updateLabels()
{
    w->labelPlayers->setText(i18n("Number of &players: %1", w->sliderPlayers->value()));
    w->labelPlanets->setText(i18n("Number of neutral p&lanets: %1", w->sliderPlanets->value()));
}

void
NewGameDlg::slotOk()
{
    bool hasHumans = false;
    for( Q3ListViewItem *item = w->listPlayers->firstChild(); 
         item; item = item->nextSibling() )
    {
        bool ai = (item->text(2) == "A");
        if (!ai)
           hasHumans = true;
    }

    if (!hasHumans)
    {
        KMessageBox::information(this, i18n("The game is much more fun when you add a human player!"));
        w->newPlayer->setFocus();
        return;
    }
	KDialog::accept();
}

void
NewGameDlg::save()
{
    KConfig *config = KGlobal::config();
    config->setGroup("Game");
    
    config->writeEntry("NrOfPlayers", w->sliderPlayers->value());
    config->writeEntry("NrOfPlanets", w->sliderPlanets->value());

    int plrNum = 0;
    for( Q3ListViewItem *item = w->listPlayers->firstChild(); 
         item; item = item->nextSibling() )
    {
        QString key = QString("Player_%1").arg(plrNum);
        QString playerName = item->text(0);
        bool ai = (item->text(2) == "A");
        if (ai)
        {
           if (config->hasKey(key))
              config->deleteEntry(key);
        }
        else
        {
           config->writeEntry(key, playerName);
        }
        plrNum++;
    }
    config->sync();
}

void
NewGameDlg::updateMiniMap()
{
    // Clear map,, player and planet lists
    map->clearMap();

    while (!plnetList->isEmpty())
        delete plnetList->takeFirst();

    while (!plrList->isEmpty())
        delete plrList->takeFirst();

    // Make player list
    // Does the name already exist in the list
    int plrNum = 0;
    for( Q3ListViewItem *item = w->listPlayers->firstChild(); 
         item; item = item->nextSibling() )
    {
        QString playerName = item->text(0);
        bool ai = (item->text(2) == "A");
        QColor color(item->text(3));
        plrList->append( Player::createPlayer( map, playerName, color, plrNum, ai ));
        plrNum++;
    }

    // make the planets
    map->populateMap( *plrList, neutral,
                      w->sliderPlanets->value(),
                      *plnetList );
}

