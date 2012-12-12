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

    resize(600, 650);
}

MainWindow::~MainWindow()
{
}


void
MainWindow::setupActions()
{
    KStandardGameAction::gameNew( this, SLOT( startNewGame() ), actionCollection() );
    KStandardGameAction::quit( this, SLOT( close() ), actionCollection() );

    m_endAction = KStandardGameAction::end( this, NULL, actionCollection() );
    m_endAction->setEnabled(false);

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
}


void
MainWindow::setupGameView()
{
    m_game      = new LocalGame( this );
    m_gameView  = new GameView( this, m_game );
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
    connect(m_endAction,      SIGNAL(triggered()),     m_gameView, SLOT(shutdownGame()));
}

void
MainWindow::startNewGame()
{
    m_gameView->deleteLater();
    m_game->deleteLater();
    setupGameView();
    m_gameView->startNewGame();
}

void
MainWindow::guiStateChange( GUIState newState )
{
    if (newState == NONE)
    {
        m_gameView->deleteLater();
        m_game->deleteLater();
        this->setupGameView();
    }
    m_endAction     ->setEnabled( m_game->isRunning() );
    m_measureAction ->setEnabled( newState == SOURCE_PLANET );
    m_standingAction->setEnabled( newState == SOURCE_PLANET );
    m_fleetAction   ->setEnabled( newState == SOURCE_PLANET );

    m_statusBarText->setText(i18n("Turn # %1", m_game->turnCounter()));
}
