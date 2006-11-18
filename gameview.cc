#include <math.h>

#include <QLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QPixmap>
#include <QKeyEvent>
#include <QIntValidator>

#include <kmessagebox.h>

#include "gamecore.h"
#include "player.h"
#include "map.h"
#include "gamelogic.h"
#include "images.h"
#include "newgamedlg.h"
#include "scoredlg.h"
#include "fleetdlg.h"
#include "mapview.h"
#include "mapscene.h"
#include "gameview.h"

#include "gameview.moc"

#include <QtDebug>


/*********************************************************************
 Game Board
*********************************************************************/

GameView::GameView( QWidget *parent, GameLogic *gameLogic )
  : QWidget( parent ),
    m_gameLogic( gameLogic ),
    m_queueMessages(false),
    m_messageQueue(), 
    m_showInformations(false),
    m_guiState( NONE )
{
    QPalette blackPal;
    blackPal.setColor( backgroundRole(), Qt::black );
    setPalette( blackPal );
    setAutoFillBackground( true );

    QColor col(Qt::green);
    QPalette palette;
    palette.setColorGroup( QPalette::Active,   Qt::white,    Qt::black, 
                           col.light(),        col.dark(),   col,
                           col.dark(75),       col.dark(75), col.dark(),
                           Qt::black );
    palette.setColorGroup( QPalette::Inactive, Qt::white,    Qt::black,
                           col.light(),        col.dark(),   col,
                           col.dark(75),       col.dark(75), col.dark(), 
                           Qt::black );
    palette.setColorGroup( QPalette::Disabled, Qt::white,    Qt::black,
                           col.light(),        col.dark(),   col, 
                           col.dark(75),       col.dark(75), col.dark(),
                           Qt::black );
    
    blackPal.setColor( QPalette::Base, Qt::black );

    m_neutralPlayer = Player::createNeutralPlayer( m_gameLogic->map() );

    //********************************************************************
    // Create the widgets in the main window
    //********************************************************************
    m_mapScene  = new MapScene(m_gameLogic->map());
    m_mapWidget = new MapView( m_mapScene );
    m_mapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    m_msgWidget = new QTextEdit( this );
    m_msgWidget->setMaximumHeight(75);
    m_msgWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_msgWidget->setReadOnly(true);
    m_msgWidget->setPalette( blackPal );
    m_msgWidget->setAutoFillBackground( true );
    m_msgWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_gameMessage = new QLabel( this );
    m_gameMessage->setPalette( palette );

    m_endTurnBtn = new QPushButton( i18n("End Turn"), this );
    m_endTurnBtn->setFixedSize( m_endTurnBtn->sizeHint() );
    m_endTurnBtn->setPalette( palette );

    m_shipCountEdit = new QLineEdit( this );
    m_shipCountEdit->setValidator( new QIntValidator(1, 32767, this ) );
    m_shipCountEdit->setMinimumSize( 40, 0 );
    m_shipCountEdit->setMaximumSize( 32767, 40 );
    m_shipCountEdit->setEnabled(false);
    m_shipCountEdit->setPalette( palette );
    m_shipCountEdit->setEchoMode( QLineEdit::Password );

    m_splashScreen = new QLabel( this );
    m_splashScreen->setPixmap(QPixmap(IMAGE_SPLASH));
    m_splashScreen->setGeometry( 0, 0, 600, 550 );

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
    topLineLayout->addWidget( m_shipCountEdit, 1 );
    topLineLayout->addWidget( m_endTurnBtn, 1 );
    
    mainLayout->addLayout( topLineLayout );
    mainLayout->addWidget( m_mapWidget );
    mainLayout->addWidget( m_msgWidget );

    //**********************************************************************
    // Set up signal/slot connections
    //**********************************************************************
    connect( m_mapScene,      SIGNAL( planetSelected(Planet *) ),
             this,            SLOT(planetSelected(Planet *)) );
    connect( m_shipCountEdit, SIGNAL(returnPressed()),
             this,            SLOT(newShipCount()) );
    connect( m_endTurnBtn,    SIGNAL( clicked() ),
             this,            SLOT( nextPlayer() ) );

    changeGameView( false );
}


//**********************************************************************
// Destructor
//**********************************************************************

GameView::~GameView()
{
    // Nothing much to do yet
}


#if 0
QSize GameView::sizeHint() const
{
    return QSize( 600, 550 );
}
#endif


//************************************************************************
// Keyboard Event handlers
//************************************************************************

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
    
    foreach (Planet *p, *m_gameLogic->planets()) {
        if( p->name() == planetName ) {
            if ( m_showInformations ) {
                m_mapScene->displayPlanetInfo( p );
                m_showInformations = false;
            } else {
                m_mapScene->displayPlanetInfo( NULL );
                planetSelected( p );
            }
            break;
        }
    }

}


//************************************************************************
// Game engine/state machine
//************************************************************************


void
GameView::turn()
{
    Planet *target = 0;

    switch( m_guiState ) {
    case NONE :
        // The standby state, waiting for clicking on a planet or starting
        // to measure a distance..
        m_guiState      = SOURCE_PLANET;
        haveSourcePlanet = false;
        haveDestPlanet   = false;
        haveShipCount    = false;
        shipCount        = 0;
        m_mapScene->unselectPlanet();

        turn();
        setFocus();
        break;

    case SOURCE_PLANET :
        // The user has clicked on a source planet for moving some fleets.
        if( haveSourcePlanet ) {
            m_guiState = DEST_PLANET;

            sourcePlanet->select();
            turn();

        } else {
            m_shipCountEdit->hide();
            m_endTurnBtn->setEnabled( true );
            m_mapScene->unselectPlanet();
            m_gameMessage->setText( "<qt>" 
                                  + m_gameLogic->currentPlayer()->coloredName()
                                  + ": "
                                  +i18n("Select source planet...") + "</qt>" );
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
            m_shipCountEdit->hide();
            m_endTurnBtn->setEnabled( false );
            sourcePlanet->select();
            m_gameMessage->setText( "<qt>"
                                  + m_gameLogic->currentPlayer()->coloredName()
                                  + ": "
                                  + i18n("Select destination planet...")
                                  + "</qt>" );
            setFocus();
        }

        break;

    case SHIP_COUNT:
        // The user has selected, source, distance, ship count.
        if( haveShipCount ) {
            // We now have a complete fleet to send, so send it
            sendAttackFleet( sourcePlanet, destPlanet, shipCount);

            m_shipCountEdit->hide();
            m_endTurnBtn->setEnabled( true );

            m_guiState = NONE;
            turn();

            m_endTurnBtn->setFocus();

        } else {
            m_gameMessage->setText( m_gameLogic->currentPlayer()->name() +
                                    i18n(": How many ships?") );

            m_shipCountEdit->setText( "" );
            m_shipCountEdit->show();
            m_shipCountEdit->setEnabled(true);
            m_shipCountEdit->setFocus();

            m_endTurnBtn->setEnabled( false );

            m_mapScene->unselectPlanet();
        }

        break;

    case RULER_SOURCE:
        // The user has selected to measure a distance with the ruler.
        if( haveSourcePlanet ) {
            m_guiState = RULER_DEST;
            sourcePlanet->select();
            turn();
        } else {
            m_shipCountEdit->hide();
            m_endTurnBtn->setEnabled( true );
            m_mapScene->unselectPlanet();

            m_gameMessage->setText( i18n("Ruler: Select starting planet.") );
            setFocus();
        }
        break;

    case RULER_DEST:
        if( haveDestPlanet ) {
            m_mapScene->unselectPlanet();

            // Display the distance between the two planets
            double dist = m_gameLogic->map()->distance( sourcePlanet,
                                                        destPlanet );

            QString msg;
            msg = i18n("The distance from Planet %1 to Planet %2 is %3 light years.\n"
                       "A ship leaving this turn will arrive on turn %4",
                   sourcePlanet->name(),
                   destPlanet->name(),
                   KGlobal::locale()->formatNumber( dist, 2 ),
                   KGlobal::locale()->formatNumber( m_gameLogic->turnNumber() 
                                                    + (int)dist, 0 ));
            KMessageBox::information( this, msg, i18n("Distance"));

            m_guiState = NONE;
            turn();
        } else {
            m_gameMessage->setText( i18n("Ruler: Select ending planet.") );
            m_shipCountEdit->hide();
            m_endTurnBtn->setEnabled( false );
            sourcePlanet->select();

            setFocus();
        }

        break;

     case AI_PLAYER:
         m_endTurnBtn->setEnabled( false );
         m_gameMessage->setText( i18n("Computer Player thinking...") );

         int ships;
         foreach (Planet *home, *m_gameLogic->planets()) {
             if (home->player() == m_gameLogic->currentPlayer()) {
                 bool  hasAttack = false;
                 ships = (int)floor(home->fleet().shipCount() * 0.7 );
                
                 if (ships >= 20) {
                     Planet  *attack;
                     double  minDistance = 100;
                    
                     foreach (attack, *m_gameLogic->planets()) {
                         bool    skip = false;
                         double  dist = m_gameLogic->map()->distance( home, attack );
                        
                         if (dist < minDistance
                             &&  attack->player() != m_gameLogic->currentPlayer()
                             && attack->fleet().shipCount() < ships ) {
                             foreach (AttackFleet *curFleet, m_gameLogic->currentPlayer()->attackList()) {
                                 if (curFleet->destination == attack) {
                                     skip = true;
                                 }
                             }
                             if (skip) 
                                 continue;
                            
                             target      = attack;
                             hasAttack   = true;
                             minDistance = dist;
                         }
                     }
                    
                     if (hasAttack) {
                         sendAttackFleet( home, target, ships );
                     } else {
                         minDistance = 100;
                         int shipsToSend = 0;
                         bool hasDestination = false;
                        
                         foreach (attack, *m_gameLogic->planets()) {
                             bool    skip = false;
                             double  dist = m_gameLogic->map()->distance( home, attack );
                             int     homeships = (int)floor(home->fleet().shipCount() * 0.5 );
                            
                             if (dist < minDistance
                                 && attack->player() == m_gameLogic->currentPlayer()
                                 && attack->fleet().shipCount() < homeships ) {
                                 foreach (AttackFleet *curFleet,
                                          m_gameLogic->currentPlayer()->attackList()) {
                                     if (curFleet->destination == attack) {
                                         skip = true;
                                     }
                                 }
                                 if (skip)
                                     continue;
                                
                                 shipsToSend = (int)floor( double(home->fleet().shipCount()
                                                                  - attack->fleet().shipCount()) / 2);
                                
                                 target         = attack;
                                 hasDestination = true;
                                 minDistance    = dist;
                             }
                         }
                         if (hasDestination) {
                             sendAttackFleet( home, target, shipsToSend );
                         }
                     }
                 }
             }
        }
        m_endTurnBtn->setEnabled( true );
        nextPlayer();
        
        break;

    default:
        break;
    }

    emit newGUIState( m_guiState );
}


//************************************************************************
// To the end turn processing (resolve combat, etc.)
//************************************************************************


void
GameView::beginTurn()
{
    if (m_messageQueue.size() > 0) {
        foreach (Player *plr, *(m_gameLogic->players())) {
            if (plr->isAiPlayer())
                continue;
            QString text;
            foreach (GameMessage msg, m_messageQueue) {
                if (plr == msg.sender || plr == msg.receiver)
                    text = text + "<br />" + msg.text;
            }
            if (text.size() > 0) {
                text = "<html>"
                    + i18n("Messages for %1", plr->coloredName())
                    + "<br />" + text + "</html>";
                KMessageBox::information(this, text);
            }
        }
        m_messageQueue.clear();
    }
    
    m_queueMessages = false;
    
    // Check for game over.
    Player *winner = m_gameLogic->findWinner();
    if (winner) {
        KMessageBox::information(this,
              i18n("The mighty %1 has conquered the galaxy!", winner->name()),
              i18n("Game Over"));
    }

    if( winner )
        gameOver();
}


void
GameView::endTurn()
{
    // A lot of messages will be collected in the message queue during
    // the turn finalisation.
    m_queueMessages = true;
}


void
GameView::gameMsg(const KLocalizedString &msg, Player *player, Planet *planet, Player *planetPlayer)
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
                        + i18n("Turn %1:", m_gameLogic->turnNumber())
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
// Set up the game board for a new game
//************************************************************************


void
GameView::startNewGame()
{
    shutdownGame();

    if( m_gameInProgress )
        return;

    NewGameDlg *newGame = new NewGameDlg( this, m_gameLogic->map(),
                                          m_gameLogic->players(),
                                          m_neutralPlayer, 
                                          m_gameLogic->planets() );

    if( !newGame->exec() ) {
        delete newGame;
        return;
    }
    newGame->save(); // Save settings for next time

    // Fix all the widgets to run a new game.
    changeGameView( true );

    // Clear all the structures in the game.
    // FIXME: It is very confusing that this has to come after the
    //        call to newGame->exec().  Change that.
    m_gameLogic->startNewGame();

    m_msgWidget->clear();
    m_shipCountEdit->hide();
    m_endTurnBtn->setEnabled( true );
    m_endTurnBtn->show();
    m_gameMessage->show();

    turn();

    delete newGame;
}


//************************************************************************
// Shut down the current game
//************************************************************************


void
GameView::shutdownGame()
{
    if( !m_gameInProgress )
        return;

    int choice = KMessageBox::warningContinueCancel
      ( this,
        i18n("Do you wish to retire this game?"),
        i18n("End Game"),
        KStdGuiItem::ok() );

    if( choice == KMessageBox::Cancel )
        return;

    gameOver();
}

void
GameView::gameOver()
{
    ScoreDlg  *scoreDlg = new ScoreDlg( this, i18n("Final Standings"), 
                                        m_gameLogic->players() );
    scoreDlg->exec();

    cleanupGame();
}

void
GameView::cleanupGame()
{
    m_shipCountEdit->hide();
    m_endTurnBtn->setEnabled( false );

    m_gameMessage->hide();
    m_endTurnBtn->hide();

    changeGameView( false );
    m_guiState = NONE;

    emit newGUIState(m_guiState);
}


//************************************************************************
// Player selected a planet
//************************************************************************


void
GameView::planetSelected( Planet *planet )
{
    switch( m_guiState ) {
        case SOURCE_PLANET:
            if( ((*planet).player()) == m_gameLogic->currentPlayer() ) {
                // got a match
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
    m_mapScene->unselectPlanet();

}


//************************************************************************
// Player hit return in the ship count edit box
//************************************************************************


void
GameView::newShipCount()
{
    QString  temp( m_shipCountEdit->text() );
    bool ok;

    switch( m_guiState ) {
    case SHIP_COUNT:
        shipCount = temp.toInt(&ok);

        if( ok )
            haveShipCount = true;

        m_shipCountEdit->setText( "" );

        turn();

        // FIXME: Insert m_gameLogic->doMove() here.

        break;

    default:
        break;
    };
}


//**********************************************************************
// transition board from play to non-play
//**********************************************************************


void
GameView::changeGameView( bool inPlay  )
{
    m_gameInProgress = inPlay;

    if( m_gameInProgress ) {
        m_mapWidget->show();
        m_gameMessage->show();
        m_endTurnBtn->show();
        m_shipCountEdit->show();
        m_splashScreen->hide();
    } else {
        m_mapWidget->hide();
        m_gameMessage->hide();
        m_endTurnBtn->hide();
        m_shipCountEdit->hide();
        m_splashScreen->show();
    }
}


//************************************************************************
// Player clicked the 'End Turn' button
//************************************************************************


// FIXME: the handling of AI players should NOT be in gameview.

void
GameView::nextPlayer()
{
    m_gameLogic->nextPlayer();

    if( m_gameInProgress ) {
        if (m_gameLogic->currentPlayer()->isAiPlayer()) {
            m_guiState = AI_PLAYER;
        }
        else {
            m_guiState = SOURCE_PLANET;
        }

        turn();
    }
}


//************************************************************************
// A complete set of source, destination planets and ship count has been
// entered, so do something about it
//************************************************************************


// FIXME: This should be in gameLogic, I think.

void
GameView::sendAttackFleet( Planet *source, Planet *dest, int ship )
{
    bool ok;

    ok = m_gameLogic->currentPlayer()->NewAttack( source, dest, ship, 
                                                  m_gameLogic->turnNumber() );

    if( !ok ) {
        KMessageBox::error( this, i18n("Not enough ships to send.") );
    }
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
GameView::showScores()
{
    ScoreDlg *scoreDlg = new ScoreDlg( this, i18n("Current Standings"),
                                       m_gameLogic->players() );
    scoreDlg->show();
}


void
GameView::showFleets()
{
  FleetDlg  *fleetDlg = new FleetDlg( this, &(m_gameLogic->currentPlayer()->attackList()) );
  fleetDlg->show();
}
