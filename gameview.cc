/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>
    Copyright 2011 Jeffrey Kelling <overlordapophis@gmail.com>

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

#include "gameview.h"

#include <QCheckBox>
#include <QDockWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QPixmap>
#include <QKeyEvent>
#include <QIntValidator>
#include <KDebug>

#include <kmessagebox.h>

#include "players/player.h"
#include "players/localplayer.h"
#include "images.h"
#include "map/mapview.h"
#include "map/mapscene.h"

#include "dialogs/newgamedlg.h"
#include "dialogs/scoredlg.h"
#include "dialogs/fleetdlg.h"

#include "view/standingswidget.h"

#include <cmath>

/*********************************************************************
 Game Board
*********************************************************************/

GameView::GameView(QWidget *parent, Game *game, QDockWidget *messagesDock, QDockWidget *standingsDock)
  : QWidget( parent ),
    m_messagesDock(messagesDock),
    m_standingsDock(standingsDock),
    m_game( game ),
    m_queueMessages(false),
    m_messageQueue(), 
    m_showInformations(false),
    m_initCompleted( false ),
    m_cleanupNeeded( false ),
    m_guiState( NONE )
{
    QPalette blackPal;
    blackPal.setColor( backgroundRole(), Qt::black );
    setPalette( blackPal );
    setAutoFillBackground( true );

    QColor col(Qt::green);
    QPalette palette;
    palette.setColorGroup( QPalette::Active,    Qt::white,          Qt::black,
                           col.lighter(),       col.darker(),       col,
                           col.lighter(125),    col.lighter(125),   col.darker(),
                           Qt::black );
    palette.setColorGroup( QPalette::Inactive,  Qt::white,          Qt::black,
                           col.lighter(),       col.darker(),       col,
                           col.lighter(125),    col.lighter(125),   col.darker(),
                           Qt::black );
    palette.setColorGroup( QPalette::Disabled,  Qt::white,          QColor(Qt::darkGray).darker(),
                           col.lighter(),       col.darker(),       col,
                           col.darker(150),     col.lighter(125),   Qt::black,
                           Qt::black );

    blackPal.setColor( QPalette::Base, Qt::black );
    blackPal.setColor( QPalette::Window, Qt::black );
    blackPal.setColor( QPalette::Button, QColor(Qt::darkGray).darker() );
    blackPal.setColor( QPalette::Text, Qt::white );
    blackPal.setColor( QPalette::ButtonText, Qt::white );
    blackPal.setColor( QPalette::WindowText, Qt::white );

    //********************************************************************
    // Create the widgets in the main window
    //********************************************************************
    m_mapScene  = new MapScene(m_game);
    m_mapWidget = new MapView( m_mapScene );
    m_mapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_mapWidget->setFrameShape(QFrame::NoFrame);

    m_msgWidget = new QTextEdit();
    m_msgWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_msgWidget->setReadOnly(true);
    m_msgWidget->setPalette( blackPal );
    m_msgWidget->setAutoFillBackground( true );

    m_messagesDock->setWidget(m_msgWidget);

    m_standingsWidget = new StandingsWidget(0);
    m_standingsDock->setWidget(m_standingsWidget);

    m_gameMessage = new QLabel( this );
    m_gameMessage->setPalette( palette );

    m_endTurnBtn = new QPushButton( i18n("End Turn"), this );
    m_endTurnBtn->setFixedSize( m_endTurnBtn->sizeHint() );
    m_endTurnBtn->setPalette( palette );

    m_shipCountEdit = new QLineEdit( this );
    m_shipValidator = new QIntValidator(1, 32767, this );
    m_shipCountEdit->setValidator( m_shipValidator );
    m_shipCountEdit->setMinimumSize( 40, 0 );
    m_shipCountEdit->setMaximumSize( 32767, 40 );
    m_shipCountEdit->setEnabled(false);
    m_shipCountEdit->setPalette( palette );
    m_shipCountEdit->setEchoMode( QLineEdit::Password );

    m_standingOrder = new QCheckBox(i18n("Standing order"), this);
    m_standingOrder->setEnabled(false);
    m_standingOrder->setPalette( palette );
    m_standingOrder->setCheckState(Qt::Unchecked);

    m_splashScreen = new QLabel( this );
    m_splashScreen->setPixmap(QPixmap(IMAGE_SPLASH));
    m_splashScreen->setScaledContents(true);

    setMouseTracking( true );
    setFocusPolicy( Qt::StrongFocus );
    setFocus();

    //********************************************************************
    // Layout the main window
    //********************************************************************
    QVBoxLayout  *mainLayout    = new QVBoxLayout( this );
    QHBoxLayout  *topLineLayout = new QHBoxLayout;

    topLineLayout->addSpacing( 5 );
    topLineLayout->addWidget( m_gameMessage, 10 );
    topLineLayout->addWidget( m_standingOrder, 1 );
    topLineLayout->addWidget( m_shipCountEdit, 1 );
    topLineLayout->addWidget( m_endTurnBtn, 1 );

    mainLayout->addLayout( topLineLayout );
    mainLayout->addWidget( m_mapWidget );

    //**********************************************************************
    // Set up signal/slot connections
    //**********************************************************************
    connect( m_mapScene,      SIGNAL( planetSelected(Planet *) ),
             this,            SLOT(planetSelected(Planet *)) );
    connect( m_shipCountEdit, SIGNAL(returnPressed()),
             this,            SLOT(newShipCount()) );
    connect( m_standingOrder, SIGNAL(clicked()),
             this,            SLOT(standingOrdersClicked()) );
    connect( m_endTurnBtn,    SIGNAL( clicked() ),
             this,            SLOT( nextPlayer() ) );

    changeGameView();
}

//**********************************************************************
// Destructor
//**********************************************************************
GameView::~GameView()
{
    // Nothing much to do yet
}

//************************************************************************
// Event handlers
//************************************************************************
void GameView::standingOrdersClicked() {
    m_shipCountEdit->setFocus();
    if(m_standingOrder->checkState() == Qt::Checked)
        m_shipValidator->setTop(INT_MAX);
    else
        m_shipValidator->setTop(sourcePlanet->fleet().shipCount());
}

void
GameView::resizeEvent ( QResizeEvent * ) {
    m_splashScreen->setGeometry( 0, 0, width(), height() );
}

void
GameView::keyPressEvent( QKeyEvent *e )
{
    // Check for the escape key
    if( e->key() == Qt::Key_Escape ) {
        switch( m_guiState ) {
        case DEST_PLANET:
        case SHIP_COUNT:
        case RULER_SOURCE:
        case RULER_DEST:
            m_guiState = SOURCE_PLANET;
            haveSourcePlanet = false;
            haveDestPlanet   = false;
            turn();
            break;
        default:
            break;
        }
        m_mapScene->displayPlanetInfo( NULL );
        m_mapScene->unselectPlanet();
        m_showInformations = false;
        return;
    }

    if( e->text().isEmpty() || e->text().at(0).isSpace() ) {
        e->ignore();
        return;
    }
    
    if ( e->text() == "?" ) {
        // Switch the information key info...
        m_showInformations = !m_showInformations;
        return;
    }

    QString planetName;

    planetName += e->text().toUpper();
    
    foreach (Planet *p, m_game->planets()) {
        if( p->name() == planetName ) {
            if ( m_showInformations ) {
                m_mapScene->selectPlanet(p);
                m_mapScene->displayPlanetInfo( p );
                m_showInformations = false;
            } else {
                m_mapScene->displayPlanetInfo( NULL );
                m_mapScene->selectPlanet(p);
                planetSelected( p );
            }
            break;
        }
    }

}


//************************************************************************
// Game engine/state machine
//************************************************************************

/**
 * Prepare the turn for a local player by updating the informational widgets.
 */

void
GameView::turnPreparation()
{
    m_standingsWidget->update(m_game->players());

    turn();
}


void
GameView::turn()
{
    switch( m_guiState ) {
    case NONE :
        // The standby state, waiting for clicking on a planet or starting
        // to measure a distance..
        m_guiState      = SOURCE_PLANET;
        haveSourcePlanet = false;
        haveDestPlanet   = false;
        haveShipCount    = false;
        standingOrder    = false;
        shipCount        = 0;
        m_mapScene->unselectPlanet();

        turn();
        setFocus();
        break;

    case SOURCE_PLANET :
        // The user has clicked on a source planet for moving some fleets.
        if( haveSourcePlanet ) {
            m_guiState = DEST_PLANET;

            m_mapScene->selectPlanet(sourcePlanet);
            turn();

        } else {
            m_shipCountEdit->setEnabled(false);
            m_shipCountEdit->setText( QString() );
            m_standingOrder->setEnabled(false);
            m_standingOrder->setCheckState(Qt::Unchecked);
            m_mapScene->unselectPlanet();
            m_gameMessage->setText( i18n("<qt>%1: Select source planet...</qt>", m_game->currentPlayer()->coloredName()) );
            setFocus();
        }

        break;

    case DEST_PLANET :
        // The user has chosen a destination planet and should now
        // specify a number of ships.
        if( haveDestPlanet ) {
            m_mapScene->unselectPlanet();
            m_guiState = SHIP_COUNT;
            turn();
        } else {
            m_shipCountEdit->setEnabled(false);
            m_standingOrder->setEnabled(false);
            m_mapScene->selectPlanet(sourcePlanet);
            m_gameMessage->setText( i18n("<qt>%1: Select destination planet...</qt>", m_game->currentPlayer()->coloredName()) );
            setFocus();
        }

        break;

    case SHIP_COUNT:
        // The user has selected, source, distance, ship count.
        if( haveShipCount ) {
            // We now have a complete fleet to send, so send it
            if( !m_game->attack(sourcePlanet, destPlanet, shipCount, standingOrder) ) {
                KMessageBox::error( this, i18n("Not enough ships to send.") );
            }

            m_shipCountEdit->setEnabled(false);
            m_standingOrder->setEnabled(false);

            m_guiState = NONE;
            turn();

            m_endTurnBtn->setFocus();

        } else {
            m_gameMessage->setText( i18n("%1: How many ships?", m_game->currentPlayer()->coloredName()) );

            m_shipCountEdit->setEnabled(true);
            m_standingOrder->setEnabled(true);
            m_shipCountEdit->setFocus();

            m_mapScene->unselectPlanet();
        }

        break;

    case RULER_SOURCE:
        // The user has selected to measure a distance with the ruler.
        if( haveSourcePlanet ) {
            m_guiState = RULER_DEST;
            m_mapScene->selectPlanet(sourcePlanet);
            turn();
        } else {
            m_shipCountEdit->setEnabled(false);
            m_mapScene->unselectPlanet();

            m_gameMessage->setText( i18n("Ruler: Select starting planet.") );
            setFocus();
        }
        break;

    case RULER_DEST:
        if( haveDestPlanet ) {
            m_mapScene->unselectPlanet();

            // Display the distance between the two planets
            double dist = m_game->map()->distance( sourcePlanet,
                                                        destPlanet );

            QString msg;
            msg = i18n("The distance from Planet %1 to Planet %2 is %3 light years.\n"
                       "A ship leaving this turn will arrive on turn %4",
                   sourcePlanet->name(),
                   destPlanet->name(),
                   QString::number(dist, 'f', 1),
                   m_game->turnCounter() + (int)std::ceil(dist));
            KMessageBox::information( this, msg, i18n("Distance"));

            m_guiState = NONE;
            turn();
        } else {
            m_gameMessage->setText( i18n("Ruler: Select ending planet.") );
            m_shipCountEdit->setEnabled(false);
            m_mapScene->selectPlanet(sourcePlanet);

            setFocus();
        }
        break;

    default:
        break;
    }

    m_endTurnBtn->setEnabled(m_guiState == SOURCE_PLANET);

    emit newGUIState( m_guiState );
}

void
GameView::gameMsg(const KLocalizedString &msg, Player *player, Planet *planet, 
        Player *planetPlayer)
{
    bool isHumanInvolved = false;

    QString           color    = "white";
    KLocalizedString  colorMsg = msg;
    KLocalizedString  plainMsg = msg;

    if (player) {
       if (!player->isAiPlayer())
          isHumanInvolved = true;
       colorMsg = colorMsg.subs(player->coloredName());
       plainMsg = plainMsg.subs(player->name());
    }

    if (planet) {
       if (!planetPlayer)
          planetPlayer = planet->player();
       if (!planetPlayer->isAiPlayer() && !planetPlayer->isNeutral())
          isHumanInvolved = true;

       QString  color = planetPlayer->color().name();
       colorMsg = colorMsg.subs(QString("<font color=\"%1\">%2</font>")
                                .arg(color, planet->name()));
       plainMsg = plainMsg.subs(planet->name());
    }

    m_msgWidget->append("<qt><font color=\"white\">"
                        + i18n("Turn %1:", m_game->turnCounter())
                        + "</font> <font color=\"" + color + "\">"
                        + colorMsg.toString()+"</font></qt>");
    m_msgWidget->moveCursor( QTextCursor::End );

    if (isHumanInvolved) {
        if ( m_queueMessages ) {
            GameMessage msg;

            msg.text     = colorMsg.toString();
            msg.sender   = player;
            msg.receiver = planetPlayer;
            m_messageQueue.append(msg);
        } else {
            KMessageBox::information(this, plainMsg.toString());
        }
    }
}

//************************************************************************
// Confirm aborting existing game
//************************************************************************
bool GameView::confirmNewGame()
{
  if( m_game->isRunning() )
        return shutdownGame();

  return true;
}

//************************************************************************
// Set up the game board for a new game
//************************************************************************
void
GameView::startNewGame()
{
    NewGameDlg *newGame = new NewGameDlg( this, m_game );

    if( !newGame->exec() ) {
        delete newGame;
        return;
    }

    newGame->save(); // Save settings for next time

    delete newGame;

    foreach (Player *player, m_game->players()) {
        if (player->isSpectator()) {

            // All planets a spectator player has assigned are turned into
            // neutral planets so that a spectator player starts without
            // any planet.

            m_game->map()->turnOverPlayerPlanets(player, m_game->neutral());
        }

        LocalPlayer *local = qobject_cast<LocalPlayer*>(player);
        if (local)
            connect(local, SIGNAL(canPlay()), this, SLOT(turnPreparation()));
    }

    connect(m_game, SIGNAL(finished()), this, SLOT(gameOver()));
    m_game->start();

    // Fix all the widgets to run a new game.
    changeGameView();

    // setup the map scene
    m_mapScene->mapUpdate();

    // Set up the base GUI for a new game.
    m_msgWidget->clear();
    m_shipCountEdit->setEnabled(false);
    m_initCompleted = true;

    //call GameView::gameOver now if needed happens if the game ends immediately after starting.
    if(m_cleanupNeeded)
        gameOver();
}


//************************************************************************
// Shut down the current game
//************************************************************************
bool
GameView::shutdownGame()
{
    if (!m_game->isRunning())
        return true;

    int choice = KMessageBox::warningContinueCancel
      ( this,
        i18n("Do you wish to retire this game?"),
        i18n("End Game"),
        KStandardGuiItem::ok() );

    if( choice == KMessageBox::Cancel )
        return false;

    gameOver();
    return true;
}


void
GameView::gameOver()
{
    if (m_initCompleted) {
        kDebug() << "Game over";

        /**
         * @todo This is an attempt to remove duplicate information from screen.
         * It is not a final solution, but only the best we came up with. The
         * problem is that the messages cannot be seen anymore, so the player
         * cannot check what happened last. Furthermore, this sudden change of
         * the GUI setup can be confusing for players.
         */

        m_messagesDock->hide();
        m_standingsDock->hide();

        ScoreDlg *scoreDlg = new ScoreDlg(this, i18n("Final Standings"), m_game->players());
        scoreDlg->exec();
        scoreDlg->deleteLater();

        cleanupGame();
    }
    else
        m_cleanupNeeded = true;
}


void
GameView::cleanupGame()
{
    m_mapScene->clearMap();
    m_game->stop();

    m_endTurnBtn->setEnabled( false );

    changeGameView();
    m_guiState = NONE;

    //m_game->cleanupGame();

    m_initCompleted = false;
    m_cleanupNeeded = false;
    emit newGUIState(m_guiState);
}


//************************************************************************
// Player selected a planet
//************************************************************************
void
GameView::planetSelected( Planet *planet )
{
    kDebug() << "planetSelected with " << m_guiState;
    switch( m_guiState ) {
        case SOURCE_PLANET:
            if( planet->player() == m_game->currentPlayer() ) {
                // got a match
                m_shipValidator->setRange(1, planet->fleet().shipCount());
                haveSourcePlanet = true;
                sourcePlanet = planet;
    
                turn();
                return;
            }
            break;
        case RULER_SOURCE:
            haveSourcePlanet = true;
            sourcePlanet     = planet;
            turn();
            return;
        case DEST_PLANET:
        case RULER_DEST:
            if( planet != sourcePlanet ) {
                // got a match
                haveDestPlanet = true;
                destPlanet     = planet;
    
                turn();
            }
            return;
        default:
            break;
    }

    // The selected planet just cannot be selected, cancel it.
    m_mapScene->unselectPlanet();
}


//************************************************************************
// Player hit return in the ship count edit box
//************************************************************************
void
GameView::newShipCount()
{
    bool ok;

    switch (m_guiState) {
    case SHIP_COUNT:
        shipCount = m_shipCountEdit->text().toInt(&ok);
        standingOrder = m_standingOrder->checkState() == Qt::Checked;
        if (ok)
            haveShipCount = true;
        m_shipCountEdit->setText( QString() );
        m_standingOrder->setCheckState(Qt::Unchecked);
        turn();
        break;

    default:
        break;
    };
}

//**********************************************************************
// transition board from play to non-play
//**********************************************************************
void
GameView::changeGameView()
{
    bool isRunning = m_game->isRunning();

    kDebug() << "Calling GameView::changeGameView" << isRunning;

    m_messagesDock->setVisible(isRunning);

    if (!isRunning) {

        // Only hide the standings dock if the game is not running, but do not
        // automatically show it as soon as the game is running.

        m_standingsDock->hide();
    }

    m_mapWidget->setVisible(isRunning);
    m_gameMessage->setVisible(isRunning);
    m_standingOrder->setVisible(isRunning);
    m_shipCountEdit->setVisible(isRunning);
    m_endTurnBtn->setVisible(isRunning);
    m_splashScreen->setVisible(!isRunning); // negation
}


//************************************************************************
// Player clicked the 'End Turn' button
//************************************************************************
void
GameView::nextPlayer()
{
    // Hum, this should be straightforward
    Player *currentPlayer = m_game->currentPlayer();
    LocalPlayer *humanPlayer = qobject_cast<LocalPlayer*>(currentPlayer);
    if (humanPlayer)
        humanPlayer->done();
    /*
    if(m_game->options().BlindMap && m_game->humanPlayerCount() > 1) {
        QString name = m_gameLogic->currentPlayer()->name();
        m_gameLogic->setBlindBreak(true);
        m_mapScene->update();
        KMessageBox::information(this, "Blind Map " + name + " Turn done");
        m_gameLogic->setBlindBreak(false);
    }
    */
}

//************************************************************************
// Toolbar items
//************************************************************************
void
GameView::measureDistance()
{
    switch( m_guiState ) {
    case SOURCE_PLANET:
        m_guiState = RULER_SOURCE;
        turn();
    default:
        break;
    }
}


void
GameView::showFleets()
{
    Player *current = m_game->currentPlayer();
    FleetDlg  *fleetDlg = new FleetDlg( this, current->attackList(),
                                        current->newAttacks(), current->standingOrders());
    if (fleetDlg->exec()) {
        AttackFleetList *deleteAttacks = fleetDlg->uncheckedFleets();
        foreach(AttackFleet *curFleet, *deleteAttacks) {
            current->cancelNewAttack(curFleet);
        }
        delete deleteAttacks;
        m_mapScene->update();
    }
    fleetDlg->deleteLater();
}
