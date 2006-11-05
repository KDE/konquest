#include <QLabel>
#include <QLayout>
#include <qpalette.h>
#include <QColor>
#include <QVBoxLayout>
#include <QFrame>
#include <kapplication.h>
#include <klocale.h>

#include "planet_info.h"
#include <kglobal.h>
#include "planet_info.moc"
#include "player.h"

PlanetInfo::PlanetInfo( QWidget *parent, QPalette palette )
    : QFrame( parent  )
{
    setPalette( palette );

    infosLabel = new QLabel( this );
    infosLabel->setMinimumWidth( 100 );
    
    clearDisplay();

    QVBoxLayout *layout1 = new QVBoxLayout( this );

    layout1->addWidget( infosLabel );
    layout1->addStretch(1);

    setMouseTracking( true );

    setMinimumSize( sizeHint() );
    setMaximumHeight( sizeHint().height() );
}

PlanetInfo::~PlanetInfo()
{
}

QSize PlanetInfo::sizeHint() const
{
    return infosLabel->sizeHint();
}

void PlanetInfo::clearDisplay()
{
    QString temp;
    
    temp = i18n("Planet name: ") + "<br />";
    temp = temp + i18n("Owner: ") + "<br />";
    temp = temp  + i18n("Ships: ") + "<br />";
    temp = temp  + i18n("Production: ") + "<br />";
    temp = temp  + i18n("Kill percent: ") + "<br />";
    infosLabel->setText( temp );
}

void PlanetInfo::showPlanet( Planet *planet )
{
    QString temp = i18n("Planet name: %1", planet->name()) + "<br />";
    if( !planet->player()->isNeutral() ) {
        temp = temp + i18n("Owner: %1", planet->player()->coloredName()) + "<br />";
        temp = temp + i18n("Ships: %1", KGlobal::locale()->formatNumber(planet->fleet().shipCount(), 0) ) + "<br />";
        temp = temp + i18n("Production: %1", KGlobal::locale()->formatNumber(planet->production(), 0) ) + "<br />";
        temp = temp + i18n("Kill percent: %1", KGlobal::locale()->formatNumber(planet->killPercentage(), 3) ) + "<br />";
    }
    infosLabel->setText( temp );
}

