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

#include <kconfig.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <KStandardGuiItem>
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
      m_map(pmap),
      m_players(players),
      m_planets(planets),
      m_neutral(neutralPlayer)
{
    setCaption(i18n("Start New Game"));
    setButtons(KDialog::Ok|KDialog::Default|KDialog::Cancel);
    setDefaultButton(KDialog::NoDefault);
    showButtonSeparator(true);

    m_w = new NewGameDlgUI(this);
    m_w->map->setMap( m_map );
    m_w->listPlayers->header()->hide();
//    w->listPlayers->setMinimumSize( 100, 150 );
    m_w->listPlayers->setSortColumn(-1);
    m_w->listPlayers->setHScrollBarMode(Q3ScrollView::AlwaysOff);
    m_w->sliderPlayers->setMinimumWidth(270);
    m_w->sliderPlanets->setMinimumWidth(270);

    m_w->newPlayer->setMaxLength( 8 );

    connect(m_w->sliderPlayers, SIGNAL(valueChanged(int)),
	    this,               SLOT(slotPlayerCount(int)));
    connect(m_w->sliderPlanets, SIGNAL(valueChanged(int)), 
	    this,               SLOT(slotNewMap()));
    connect(m_w->rejectMap,     SIGNAL(clicked()),
	    this,               SLOT(slotNewMap()));
    connect(m_w->newPlayer,     SIGNAL(textChanged(const QString &)),
	    this,               SLOT(slotNewPlayer()));
    connect(m_w->newPlayer,     SIGNAL(returnPressed()), 
	    this,               SLOT(slotAddPlayer()));
    connect(m_w->addPlayer,     SIGNAL(clicked()),
	    this,               SLOT(slotAddPlayer()));

    init();

    setMainWidget(m_w);
}


void
NewGameDlg::slotDefault()
{
    m_w->sliderPlayers->setValue(2);
    m_w->sliderPlanets->setValue(3);

    m_w->listPlayers->clear();

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
       
    m_w->sliderPlayers->setValue(nrOfPlayers);
    m_w->sliderPlanets->setValue(nrOfPlanets);
    setPlayerCount(nrOfPlayers);
    slotNewPlayer();
    
    // Restore player names
    int plrNum = 0;
    for( Q3ListViewItem *item = m_w->listPlayers->firstChild(); 
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
    m_w->addPlayer->setEnabled(!m_w->newPlayer->text().isEmpty());
}

void
NewGameDlg::slotAddPlayer()
{
    QString playerName = m_w->newPlayer->text();
    if (playerName.isEmpty())
       return;
       
    Q3ListViewItem *item;
    do {
       item = m_w->listPlayers->firstChild();
       while( item ) {
          if (item->text(2) == "A")
             break;

          item = item->nextSibling();
       }
       if (!item) {
          int nrPlayers = m_w->listPlayers->childCount();
          if (nrPlayers >= MAX_PLAYERS)
             return; // Too bad

          nrPlayers++;
          m_w->sliderPlayers->setValue(nrPlayers);
          setPlayerCount(nrPlayers);
       }
    } while(!item);
    
    item->setText(2, "H"); // Human
    item->setText(1, i18n("Human Player"));
    item->setText(0, playerName);
    
    m_w->newPlayer->setText(QString::null);
    
    updateMiniMap();
    updateLabels();
}


void
NewGameDlg::setPlayerCount(int playerCount)
{
    QColor PlayerColors[MAX_PLAYERS] = { 
	QColor( 130, 130, 255 ),
	Qt::yellow,
	Qt::red,
	Qt::green,
	Qt::white,
	Qt::cyan,
	Qt::magenta,
	QColor( 235, 153, 46 ),
	QColor( 106, 157, 104 ),
	QColor( 131, 153, 128) 
    };
       
    int              i = 0;
    Q3ListViewItem  *lastItem = 0;
    Q3ListViewItem  *item = 0;
    Q3ListViewItem  *nextItem = m_w->listPlayers->firstChild();
    while( (item = nextItem) ) {
       nextItem = item->nextSibling();
       if (i >= playerCount) {
          delete item;
       }
       else {
          lastItem = item;
       }
       i++;
    }
    
    while (m_w->listPlayers->childCount() < playerCount) {
       QString  playerName = i18nc("Generated AI player name", "Comp%1", i+1);
       QPixmap  pm(16,16);
       QColor   color(PlayerColors[i]);

       pm.fill(color);
       Q3ListViewItem *item = new Q3ListViewItem(m_w->listPlayers,
						 lastItem, playerName,
						 i18n("Computer Player"),
						 "A", color.name());
       item->setPixmap(0, pm);
       lastItem = item;
       i++;
    }
}

void
NewGameDlg::slotPlayerCount(int playerCount)
{
    if (m_w->listPlayers->childCount() == playerCount)
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


void
NewGameDlg::updateLabels()
{
    m_w->labelPlayers->setText(i18n("Number of &players: %1", 
				    m_w->sliderPlayers->value()));
    m_w->labelPlanets->setText(i18n("Number of neutral p&lanets: %1",
				    m_w->sliderPlanets->value()));
}

void
NewGameDlg::slotOk()
{
    bool hasHumans = false;

    for( Q3ListViewItem *item = m_w->listPlayers->firstChild(); 
         item; item = item->nextSibling() )
    {
        bool ai = (item->text(2) == "A");
        if (!ai)
           hasHumans = true;
    }

    if (!hasHumans) {
        KMessageBox::information(this,
              i18n("The game is much more fun when you add a human player!"));
        m_w->newPlayer->setFocus();
        return;
    }

    KDialog::accept();
}

void
NewGameDlg::save()
{
    KConfig *config = KGlobal::config();
    config->setGroup("Game");
    
    config->writeEntry("NrOfPlayers", m_w->sliderPlayers->value());
    config->writeEntry("NrOfPlanets", m_w->sliderPlanets->value());

    int plrNum = 0;
    for( Q3ListViewItem *item = m_w->listPlayers->firstChild(); 
         item; item = item->nextSibling() )
    {
        QString  key        = QString("Player_%1").arg(plrNum);
        QString  playerName = item->text(0);

        bool ai = (item->text(2) == "A");
        if (ai) {
           if (config->hasKey(key))
              config->deleteEntry(key);
        }
        else {
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
    m_map->clearMap();

    while (!m_planets->isEmpty())
        delete m_planets->takeFirst();

    while (!m_players->isEmpty())
        delete m_players->takeFirst();

    // Make player list
    // Does the name already exist in the list
    int plrNum = 0;
    for( Q3ListViewItem *item = m_w->listPlayers->firstChild(); 
         item; item = item->nextSibling() )
    {
        QString  playerName = item->text(0);
        bool     ai         = (item->text(2) == "A");
        QColor   color(item->text(3));

        m_players->append( Player::createPlayer( m_map, playerName, color, 
						 plrNum, ai ));
        plrNum++;
    }

    // make the planets
    m_map->populateMap( *m_players, m_neutral,
			m_w->sliderPlanets->value(),
			*m_planets );
}

