#include <qlabel.h>
#include <qlayout.h>
#include <qslider.h>
#include <qvbox.h>

#include <kapplication.h>
#include <kconfig.h>
#include <klocale.h>
#include <kstdguiitem.h>

#include "gameenddlg.h"
#include "gameenddlg.moc"

GameEndDlg::GameEndDlg( QWidget *parent )
    : KDialogBase( i18n("Out of Turns"), 
      KDialogBase::Yes|KDialogBase::No, KDialogBase::Yes, KDialogBase::No,
      parent, "end_game_dialog", true, true )
{
    QVBox *page = makeVBoxMainWidget();

    // Create controls
    QLabel *label1 = new QLabel( i18n("This is the last turn.\nDo you wish to add extra turns?")+"\n\n", page );
    label1->setAlignment( AlignCenter );
    
    turnCountLbl = new QLabel( page );
    turnCount = new QSlider( 1, 40, 1, 5, Qt::Horizontal, page );

    KGuiItem addTurns(i18n("&Add Turns"), QString::null, QString::null,
                      i18n("Add the specified number of turns to the game and continue playing."));
    KGuiItem gameOver(i18n("&Game Over"), QString::null, QString::null,
                      i18n("Terminate the current game."));
    
    setButtonGuiItem(KDialogBase::Yes, addTurns);
    setButtonGuiItem(KDialogBase::No, gameOver);

    init();

    connect( turnCount, SIGNAL(valueChanged( int )), this, SLOT(turnCountChange( int )) );
}

GameEndDlg::~GameEndDlg()
{
}

void
GameEndDlg::init()
{
    KConfig *config = kapp->config();
    config->setGroup("Game");
    int turns = config->readNumEntry("ExtraTurns", 10);
    turnCount->setValue(turns);
    turnCountChange(turns);
}

void
GameEndDlg::slotYes()
{
    KConfig *config = kapp->config();
    config->setGroup("Game");
    config->writeEntry("ExtraTurns", extraTurns());
    config->sync();
    KDialogBase::slotYes();
}

int
GameEndDlg::extraTurns()
{
    return turnCount->value();
}

void
GameEndDlg::turnCountChange( int newTurnCount )
{
    QString newLbl = i18n("Extra turns: %1").arg( newTurnCount );
    turnCountLbl->setText( newLbl);
}
