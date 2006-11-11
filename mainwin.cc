#include <config.h>

#include <QPushButton>
#include <QLabel>

#include <kapplication.h>
#include <klocale.h>
#include <kglobal.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kstdgameaction.h>
#include <kicon.h>
#include <kstatusbar.h>

#include "version.h"
#include "mainwin.h"
#include "mainwin.moc"
#include "gameboard.h"

// KonquestMainWindow


MainWindow::MainWindow()
{
    setCaption( i18n("Galactic Conquest") );

    setupGameBoard();
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
    KStdGameAction::gameNew( m_gameBoard, SLOT( startNewGame() ),
			     actionCollection() );
    KStdGameAction::quit( this, SLOT( close() ), actionCollection() );

    m_endAction = KStdGameAction::end( m_gameBoard, SLOT( shutdownGame() ), 
				       actionCollection() );
    m_endAction->setEnabled(false);

    //AB: there is no icon for disabled - KToolBar::insertButton shows the
    //different state - KAction not :-(
    m_measureAction = new KAction(KIcon("ruler"),  i18n("&Measure Distance"), 
				  actionCollection(), "game_measure" );
    connect(m_measureAction, SIGNAL(triggered(bool)),
	    m_gameBoard,     SLOT( measureDistance() ));
    m_measureAction->setEnabled(false);

    // Show standings
    m_standingAction = new KAction(KIcon("help"), i18n("&Show Standings"),
				   actionCollection(), "game_scores" );
    connect(m_standingAction, SIGNAL(triggered(bool)),
	    m_gameBoard,      SLOT( showScores() ));
    m_standingAction->setEnabled(false);

    // Show fleet overview
    m_fleetAction = new KAction(KIcon("launch"),  i18n("&Fleet Overview"), 
				actionCollection(), "game_fleets" );
    connect(m_fleetAction, SIGNAL(triggered(bool)),
	    m_gameBoard,   SLOT( showFleets() ));
    m_fleetAction->setEnabled(false);

    // Toolbar
    addToolBar ( Qt::LeftToolBarArea, toolBar() );
    toolBar()->setMovable(false);
}


void
MainWindow::setupGameBoard()
{
    m_gameBoard = new GameBoard( this );
    setCentralWidget(m_gameBoard);

    connect( m_gameBoard, SIGNAL( newGameState( GameState )),
	     this,        SLOT( gameStateChange( GameState ) ) );
}


void
MainWindow::gameStateChange( GameState newState )
{
    m_endAction     ->setEnabled( m_gameBoard->isGameInProgress() );
    m_measureAction ->setEnabled( newState==SOURCE_PLANET );
    m_standingAction->setEnabled( newState==SOURCE_PLANET );
    m_fleetAction   ->setEnabled( newState==SOURCE_PLANET );

    m_statusBarText->setText(i18n("Turn # %1", m_gameBoard->turnNumber()));
}
