#include <QLabel>
#include <QLayout>
#include <qslider.h>

#include <kapplication.h>
#include <kconfig.h>
#include <klocale.h>
#include <kstdguiitem.h>
#include <kvbox.h>
#include <kglobal.h>

#include "gameenddlg.h"
#include "gameenddlg.moc"

GameEndDlg::GameEndDlg( QWidget *parent )
    : KDialog( parent)
{
    setCaption(i18n("Out of Turns"));
    setButtons(KDialog::Yes|KDialog::No);
    setDefaultButton(KDialog::Yes);
    showButtonSeparator(true);
    KVBox *page = new KVBox(this);
    setMainWidget(page);

    // Create controls
    QLabel *label1 = new QLabel( i18n("This is the last turn.\nDo you wish to add extra turns?")+"\n\n", page );
    label1->setAlignment( Qt::AlignCenter );

    turnCountLbl = new QLabel( page );
    turnCount = new QSlider( Qt::Horizontal, page );
    turnCount->setRange( 1, 40 );
    turnCount->setPageStep( 1 );
    turnCount->setValue( 5 );

    KGuiItem addTurns(i18n("&Add Turns"), QString::null, QString::null,
                      i18n("Add the specified number of turns to the game and continue playing."));
    KGuiItem gameOver(i18n("&Game Over"), QString::null, QString::null,
                      i18n("Terminate the current game."));

    setButtonGuiItem(KDialog::Yes, addTurns);
    setButtonGuiItem(KDialog::No, gameOver);

    init();

    connect( turnCount, SIGNAL(valueChanged( int )), this, SLOT(turnCountChange( int )) );
}

GameEndDlg::~GameEndDlg()
{
}

void
GameEndDlg::init()
{
    KConfig *config = KGlobal::config();
    config->setGroup("Game");
    int turns = config->readEntry("ExtraTurns", 10);
    turnCount->setValue(turns);
    turnCountChange(turns);
}

void
GameEndDlg::slotYes()
{
    KConfig *config = KGlobal::config();
    config->setGroup("Game");
    config->writeEntry("ExtraTurns", extraTurns());
    config->sync();
    KDialog::slotButtonClicked(KDialog::Yes);
}

int
GameEndDlg::extraTurns()
{
    return turnCount->value();
}

void
GameEndDlg::turnCountChange( int newTurnCount )
{
    QString newLbl = i18n("Extra turns: %1", newTurnCount );
    turnCountLbl->setText( newLbl);
}
