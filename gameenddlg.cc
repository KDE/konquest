#include <kslider.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbt.h>
#include <kapp.h>

#include "gameenddlg.h"
#include "gameenddlg.moc"

GameEndDlg::GameEndDlg( QWidget *parent )
    : QDialog( parent, 0, true )
{
    // Create controls
    QLabel *label1 = new QLabel( i18n("This is the last turn.\nDo you wish to add more turns?"), this );
    label1->setAlignment( AlignCenter );
    
    turnCount = new KSlider( 1, 40, 1, 5, KSlider::Horizontal, this );
    turnCountLbl = new QLabel( this );

    QPushButton *yesBtn = new QPushButton( i18n("Yes"), this );
    yesBtn->setFixedSize( yesBtn->sizeHint() );
    yesBtn->setAutoDefault( true );

    QPushButton *noBtn = new QPushButton( i18n("No"), this );
    noBtn->setFixedSize( noBtn->sizeHint() );

    turnCountChange( 5 );
    
    // Layout controls
    QBoxLayout *layout1 = new QVBoxLayout( this );
    QBoxLayout *layout2 = new QHBoxLayout;

    layout1->addWidget( label1 );
    layout1->addWidget( turnCountLbl );
    layout1->addWidget( turnCount );

    layout1->addLayout( layout2 );

    layout2->addSpacing( 5 );
    layout2->addWidget( yesBtn );
    layout2->addStretch( 0 );
    layout2->addWidget( noBtn );
    layout2->addSpacing( 5 );


    connect( yesBtn, SIGNAL(clicked()), this, SLOT(accept()) );
    connect( noBtn, SIGNAL(clicked()), this, SLOT(reject()) );
    connect( turnCount, SIGNAL(valueChanged( int )), this, SLOT(turnCountChange( int )) );

    setFixedSize( 250, 170 );
}

GameEndDlg::~GameEndDlg()
{
}

void
GameEndDlg::turnCountChange( int newTurnCount )
{
    QString newLbl;
    
    addTurns = newTurnCount;

    newLbl.sprintf( i18n("Add %d turns"), addTurns );

    turnCountLbl->setText( newLbl);

}






