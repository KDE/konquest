/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwin.h"

#include <QAction>
#include <QDockWidget>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>

#include <KActionCollection>
#include <KStandardGameAction>
#include <KLocalizedString>
#include <KToolBar>

#include "game.h"
#include "localgame.h"
#include "gameview.h"


// KonquestMainWindow
MainWindow::MainWindow()
{
    setWindowTitle( i18n("Galactic Conquest") );

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
    KStandardGameAction::gameNew(this, &MainWindow::startNewGame, actionCollection());
    KStandardGameAction::quit(this, &MainWindow::close, actionCollection());

    m_endTurnAction = KStandardGameAction::endTurn(this, nullptr, actionCollection());
    m_endTurnAction->setShortcut(Qt::CTRL | Qt::Key_E);
    m_endTurnAction->setEnabled(false);

    m_endGameAction = KStandardGameAction::end( this, nullptr, actionCollection() );
    m_endGameAction->setEnabled(false);

    //AB: there is no icon for disabled - KToolBar::insertButton shows the
    //different state - QAction not :-(
    m_measureAction = actionCollection()->addAction( QStringLiteral(  "game_measure" ) );
    m_measureAction->setIcon( QIcon::fromTheme( QStringLiteral( "go-jump" )) );
    m_measureAction->setText( i18n("&Measure Distance") );
    m_measureAction->setEnabled(false);

    // Show fleet overview
    m_fleetAction = actionCollection()->addAction( QStringLiteral(  "game_fleets" ) );
    m_fleetAction->setIcon( QIcon::fromTheme( QStringLiteral( "fork" )) );
    m_fleetAction->setText( i18n("&Fleet Overview") );
    m_fleetAction->setEnabled(false);

    // Toolbar
    addToolBar ( Qt::LeftToolBarArea, toolBar() );
    toolBar()->setMovable(false);

    // docking area - messages

    m_messagesDock = new QDockWidget(i18n("Messages"), this);
    m_messagesDock->setObjectName(QStringLiteral("dock-messages"));

    addDockWidget(Qt::BottomDockWidgetArea, m_messagesDock);

    m_messagesAction = actionCollection()->addAction(QStringLiteral("view_messages"));
    m_messagesAction->setText(i18n("Show &Messages"));
    m_messagesAction->setCheckable(true);
    m_messagesAction->setChecked(m_messagesDock->isVisible());

    // The action signal "toggled" is fired even in case the state is changed
    // via code using setChecked(). "triggered" however is only fired if the
    // user actually triggered the change.

    // The dock signal "visibilityChanged" is fired if the dock is shown or
    // hidden. But this includes hidden in a tab as well. The action should not
    // represent the visibility state, but if the dock is present somewhere in
    // the GUI, regardless of currently visible in an active tab or invisible
    // in a not currently active tab.

    connect(m_messagesAction, &QAction::triggered, m_messagesDock, &QDockWidget::setVisible);
    connect(m_messagesDock, &QDockWidget::visibilityChanged, this, &MainWindow::updateMessagesActionSlot);

    // docking area - standings

    m_standingsDock = new QDockWidget(i18n("Standings"), this);
    m_standingsDock->setObjectName(QStringLiteral("dock-standings"));

    tabifyDockWidget(m_messagesDock, m_standingsDock);

    m_standingsAction = actionCollection()->addAction(QStringLiteral("view_standings"));
    m_standingsAction->setIcon(QIcon::fromTheme(QStringLiteral("help-contents")));
    m_standingsAction->setText(i18n("Show &Standings"));
    m_standingsAction->setCheckable(true);
    m_standingsAction->setChecked(m_standingsDock->isVisible());

    connect(m_standingsAction, &QAction::triggered, m_standingsDock, &QDockWidget::setVisible);
    connect(m_standingsDock, &QDockWidget::visibilityChanged, this, &MainWindow::updateStandingsActionSlot);
}


void
MainWindow::setupGameView()
{
    m_game      = new LocalGame( this );
    m_gameView  = new GameView(this, m_game, m_messagesDock, m_standingsDock);
    setCentralWidget( m_gameView );

    connect(m_game, &Game::gameMsg, m_gameView, &GameView::gameMsg);
    connect(m_gameView, &GameView::newGUIState, this, &MainWindow::guiStateChange);

    connect(m_measureAction, &QAction::triggered, m_gameView, &GameView::measureDistance);
    connect(m_fleetAction, &QAction::triggered, m_gameView, &GameView::showFleets);
    connect(m_endTurnAction, &QAction::triggered, m_gameView, &GameView::nextPlayer);
    connect(m_endGameAction, &QAction::triggered, m_gameView, &GameView::shutdownGame);
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
    m_standingsAction->setEnabled(false);

    m_messagesDock->toggleViewAction()->setEnabled(false);
    m_standingsDock->toggleViewAction()->setEnabled(false);

    m_messagesDock->hide();
    m_standingsDock->hide();
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
    m_fleetAction   ->setEnabled( newState == SOURCE_PLANET );

    m_messagesAction->setEnabled(m_game->isRunning());
    m_standingsAction->setEnabled(m_game->isRunning());

    m_messagesDock->toggleViewAction()->setEnabled(m_game->isRunning());
    m_standingsDock->toggleViewAction()->setEnabled(m_game->isRunning());

    m_statusBarText->setText(i18n("Turn # %1", m_game->turnCounter()));
}


void
MainWindow::updateMessagesActionSlot()
{
    m_messagesAction->setChecked(m_messagesDock->toggleViewAction()->isChecked());
}


void
MainWindow::updateStandingsActionSlot()
{
    m_standingsAction->setChecked(m_standingsDock->toggleViewAction()->isChecked());
}
