#include <qheader.h>
#include <qlayout.h>
#include <qcolor.h>
#include <qlabel.h>
#include <qslider.h>
#include <qevent.h>
#include <qkeycode.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlineedit.h>

#include <kapplication.h>
#include <kconfig.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>

#include "newgamedlg.h"
#include "newgamedlg.moc"

#include "newGameDlg_ui.h"

/*************************************************************************
 New Game Dialog Members
 ************************************************************************/

NewGameDlg::NewGameDlg( QWidget *parent, Map *pmap, PlayerList *players,
                        Player *neutralPlayer, PlanetList *planets )
    : KDialogBase( parent, "new_game_dialog", true, i18n("Start New Game"),
                   KDialogBase::Ok|KDialogBase::Default|KDialogBase::Cancel, KDialogBase::NoDefault, true ),
      plrList(players), plnetList(planets), neutral(neutralPlayer),
      map(pmap)
{
    w = new NewGameDlgUI(this);
    w->map->setMap(map);
    w->listPlayers->header()->hide();
//    w->listPlayers->setMinimumSize( 100, 150 );
    w->listPlayers->setSortColumn(-1);
    w->listPlayers->setHScrollBarMode(QScrollView::AlwaysOff);
    w->sliderPlayers->setMinimumWidth(270);
    w->sliderPlanets->setMinimumWidth(270);

    w->newPlayer->setMaxLength( 8 );

    connect(w->sliderPlayers, SIGNAL(valueChanged(int)), this, SLOT(slotPlayerCount(int)));
    connect(w->sliderPlanets, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(w->sliderTurns, SIGNAL(valueChanged(int)), this, SLOT(slotTurns()));
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
    w->sliderTurns->setValue(15);

    w->listPlayers->clear();

    setPlayerCount(2);
    
    updateMiniMap();
    updateLabels();
}

void
NewGameDlg::init()
{
    KConfig *config = kapp->config();
    config->setGroup("Game");
    int nrOfPlayers = config->readNumEntry("NrOfPlayers");
    if (nrOfPlayers < 2)
       nrOfPlayers = 2;
    if (nrOfPlayers > MAX_PLAYERS)
       nrOfPlayers = MAX_PLAYERS;

    int nrOfPlanets = config->readNumEntry("NrOfPlanets", 3);
    int nrOfTurns = config->readNumEntry("NrOfTurns", 15);
       
    w->sliderPlayers->setValue(nrOfPlayers);
    w->sliderPlanets->setValue(nrOfPlanets);
    w->sliderTurns->setValue(nrOfTurns);
    setPlayerCount(nrOfPlayers);
    slotNewPlayer();
    
    // Restore player names
    int plrNum = 0;
    for( QListViewItem *item = w->listPlayers->firstChild(); 
         item; item = item->nextSibling(), plrNum++ )
    {
       QString key = QString("Player_%1").arg(plrNum);
       
       QString playerName = config->readEntry(key);
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
       
    QListViewItem *item;
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
    QColor PlayerColors[MAX_PLAYERS] = { QColor( 130, 130, 255 ), yellow, red, green,
    		white, cyan, magenta, QColor( 235, 153, 46 ),
		QColor( 106, 157, 104 ),  QColor( 131, 153, 128) };
       
    int i = 0;
    QListViewItem *lastItem = 0;
    QListViewItem *item = 0;
    QListViewItem *nextItem = w->listPlayers->firstChild();
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
       QString playerName = i18n("Generated AI player name", "Comp%1").arg(i+1);
       QPixmap pm(16,16);
       QColor color(PlayerColors[i]);
       pm.fill(color);
       QListViewItem *item = new QListViewItem(w->listPlayers, lastItem, playerName, i18n("Computer Player"), "A", color.name());
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
    return w->sliderTurns->value();
}

void
NewGameDlg::updateLabels()
{
    w->labelPlayers->setText(i18n("Number of &players: %1").arg(w->sliderPlayers->value()));
    w->labelPlanets->setText(i18n("Number of neutral p&lanets: %1").arg(w->sliderPlanets->value()));
    w->labelTurns->setText(i18n("Number of &turns: %1").arg(w->sliderTurns->value()));
}

void
NewGameDlg::slotOk()
{
    bool hasHumans = false;
    for( QListViewItem *item = w->listPlayers->firstChild(); 
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
    KDialogBase::slotOk();
}

void
NewGameDlg::save()
{
    KConfig *config = kapp->config();
    config->setGroup("Game");
    
    config->writeEntry("NrOfPlayers", w->sliderPlayers->value());
    config->writeEntry("NrOfPlanets", w->sliderPlanets->value());
    config->writeEntry("NrOfTurns", w->sliderTurns->value());

    int plrNum = 0;
    for( QListViewItem *item = w->listPlayers->firstChild(); 
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

    Planet *planet;
    planet = plnetList->first();
    for( planet = plnetList->take(); planet != 0; planet = plnetList->take() ) {
        delete planet;
    }

    Player *player;
    player = plrList->first();
    for( player = plrList->take(); player != 0; player = plrList->take() ) {
        delete player;
    }

    // Make player list
    // Does the name already exist in the list
    int plrNum = 0;
    for( QListViewItem *item = w->listPlayers->firstChild(); 
         item; item = item->nextSibling() )
    {
        QString playerName = item->text(0);
        bool ai = (item->text(2) == "A");
        QColor color(item->text(3));
        plrList->append( Player::createPlayer( playerName, color, plrNum, ai ));
        plrNum++;
    }

    // make the planets
    map->populateMap( *plrList, neutral,
                      w->sliderPlanets->value(),
                      *plnetList );
}

