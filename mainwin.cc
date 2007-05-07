#include "mainwin.h"

#include <QPushButton>
#include <QLabel>

#include <klocale.h>
#include <kglobal.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <kstandardgameaction.h>
#include <kicon.h>
#include <kstatusbar.h>

#include "mainwin.moc"
#include "gamelogic.h"
#include "gameview.h"


// KonquestMainWindow


MainWindow::MainWindow()
{
    setCaption( i18n("Galactic Conquest") );

    setupGameView();
    setupActions();
    setupGUI();

    // The status bar.
    m_statusBarText = new QLabel(i18n("Galactic Conquest"));
    statusBar()->addWidget(m_statusBarText);

    resize(600, 650);
}

MainWindow::~MainWindow()
{
}


void
MainWindow::setupActions()
{
    QAction *action;

    action = KStandardGameAction::gameNew( m_gameView, SLOT( startNewGame() ), this );
    actionCollection()->addAction( action->objectName(), action );
    action = KStandardGameAction::quit( this, SLOT( close() ), this );
    actionCollection()->addAction( action->objectName(), action );

    m_endAction = KStandardGameAction::end( m_gameView, SLOT( shutdownGame() ), this );
    actionCollection()->addAction( m_endAction->objectName(), m_endAction );
    m_endAction->setEnabled(false);

    //AB: there is no icon for disabled - KToolBar::insertButton shows the
    //different state - KAction not :-(
    m_measureAction = actionCollection()->addAction( "game_measure" );
    m_measureAction->setIcon( KIcon("ruler") );
    m_measureAction->setText( i18n("&Measure Distance") );
    connect(m_measureAction, SIGNAL(triggered(bool)),
	    m_gameView,      SLOT( measureDistance() ));
    m_measureAction->setEnabled(false);

    // Show standings
    m_standingAction = actionCollection()->addAction( "game_scores" );
    m_standingAction->setIcon( KIcon("help-contents") );
    m_standingAction->setText( i18n("&Show Standings") );
    connect(m_standingAction, SIGNAL(triggered(bool)),
	    m_gameView,       SLOT( showScores() ));
    m_standingAction->setEnabled(false);

    // Show fleet overview
    m_fleetAction = actionCollection()->addAction( "game_fleets" );
    m_fleetAction->setIcon( KIcon("launch") );
    m_fleetAction->setText( i18n("&Fleet Overview") );
    connect(m_fleetAction, SIGNAL(triggered(bool)),
	    m_gameView,    SLOT( showFleets() ));
    m_fleetAction->setEnabled(false);

    // Toolbar
    addToolBar ( Qt::LeftToolBarArea, toolBar() );
    toolBar()->setMovable(false);
}


void
MainWindow::setupGameView()
{
    m_gameLogic = new GameLogic( this );
    m_gameView  = new GameView( this, m_gameLogic );
    setCentralWidget( m_gameView );

    connect ( m_gameLogic, SIGNAL( gameMsg(const KLocalizedString &,
					   Player *, Planet *,
					   Player * ) ),
	      m_gameView,  SLOT( gameMsg(const KLocalizedString &,
					 Player *, Planet *,
					 Player * ) ) );
    connect (m_gameLogic, SIGNAL( beginTurn()), m_gameView, SLOT(beginTurn()));
    connect (m_gameLogic, SIGNAL( endTurn()), m_gameView, SLOT(endTurn()));
    connect( m_gameView, SIGNAL( newGUIState( GUIState )),
	     this,       SLOT( guiStateChange( GUIState ) ) );
}


void
MainWindow::guiStateChange( GUIState newState )
{
    m_endAction     ->setEnabled( m_gameView->isGameInProgress() );
    m_measureAction ->setEnabled( newState == SOURCE_PLANET );
    m_standingAction->setEnabled( newState == SOURCE_PLANET );
    m_fleetAction   ->setEnabled( newState == SOURCE_PLANET );

    m_statusBarText->setText(i18n("Turn # %1", m_gameLogic->turnNumber()));
}
