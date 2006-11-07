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

#include <math.h>

#include "gamecore.h"

#include "newgamedlg.h"
#include "gameenddlg.h"
#include "scoredlg.h"
#include "fleetdlg.h"
#include "gameboard.h"
#include "mapview.h"
#include "planet_info.h"
#include "player.h"
#include "map.h"

#include "gameboard.moc"

#include <QtDebug>

/*********************************************************************
 Game Board
*********************************************************************/
GameBoard::GameBoard( QWidget *parent )
    : QWidget( parent ), gameInProgress( false ), gameState( NONE )
{
    QPalette blackPal;
    blackPal.setColor( backgroundRole(), Qt::black );
    setPalette( blackPal );
    setAutoFillBackground( true );

    QColor col(Qt::green);
    QPalette palette;
    palette.setColorGroup( QPalette::Active, Qt::white, Qt::black, col.light(), col.dark(), col, col.dark(75), col.dark(75), col.dark(), Qt::black );
    palette.setColorGroup( QPalette::Inactive, Qt::white, Qt::black, col.light(), col.dark(), col, col.dark(75), col.dark(75), col.dark(), Qt::black );
    palette.setColorGroup( QPalette::Disabled, Qt::white, Qt::black, col.light(), col.dark(), col, col.dark(75), col.dark(75), col.dark(), Qt::black );

    neutralPlayer = Player::createNeutralPlayer(map);
    map = new Map;

    //********************************************************************
    // Create the widgets in the main window
    //********************************************************************
    mapWidget = new MapView( map, this );
    msgWidget = new QTextEdit( this );
    msgWidget->setMinimumHeight(100);
    msgWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    msgWidget->setReadOnly(true);
    blackPal.setColor( QPalette::Base, Qt::black );
    msgWidget->setPalette( blackPal );
    msgWidget->setAutoFillBackground( true );

    planetInfo = new PlanetInfo( this, palette );
    gameMessage = new QLabel( this );
    gameMessage->setPalette( palette );
    turnCounter = new QLabel( this );
    turnCounter->setPalette( palette );
    turnCounter->setText( "Turn" );
    turnCounter->setMaximumHeight( turnCounter->sizeHint().height() );

    endTurn = new QPushButton( i18n("End Turn"), this );
    endTurn->setFixedSize( endTurn->sizeHint() );
    endTurn->setPalette( palette );

    shipCountEdit = new QLineEdit( this );
    shipCountEdit->setValidator( new QIntValidator(1, 32767, this ) );
    shipCountEdit->setMinimumSize( 40, 0 );
    shipCountEdit->setMaximumSize( 32767, 40 );
    shipCountEdit->setEnabled(false);
    shipCountEdit->setPalette( palette );
    shipCountEdit->setEchoMode( QLineEdit::Password );

    splashScreen = new QLabel( this );
    splashScreen->setPixmap(QPixmap(IMAGE_SPLASH));
    splashScreen->setGeometry( 0, 0, 600, 550 );

    setMouseTracking( true );
    setFocusPolicy( Qt::StrongFocus );
    setFocus();

    //********************************************************************
    // Layout the main window
    //********************************************************************
    QHBoxLayout *mainLayout = new QHBoxLayout( this );
    QHBoxLayout *topLineLayout = new QHBoxLayout;
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QVBoxLayout *rightLayout = new QVBoxLayout;

    mainLayout->addLayout( leftLayout );
    leftLayout->addLayout( topLineLayout );

    topLineLayout->addSpacing( 5 );
    topLineLayout->addWidget( gameMessage, 10 );
    topLineLayout->addWidget( shipCountEdit, 1 );
    topLineLayout->addWidget( endTurn, 1 );

    leftLayout->addSpacing( 5 );
    leftLayout->addWidget( mapWidget );
    leftLayout->addWidget( msgWidget );
    leftLayout->addStretch( 1 );

    mainLayout->addSpacing( 5 );
    mainLayout->addLayout( rightLayout, 10 );

    rightLayout->addWidget( planetInfo, 1 );
    //rightLayout->addSpacing( 10 );
    rightLayout->addWidget( turnCounter,  1 );
    rightLayout->addStretch( 1 );

    mainLayout->addStretch( 1 );

    //**********************************************************************
    // Set up signal/slot connections
    //**********************************************************************
    connect( mapWidget, SIGNAL( planetSelected(Planet *) ), this, SLOT(planetSelected(Planet *)) );
    connect( shipCountEdit, SIGNAL(returnPressed()), this, SLOT(newShipCount()) );
    connect( endTurn, SIGNAL( clicked() ), this, SLOT( nextPlayer() ) );
    connect( mapWidget, SIGNAL( planetHighlighted(Planet *)), planetInfo, SLOT(showPlanet(Planet *)) );

    changeGameBoard( false );
}


//**********************************************************************
// Destructor
//**********************************************************************
GameBoard::~GameBoard()
{
    // Nothing much to do yet
}

#if 0
QSize GameBoard::sizeHint() const
{
    return QSize( 600, 550 );
}
#endif

//************************************************************************
// Keyboard Event handlers
//************************************************************************
void
GameBoard::keyPressEvent( QKeyEvent *e )
{
    // Check for the escape key
    if( e->key() == Qt::Key_Escape ) {
        switch( gameState ) {
        case DEST_PLANET:
        case SHIP_COUNT:
        case RULER_SOURCE:
        case RULER_DEST:
            gameState = SOURCE_PLANET;
            haveSourcePlanet = false;
            haveDestPlanet = false;
            turn();
            break;
        default:
            break;
        }
        return;
    }

    if( e->text().isEmpty() || e->text().at(0).isSpace() ) {
        e->ignore();
        return;
    }

    QString planetName;

    planetName += e->text().toUpper();
    
    foreach (Planet *p, planets) {
        if( p->name() == planetName )
            planetSelected( p );
    }

}

QString
GameBoard::playerString(Player *player)
{
    if (!player)
        return (*currentPlayer)->coloredName();
    return player->coloredName();
}

//************************************************************************
// Game engine/state machine
//************************************************************************
void
GameBoard::turn()
{
    Planet *target = 0;

    switch( gameState ) {
    case NONE :
        // stuff for none
        gameState = SOURCE_PLANET;
        haveSourcePlanet = false;
        haveDestPlanet = false;
        haveShipCount = false;
        shipCount = 0;
        mapWidget->unselectPlanet();


        turn();
        setFocus();
        break;

    case SOURCE_PLANET :

        if( haveSourcePlanet ) {
            gameState = DEST_PLANET;

            sourcePlanet->select();
            turn();

        } else {
            shipCountEdit->hide();
            endTurn->setEnabled( true );
            mapWidget->unselectPlanet();
            gameMessage->setText( "<qt>" + playerString() + ": " +
                                    i18n("Select source planet...") + "</qt>" );
            setFocus();
        }

        break;

    case DEST_PLANET :

        if( haveDestPlanet ) {
            mapWidget->unselectPlanet();
            gameState = SHIP_COUNT;
            turn();

        } else {
            shipCountEdit->hide();
            endTurn->setEnabled( false );
            sourcePlanet->select();
            gameMessage->setText( "<qt>" + playerString() + ": " +
                                    i18n("Select destination planet...") + "</qt>" );
            setFocus();
        }

        break;

    case SHIP_COUNT:

        if( haveShipCount ) {
            // We now have a complete fleet to send, so send it
            sendAttackFleet( sourcePlanet, destPlanet, shipCount);

            shipCountEdit->hide();
            endTurn->setEnabled( true );

            gameState = NONE;
            turn();

            endTurn->setFocus();

        } else {
            gameMessage->setText( (*currentPlayer)->name() +
                                    i18n(": How many ships?") );

            shipCountEdit->setText( "" );
            shipCountEdit->show();
            shipCountEdit->setEnabled(true);
            shipCountEdit->setFocus();

            endTurn->setEnabled( false );

            mapWidget->unselectPlanet();
        }

        break;

    case RULER_SOURCE:
        if( haveSourcePlanet ) {
            gameState = RULER_DEST;
            sourcePlanet->select();
            turn();
        } else {
            shipCountEdit->hide();
            endTurn->setEnabled( true );
            mapWidget->unselectPlanet();

            gameMessage->setText( i18n("Ruler: Select starting planet.") );
            setFocus();
        }

        break;

    case RULER_DEST:
        if( haveDestPlanet ) {
            mapWidget->unselectPlanet();

            // Display the distance between the two planets
            double dist = map->distance( sourcePlanet, destPlanet );

            QString msg;
            msg = i18n("The distance from Planet %1 to Planet %2 is %3 light years.\n"
                       "A ship leaving this turn will arrive on turn %4",
                   sourcePlanet->name(),
                   destPlanet->name(),
                   KGlobal::locale()->formatNumber( dist, 2 ),
                   KGlobal::locale()->formatNumber( turnNumber + (int)dist, 0 ));
            KMessageBox::information( this, msg, i18n("Distance"));

            gameState = NONE;
            turn();
        } else {
            gameMessage->setText( i18n("Ruler: Select ending planet.") );
            shipCountEdit->hide();
            endTurn->setEnabled( false );
            sourcePlanet->select();

            setFocus();
        }

        break;

     case AI_PLAYER:
         endTurn->setEnabled( false );
         gameMessage->setText( i18n("Computer Player thinking...") );

         int ships;
         foreach (Planet *home, planets) {
            if (home->player() == (*currentPlayer)) {
                bool hasAttack = false;
                ships = (int)floor(home->fleet().shipCount() * 0.7 );
                
                if (ships >= 20) {
                    Planet *attack;
                    double minDistance = 100;
                    
                    foreach (attack, planets) {
                        bool skip = false;
                        
                        double dist = map->distance( home, attack );
                        
                        if ((dist < minDistance) &&  (attack->player() != (*currentPlayer)) &&
                                (attack->fleet().shipCount() < ships )) {
                            foreach (AttackFleet *curFleet, (*currentPlayer)->attackList()) {
                                if (curFleet->destination == attack) {
                                    skip = true;
                                }
                            }
                            if (skip) continue;
                            
                            target = attack;
                            hasAttack = true;
                            minDistance = dist;
                        }
                    }
                    
                    if (hasAttack) {
                        sendAttackFleet( home, target, ships );
                    } else {
                        minDistance = 100;
                        int shipsToSend = 0;
                        bool hasDestination = false;
                        
                        foreach (attack, planets) {
                            bool skip = false;
                            double dist = map->distance( home, attack );
                            int homeships = (int)floor(home->fleet().shipCount() * 0.5 );
                            
                            if ((dist < minDistance) &&  (attack->player() == (*currentPlayer)) &&
                                      (attack->fleet().shipCount() < homeships )) {
                                foreach (AttackFleet *curFleet, (*currentPlayer)->attackList()) {
                                    if (curFleet->destination == attack) {
                                        skip = true;
                                    }
                                }
                                if (skip) continue;
                                
                                shipsToSend = (int)floor(
					double(home->fleet().shipCount()
					       - attack->fleet().shipCount()) / 2);
                                
                                target = attack;
                                hasDestination = true;
                                minDistance = dist;
                            }
                        }
                        if (hasDestination) {
                            sendAttackFleet( home, target, shipsToSend );
                        }
                    }
                }
            }
        }
        endTurn->setEnabled( true );
        nextPlayer();
        
        break;

    default:
        break;
    }

    QString turnStr;
    turnStr = i18n("Turn #: %1 of %2", turnNumber, lastTurn);

    turnCounter->setText( turnStr );

    emit newGameState( gameState );
}
//************************************************************************
// To the end turn processing (resolve combat, etc.)
//************************************************************************
void
GameBoard::nextTurn()
{
    resolveShipsInFlight();

    scanForSurvivors();

    // advance to first living player
    while( (*currentPlayer) && !(*currentPlayer)->isInPlay() ) {
    	++currentPlayer;
    };

    // advance turn counter
    turnNumber++;

    // update the planets
    foreach (Planet *planet, planets) {
        planet->turn();
    }

    Player *winner = findWinner();
    if (winner)
    {
        KMessageBox::information(this,
              i18n("The mighty %1 has conquered the galaxy!", winner->name()),
              i18n("Game Over"));
    }

    if( (turnNumber == lastTurn) && !winner )
    {
        GameEndDlg *dlg = new GameEndDlg( this );

        if( dlg->exec() == KDialog::Yes ) {
            lastTurn += dlg->extraTurns();
        }

        delete dlg;
    }

    if( winner || (turnNumber >= lastTurn) )
    {
        // Game over, man! Game over.

        gameOver();
    };
}

//************************************************************************
// determine the fate of the ships in transit
//************************************************************************
void
GameBoard::resolveShipsInFlight()
{
    AttackFleetList arrivingShips;
    
    foreach (Player *plr, players) {
        foreach (AttackFleet *fleet, plr->attackList()) {
            double fleetArrivalTurn = floor(fleet->arrivalTurn);

            if( turnNumber == int (fleetArrivalTurn) ) {
                doFleetArrival( fleet );
                plr->attackList().remove( fleet );
                delete fleet;
            }
        }
    }

}

Player *
GameBoard::findWinner()
{
    Player *winner = 0;

    foreach (Player *plr, players) {
        if (plr->isInPlay())
        {
            if (winner != 0)
                return 0;
            winner = plr;
        }
        else if (plr->attackList().count() != 0)
            // There is still a fleet attacking, so wait for its arrival...
            return 0;
    }
    return winner;
}

void
GameBoard::gameMsg(const KLocalizedString &msg, Player *player, Planet *planet, Player *planetPlayer)
{
    bool isHumanInvolved = false;

    QString color = "white";
    KLocalizedString colorMsg = msg;
    KLocalizedString plainMsg = msg;

    if (player)
    {
       if (!player->isAiPlayer())
          isHumanInvolved = true;
       colorMsg = colorMsg.subs(playerString(player));
       plainMsg = plainMsg.subs(player->name());
    }

    if (planet)
    {
       if (!planetPlayer)
          planetPlayer = planet->player();
       if (!planetPlayer->isAiPlayer() && !planetPlayer->isNeutral())
          isHumanInvolved = true;

       QString color = planetPlayer->color().name();
       colorMsg = colorMsg.subs(QString("<font color=\"%1\">%2</font>").arg(color, planet->name()));
       plainMsg = plainMsg.subs(planet->name());
    }
    msgWidget->append("<qt><font color=\"white\">"+i18n("Turn %1:", turnNumber)+"</font> <font color=\""+color+"\">"+colorMsg.toString()+"</font></qt>");
    msgWidget->moveCursor( QTextCursor::End );

    if (isHumanInvolved)
    {
       KMessageBox::information(this, plainMsg.toString());
    }
}

//************************************************************************
// check to see any players have been eliminated
//************************************************************************
void
GameBoard::scanForSurvivors()
{
    QList<Player *> activePlayers;
    QList<Player *> inactivePlayers;

    // insert all of the active players into a special
    // list, the deactivate them
    Player *plr;
    foreach (plr, players) {
        if( plr->isInPlay() ) {
            activePlayers.append( plr );
            plr->setInPlay( false );
        } else {
            inactivePlayers.append( plr );
        }
    }


    // iterate through the list of planets and
    // mark their owners in play
    foreach (Planet *planet, planets) {
        planet->player()->setInPlay( true );
    }

    foreach (plr, activePlayers) {
        if( !plr->isInPlay() ) {
            // Player has bitten the dust
            gameMsg(ki18n("The once mighty empire of %1 has fallen in ruins."), plr);
        }
    }

    foreach (plr, inactivePlayers) {
        if( plr->isInPlay() ) {
            // Player has bitten the dust
            gameMsg(ki18n("The fallen empire of %1 has staggered back to life."), plr);
        }
    }
}

//************************************************************************
// handle the arrival of a fleet at a planet
//************************************************************************
void
GameBoard::doFleetArrival( AttackFleet *arrivingFleet )
{
    // Check to see of (fleet owner) == (planet owner)
    // if the planet and fleet owner are the same, then merge the fleets
    // otherwise attack.

    if( (*arrivingFleet->owner) == (*arrivingFleet->destination->player())) {
        if (!arrivingFleet->owner->isAiPlayer()) {
        	arrivingFleet->destination->fleet().absorb(arrivingFleet);

        	gameMsg(ki18np("Reinforcements (1 ship) have arrived for planet %2.",
        	               "Reinforcements (%n ships) have arrived for planet %2.")
        	              .subs(arrivingFleet->shipCount()),
                        0, arrivingFleet->destination);
        }
    } else {

        // let's get ready to rumble...

        CoreLogic cl;
        AttackFleet &attacker = *arrivingFleet;
        DefenseFleet &defender = arrivingFleet->destination->fleet();
        Planet &prizePlanet = *(arrivingFleet->destination);

        bool haveVictor = false;
        bool planetHolds = true;

        while( !haveVictor ) {
            double attackerRoll = cl.roll();
            double defenderRoll = cl.roll();

            if( defenderRoll < prizePlanet.killPercentage() ) {
                attacker.removeShips( 1 );
            }

            if( attacker.shipCount() <= 0 ) {
                haveVictor = true;
                planetHolds = true;
                continue;
            }

            if( attackerRoll < attacker.killPercentage ) {
                defender.removeShips( 1 );
                attacker.owner->statEnemyShipsDestroyed( 1 );
            }

            if( defender.shipCount() <= 0 ) {
                haveVictor = true;
                planetHolds = false;
            }
        }

        if( planetHolds ) {
            prizePlanet.player()->statEnemyFleetsDestroyed(1);
            gameMsg(ki18n("Planet %2 has held against an attack from %1."),
                    attacker.owner, &prizePlanet);
        } else {
            Player *defender = prizePlanet.player();
            attacker.owner->statEnemyFleetsDestroyed( 1 );

            arrivingFleet->destination->conquer( arrivingFleet );

            gameMsg(ki18n("Planet %2 has fallen to %1."),
                    attacker.owner, &prizePlanet, defender);
        }
    }

    mapWidget->update();
}

//************************************************************************
// Set up the game board for a new game
//************************************************************************
void
GameBoard::startNewGame()
{
    shutdownGame();

    if( gameInProgress )
        return;

    NewGameDlg *newGame = new NewGameDlg( this, map, &players, neutralPlayer, &planets );

    if( !newGame->exec() )
    {
        delete newGame;
        return;
    }
    newGame->save(); // Save settings for next time

    msgWidget->clear();

    changeGameBoard( true );

    shipCountEdit->hide();
    endTurn->setEnabled( true );

    currentPlayer = players.begin();

    endTurn->show();
    gameMessage->show();

    lastTurn = newGame->turns();

    turnNumber = 1;
    turn();

    delete newGame;
}

//************************************************************************
// Shut down the current game
//************************************************************************
void
GameBoard::shutdownGame()
{
    if( !gameInProgress )
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
GameBoard::gameOver()
{
    ScoreDlg *scoreDlg = new ScoreDlg( this, i18n("Final Standings"), &players );
    scoreDlg->exec();

    cleanupGame();
}

void
GameBoard::cleanupGame()
{
    map->clearMap();

    planets.clear();
    players.clear();

    shipCountEdit->hide();
    endTurn->setEnabled( false );

    gameMessage->hide();
    endTurn->hide();

    changeGameBoard( false );
    gameState = NONE;
    emit newGameState(gameState);
}


//************************************************************************
// Player selected a planet
//************************************************************************
void
GameBoard::planetSelected( Planet *planet )
{
    switch( gameState ) {
    case SOURCE_PLANET:
        if( ((*planet).player()) == (*currentPlayer) ) {
            // got a match
            haveSourcePlanet = true;
            sourcePlanet = planet;

            turn();
        }

        break;

    case RULER_SOURCE:
        haveSourcePlanet = true;
        sourcePlanet = planet;
        turn();
        break;

    case DEST_PLANET:
    case RULER_DEST:
        if( planet != sourcePlanet ) {
            // got a match
            haveDestPlanet = true;
            destPlanet = planet;

            turn();
        }

        break;

    default:
    case NONE :
        break;
    }

}

//************************************************************************
// Player hit return in the ship count edit box
//************************************************************************
void
GameBoard::newShipCount()
{
    QString temp( shipCountEdit->text() );
    bool ok;

    switch( gameState ) {
    case SHIP_COUNT:
        shipCount = temp.toInt(&ok);

        if( ok )
            haveShipCount = true;

        shipCountEdit->setText( "" );

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
GameBoard::changeGameBoard( bool inPlay  )
{
    gameInProgress = inPlay;

    if( gameInProgress ) {
        mapWidget->show();
        planetInfo->show();
        gameMessage->show();
        endTurn->show();
        shipCountEdit->show();
        splashScreen->hide();
    } else {
        mapWidget->hide();
        planetInfo->hide();
        gameMessage->hide();
        endTurn->hide();
        shipCountEdit->hide();
        splashScreen->show();
    }
}

//************************************************************************
// Player clicked the 'End Turn' button
//************************************************************************
void
GameBoard::nextPlayer()
{
    // end turn and advance to next player
    do {
        ++currentPlayer;
    } while ((currentPlayer != players.end()) && (!(*currentPlayer)->isInPlay()));

    if( currentPlayer == players.end() ) {
        // end of player list, new turn
        currentPlayer = players.begin();
        nextTurn();
    }

    if( gameInProgress ) {
       if ((*currentPlayer)->isAiPlayer()) {
             gameState = AI_PLAYER;
         }
         else {
             gameState = SOURCE_PLANET;

        }
       turn();
    }
}

//************************************************************************
// A complete set of source, destination planets and ship count has been
// entered, so do something about it
//************************************************************************
void
GameBoard::sendAttackFleet( Planet *source, Planet *dest, int ship )
{
    bool ok;

    ok = (*currentPlayer)->NewAttack( source, dest,
                                                ship, turnNumber );

    if( !ok ) {
      KMessageBox::error( this,
			  i18n("Not enough ships to send.") );
    }
}

//************************************************************************
// Toolbar items
//************************************************************************
void
GameBoard::measureDistance()
{
    switch( gameState ) {
    case SOURCE_PLANET:
        gameState = RULER_SOURCE;
        turn();
    default:
        break;
    }
}

void
GameBoard::showScores()
{
    ScoreDlg *scoreDlg = new ScoreDlg( this, i18n("Current Standings"), &players );
    scoreDlg->show();
}

void
GameBoard::showFleets()
{
  FleetDlg *fleetDlg = new FleetDlg( this, &((*currentPlayer)->attackList()) );
  fleetDlg->show();
}
