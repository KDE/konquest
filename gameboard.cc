#include <qlayout.h>
#include <qcolor.h>
#include <qlabel.h>
#include <qslider.h>
#include <qevent.h>
#include <qkeycode.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qvalidator.h>
#include <qtextedit.h>

#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kglobal.h>
#include <kiconloader.h>

#include <ctype.h>
#include <math.h>

#include "gamecore.h"

#include "int_validator.h"
#include "newgamedlg.h"
#include "gameenddlg.h"
#include "scoredlg.h"
#include "fleetdlg.h"
#include "gameboard.h"
#include "gameboard.moc"


/*********************************************************************
 Game Board
*********************************************************************/
GameBoard::GameBoard( QWidget *parent )
    : QWidget( parent ), gameInProgress( false ), gameState( NONE )
{
    QColorGroup cg( white, black, green.light(), green.dark(), green, green.dark(75), green.dark() );
    QPalette palette( cg, cg, cg );

    neutralPlayer = Player::createNeutralPlayer();
    map = new Map;
    
    planets.setAutoDelete(true);
    players.setAutoDelete(true);

    //********************************************************************
    // Create the widgets in the main window
    //********************************************************************
    mapWidget = new ConquestMap( map, this );
    msgWidget = new QTextEdit( this );
    msgWidget->setTextFormat(LogText);
    msgWidget->setMinimumHeight(100);
    msgWidget->setHScrollBarMode(QScrollView::AlwaysOff);
    msgWidget->setPaper(QBrush(Qt::black));
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
    IntValidator *v = new IntValidator( 1, 32767, this );
    shipCountEdit->setValidator( v );
    shipCountEdit->setMinimumSize( 40, 0 );
    shipCountEdit->setMaximumSize( 32767, 40 );
    shipCountEdit->setEnabled(false);
    shipCountEdit->setPalette( palette );
    shipCountEdit->setEchoMode( QLineEdit::Password );

    splashScreen = new QLabel( this );
    splashScreen->setPixmap(QPixmap(IMAGE_SPLASH));
    splashScreen->setGeometry( 0, 0, 600, 550 );

    setMinimumSize( 600, 600 );

    setMouseTracking( true );
    setFocusPolicy( StrongFocus );
    setFocus();

    //********************************************************************
    // Layout the main window
    //********************************************************************
    QHBoxLayout *layout1 = new QHBoxLayout( this );
    QVBoxLayout *layout2 = new QVBoxLayout;
    QHBoxLayout *layout3 = new QHBoxLayout;
    QVBoxLayout *layout4 = new QVBoxLayout;

    layout1->addLayout( layout2 );
    layout2->addLayout( layout3 );

    layout3->addSpacing( 5 );
    layout3->addWidget( gameMessage, 10 );
    layout3->addWidget( shipCountEdit, 1 );
    layout3->addWidget( endTurn, 1 );

    layout2->addSpacing( 5 );
    layout2->addWidget( mapWidget, 0, AlignTop );
    layout2->addWidget( msgWidget );
    layout2->addStretch( 1 );

    layout1->addSpacing( 5 );
    layout1->addLayout( layout4, 10 );

    layout4->addWidget( planetInfo, 1 );
    layout4->addSpacing( 10 );
    layout4->addWidget( turnCounter,  1 );
    layout4->addStretch( 1 );

    layout1->addStretch( 1 );

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
    if( e->key() == Key_Escape ) {
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

    if( !isgraph( e->ascii() ) ) {
        e->ignore();
        return;
    }

    PlanetListIterator planetSearch( planets );
    QString planetName;

    planetName += toupper( e->ascii() );

    for(Planet *p = planetSearch.toFirst();
        p != NULL;
        p = ++planetSearch ) {

        if( p->getName() == planetName )
            planetSelected( p );
    }

}

QString
GameBoard::playerString(Player *player)
{
    if (!player)
        player = currentPlayer->current();
    return player->getColoredName();
}

//************************************************************************
// Game engine/state machine
//************************************************************************
void
GameBoard::turn()
{
    PlanetListIterator planetAi( planets );
    PlanetListIterator planetAttack( planets );
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
            gameMessage->setText( currentPlayer->current()->getName() +
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
            CoreLogic cl;
            double dist = cl.distance( sourcePlanet, destPlanet );

            QString msg;
            msg = i18n("The distance from Planet %1 to Planet %2 is %3 light years.\n"
                       "A ship leaving this turn will arrive on turn %4")
                  .arg(sourcePlanet->getName())
		  .arg(destPlanet->getName())
                  .arg(KGlobal::locale()->formatNumber( dist, 2 ))
		  .arg(KGlobal::locale()->formatNumber( turnNumber + (int)dist, 0 ));
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

         Planet *home;

         int ships;
         planetAi.toFirst();
         
         while ((home = planetAi())) {
            if (home->getPlayer() == currentPlayer->current()) {

                           bool hasAttack = false;
                           ships = (int)floor(home->getFleet().getShipCount() * 0.7 );
                           
                           if (ships >= 20) {

				Planet *attack;
				double minDistance = 100;
			        planetAttack.toFirst();
				while ((attack = planetAttack())) {
                                        bool skip = false;
                                        
					CoreLogic cl;
					double dist = cl.distance( home, attack );

					 if ((dist < minDistance) &&  (attack->getPlayer() != currentPlayer->current()) &&
                                                        (attack->getFleet().getShipCount() < ships )) {
                                                AttackFleetListIterator FleetsinFlight( currentPlayer->current()->getAttackList() );
                                                AttackFleet *curFleet;

                                                while ( (curFleet = FleetsinFlight())) {
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
                                }
                                else {
                                    planetAttack.toFirst();
                                    minDistance = 100;
                                    int shipsToSend = 0;
                                    bool hasDestination = false;
                                    
                                    while ((attack = planetAttack())) {
                                        bool skip = false;
                                        CoreLogic cl;
                                        double dist = cl.distance( home, attack );
                                        int homeships = (int)floor(home->getFleet().getShipCount() * 0.5 );
                                        
                                        if ((dist < minDistance) &&  (attack->getPlayer() == currentPlayer->current()) &&
                                                        (attack->getFleet().getShipCount() < homeships )) {
                                                AttackFleetListIterator FleetsinFlight( currentPlayer->current()->getAttackList() );
                                                AttackFleet *curFleet;

                                                while ( (curFleet = FleetsinFlight())) {
                                                 	if (curFleet->destination == attack) {
                                                        	skip = true;
							}
						}
                                                if (skip) continue;

                                                shipsToSend = (int)floor((home->getFleet().getShipCount() - attack->getFleet().getShipCount()) / 2) ;
                                                
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
    turnStr = i18n("Turn #: %1 of %2").arg(turnNumber).arg(lastTurn);

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
    while( currentPlayer->current() && !currentPlayer->current()->isInPlay() ) {
    	++(*currentPlayer);
    };

    // advance turn counter
    turnNumber++;

    // update the planets
    PlanetListIterator nextPlanet( planets );
    Planet *planet;

    while( (planet = nextPlanet()) )
    {
        planet->turn();
    }

    // Tell the status widget to update itself
    planetInfo->rescanPlanets();

    Player *winner = findWinner();
    if (winner)
    {
        mapWidget->repaint(true);
        KMessageBox::information(this, 
              i18n("The mighty %1 has conquered the galaxy!").arg(winner->getName()),
              i18n("Game Over"));
    }

    if( (turnNumber == lastTurn) && !winner )
    {
        mapWidget->repaint(true);
        GameEndDlg *dlg = new GameEndDlg( this );

        if( dlg->exec() == KDialogBase::Yes ) {
            lastTurn += dlg->extraTurns();
        }

        delete dlg;
    }

    if( winner || (turnNumber >= lastTurn) )
    {
        // Game over, man! Game over.

        mapWidget->repaint(true);

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
    PlayerListIterator nextPlayer( players );
    Player *plr;

    while( (plr = nextPlayer()) ) {
        AttackFleetListIterator nextFleet( plr->getAttackList() );

        AttackFleet *fleet;

        while( (fleet = nextFleet()) ) {
            double fleetArrivalTurn = floor(fleet->arrivalTurn);

            if( turnNumber == int (fleetArrivalTurn) ) {
                doFleetArrival( fleet );
                plr->getAttackList().removeRef( fleet );
                delete fleet;
            }
        }
    }

}

Player *
GameBoard::findWinner()
{
    Player *winner = 0;
    int activePlayers = 0;

    PlayerListIterator nextPlayer( players );
    Player *plr;
    
    while( (plr = nextPlayer()) ) {
        if (plr->isInPlay())
        {
            winner = plr;
            activePlayers++;
        }
        else if (plr->getAttackList().count() != 0)
        {
            activePlayers++;
        }
    }
    if (activePlayers == 1)
        return winner;

    return 0;
}

void
GameBoard::gameMsg(const QString &msg, Player *player, Planet *planet, Player *planetPlayer)
{
    bool isHumanInvolved = false;
       
    QString color = "white";
    QString colorMsg = msg;
    QString plainMsg = msg;

    if (player)
    {
       if (!player->isAiPlayer())
          isHumanInvolved = true;
       colorMsg = colorMsg.arg(playerString(player));
       plainMsg = plainMsg.arg(player->getName());
    }

    if (planet)
    {
       if (!planetPlayer)
          planetPlayer = planet->getPlayer();
       if (!planetPlayer->isAiPlayer() && !planetPlayer->isNeutral())
          isHumanInvolved = true;

       QString color = planetPlayer->getColor().name();
       colorMsg = colorMsg.arg(QString("<font color=\"%1\">%2</font>").arg(color, planet->getName()));
       plainMsg = plainMsg.arg(planet->getName());
    }
    msgWidget->append(("<qt><font color=\"white\">Turn %1:</font> <font color=\""+color+"\">").arg(turnNumber)+colorMsg+"</font></qt>");
    msgWidget->scrollToBottom();
    
    if (isHumanInvolved)
    {
       mapWidget->repaint(true);
       KMessageBox::information(this, plainMsg);
    }
}

//************************************************************************
// check to see any players have been eliminated
//************************************************************************
void
GameBoard::scanForSurvivors()
{
    PlayerListIterator nextPlayer( players );
    PlayerList activePlayers;
    PlayerList inactivePlayers;

    // insert all of the active players into a special
    // list, the deactivate them
    Player *plr;
    while( (plr = nextPlayer()) ) {
        if( plr->isInPlay() ) {
            activePlayers.append( plr );
            plr->setInPlay( false );
        } else {
            inactivePlayers.append( plr );
        }
    }


    // iterate through the list of planets and
    // mark their owners in play
    PlanetListIterator nextPlanet( planets );

    Planet *planet;
    while( (planet = nextPlanet()) ) {
        planet->getPlayer()->setInPlay( true );
    }


    PlayerListIterator nextActivePlayer( activePlayers );
    while( (plr = nextActivePlayer()) ) {
        if( !plr->isInPlay() ) {
            // Player has bitten the dust
            QString msg;
            msg = i18n("The once mighty empire of %1 has fallen in ruins.");
            gameMsg(msg, plr);
        }
    }

    PlayerListIterator nextInactivePlayer( inactivePlayers );
    while( (plr = nextInactivePlayer()) ) {
        if( plr->isInPlay() ) {
            // Player has bitten the dust
            QString msg;
            msg = i18n("The fallen empire of %1 has staggered back to life.");
            gameMsg(msg, plr);
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

    if( (*arrivingFleet->owner) == (*arrivingFleet->destination->getPlayer())) {
        if (!arrivingFleet->owner->isAiPlayer()) {
        	arrivingFleet->destination->getFleet().absorb(arrivingFleet);

        	QString msg;
        	msg = i18n("Reinforcements (%1 ships) have arrived for planet %2.")
        	      .arg(arrivingFleet->getShipCount());
                gameMsg(msg, 0, arrivingFleet->destination);
        }
    } else {

        // let's get ready to rumble...

        CoreLogic cl;
        AttackFleet &attacker = *arrivingFleet;
        DefenseFleet &defender = arrivingFleet->destination->getFleet();
        Planet &prizePlanet = *(arrivingFleet->destination);

        bool haveVictor = false;
        bool planetHolds = true;

        while( !haveVictor ) {
            double attackerRoll = cl.roll();
            double defenderRoll = cl.roll();

            if( defenderRoll < prizePlanet.getKillPercentage() ) {
                attacker.removeShips( 1 );
            }

            if( attacker.getShipCount() <= 0 ) {
                haveVictor = true;
                planetHolds = true;
                continue;
            }

            if( attackerRoll < attacker.killPercentage ) {
                defender.removeShips( 1 );
                attacker.owner->statEnemyShipsDestroyed( 1 );
            }

            if( defender.getShipCount() <= 0 ) {
                haveVictor = true;
                planetHolds = false;
            }
        }

        if( planetHolds ) {
            prizePlanet.getPlayer()->statEnemyFleetsDestroyed(1);
            QString msg;
            msg = i18n("Planet %2 has held against an attack from %1.");
            gameMsg(msg, attacker.owner, &prizePlanet);
        } else {
            Player *defender = prizePlanet.getPlayer();
            attacker.owner->statEnemyFleetsDestroyed( 1 );

            arrivingFleet->destination->conquer( arrivingFleet );

            QString msg;
            msg = i18n("Planet %2 has fallen to %1.");
            gameMsg(msg, attacker.owner, &prizePlanet, defender);
        }
    }

    mapWidget->repaint(true);
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

    planetInfo->setPlanetList(planets);

    shipCountEdit->hide();
    endTurn->setEnabled( true );

    currentPlayer = new PlayerListIterator( players );
    currentPlayer->toFirst();

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

    delete currentPlayer;
    currentPlayer = NULL;

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
        if( (*planet->getPlayer()) == (*currentPlayer->current()) ) {
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
        setBackgroundColor( black );
    } else {
        mapWidget->hide();
        planetInfo->hide();
        gameMessage->hide();
        endTurn->hide();
        shipCountEdit->hide();
        splashScreen->show();
        setBackgroundColor( black );
    }

}

//************************************************************************
// Player clicked the 'End Turn' button
//************************************************************************
void
GameBoard::nextPlayer()
{
    // end turn and advance to next player
    Player *plr;

    while( (plr = ++(*currentPlayer)) && !(plr->isInPlay()) ) {}

    if( !plr ) {
        // end of player list, new turn
        currentPlayer->toFirst();
        nextTurn();
    }

    if( gameInProgress ) {
       if (currentPlayer->current()->isAiPlayer()) {
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

    ok = currentPlayer->current()->NewAttack( source, dest,
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
  FleetDlg *fleetDlg = new FleetDlg( this, &(currentPlayer->current()->getAttackList()) );
  fleetDlg->show();
}
