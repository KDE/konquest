/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "mainwin.h"

#include <QDockWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

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

#include "game.h"
#include "localgame.h"
#include "gameview.h"


// KonquestMainWindow
MainWindow::MainWindow()
{
    setCaption( i18n("Galactic Conquest") );

    setupActions();
    setupGameView();
    setupGUI();

    // The status bar.
    m_statusBarText = new QLabel(i18n("Galactic Conquest"));
    statusBar()->addWidget(m_statusBarText);
}


MainWindow::~MainWindow()
{
}


QSize
MainWindow::sizeHint() const
{
    return KXmlGuiWindow::sizeHint().expandedTo(QSize(600, 650));
}


void
MainWindow::setupActions()
{
    KStandardGameAction::gameNew( this, SLOT( startNewGame() ), actionCollection() );
    KStandardGameAction::quit( this, SLOT( close() ), actionCollection() );

    m_endTurnAction = KStandardGameAction::endTurn(this, NULL, actionCollection());
    m_endTurnAction->setShortcut(Qt::CTRL + Qt::Key_E);
    m_endTurnAction->setEnabled(false);

    m_endGameAction = KStandardGameAction::end( this, NULL, actionCollection() );
    m_endGameAction->setEnabled(false);

    //AB: there is no icon for disabled - KToolBar::insertButton shows the
    //different state - KAction not :-(
    m_measureAction = actionCollection()->addAction( QLatin1String(  "game_measure" ) );
    m_measureAction->setIcon( KIcon( QLatin1String( "go-jump" )) );
    m_measureAction->setText( i18n("&Measure Distance") );
    m_measureAction->setEnabled(false);

    // Show standings
    m_standingAction = actionCollection()->addAction( QLatin1String(  "game_scores" ) );
    m_standingAction->setIcon( KIcon( QLatin1String( "help-contents" )) );
    m_standingAction->setText( i18n("&Show Standings") );
    m_standingAction->setEnabled(false);

    // Show fleet overview
    m_fleetAction = actionCollection()->addAction( QLatin1String(  "game_fleets" ) );
    m_fleetAction->setIcon( KIcon( QLatin1String( "fork" )) );
    m_fleetAction->setText( i18n("&Fleet Overview") );
    m_fleetAction->setEnabled(false);

    // Toolbar
    addToolBar ( Qt::LeftToolBarArea, toolBar() );
    toolBar()->setMovable(false);

    // docking area - messages

    m_messagesDock = new QDockWidget(i18n("Messages"), this);
    m_messagesDock->setObjectName("dock-messages");

    addDockWidget(Qt::BottomDockWidgetArea, m_messagesDock);

    m_messagesAction = actionCollection()->addAction(QLatin1String("view_messages"));
    m_messagesAction->setText(i18n("Show &Messages"));
    m_messagesAction->setCheckable(true);
    m_messagesAction->setChecked(m_messagesDock->isVisible());

    // The dock signal "visibilityChanged" is fired if the dock is shown or
    // hidden. But this includes hidden in a tab as well. The action should not
    // represent the visibility state, but if the dock is present somewhere in
    // the GUI, regardless of currently visible in an active tab or invisible
    // in a not currently active tab.

    connect(m_messagesAction, SIGNAL(triggered(bool)), m_messagesDock, SLOT(setVisible(bool)));
    connect(m_messagesDock, SIGNAL(visibilityChanged(bool)), this, SLOT(updateMessagesActionSlot()));
}


void
MainWindow::setupGameView()
{
    m_game      = new LocalGame( this );
    m_gameView  = new GameView(this, m_game, m_messagesDock);
    setCentralWidget( m_gameView );

    connect ( m_game,    SIGNAL( gameMsg(const KLocalizedString &,
					   Player *, Planet *,
					   Player * ) ),
	      m_gameView,  SLOT( gameMsg(const KLocalizedString &,
					 Player *, Planet *,
					 Player * ) ) );
    connect (m_gameView, SIGNAL( newGUIState( GUIState )),
	     this,       SLOT( guiStateChange( GUIState ) ) );

    connect(m_measureAction,  SIGNAL(triggered(bool)), m_gameView, SLOT( measureDistance() ));
    connect(m_standingAction, SIGNAL(triggered(bool)), m_gameView, SLOT( showScores() ));
    connect(m_fleetAction,    SIGNAL(triggered(bool)), m_gameView, SLOT( showFleets() ));
    connect(m_endTurnAction,  SIGNAL(triggered()),     m_gameView, SLOT(nextPlayer()));
    connect(m_endGameAction,  SIGNAL(triggered()),     m_gameView, SLOT(shutdownGame()));
}


void
MainWindow::setupGUI()
{
    KXmlGuiWindow::setupGUI();

    /**
     * @todo The docks should not be visible on the main screen, and neither
     * should it be possible to open the docks. During the game and later on,
     * this is handled by GameView::changeGameView() and by
     * MainWindow::guiStateChange(). Just the initial setup does not work that
     * way. - Rework the GUI setup sequence so that the following hack is not
     * required.
     */

    m_messagesAction->setEnabled(false);

    m_messagesDock->toggleViewAction()->setEnabled(false);

    m_messagesDock->hide();
}


void
MainWindow::startNewGame()
{
    if (m_gameView->confirmNewGame())
    {
        m_gameView->deleteLater();
        m_game->deleteLater();
        setupGameView();
        m_gameView->startNewGame();
    }
}

void
MainWindow::guiStateChange( GUIState newState )
{
    if (newState == NONE) {
        m_gameView->deleteLater();
        m_game->deleteLater();
        this->setupGameView();
    }

    // An alternative to disabling the "end turn" action during "send fleet
    // command sequence" is to simply abort this sequence if the user decides
    // to "end turn" before completion.

    /**
     * @todo The game view handles the state of the actions, so the game view
     * should be able to update the enabled state of the actions as well. This
     * should be implemented via signals, instead of copying the conditions here
     * again.
     */

    m_endTurnAction ->setEnabled( m_game->isRunning() && (newState == SOURCE_PLANET) );
    m_endGameAction ->setEnabled( m_game->isRunning() );
    m_measureAction ->setEnabled( newState == SOURCE_PLANET );
    m_standingAction->setEnabled( newState == SOURCE_PLANET );
    m_fleetAction   ->setEnabled( newState == SOURCE_PLANET );

    m_messagesAction->setEnabled(m_game->isRunning());

    m_messagesDock->toggleViewAction()->setEnabled(m_game->isRunning());

    m_statusBarText->setText(i18n("Turn # %1", m_game->turnCounter()));
}


void
MainWindow::updateMessagesActionSlot()
{
    m_messagesAction->setChecked(m_messagesDock->toggleViewAction()->isChecked());
}
