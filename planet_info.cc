#include <QLabel>
#include <QLayout>
#include <qpalette.h>
#include <QColor>
//Added by qt3to4:
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
    emptyPlanetInfoList();
}

QSize PlanetInfo::sizeHint() const
{
    return infosLabel->sizeHint();
}

void PlanetInfo::setPlanetList( PlanetList &newPlanets )
{
    emptyPlanetInfoList();

    PlanetListIterator itr( newPlanets );

    Planet *p;
    while( (p = itr()) ) {
        planet_info_buffer *stats = new planet_info_buffer;
        stats->planet = p;
        planet_stats.append( stats );
    }

    rescanPlanets();
}

void PlanetInfo::rescanPlanets()
{
    PlanetInfoListIterator itr( planet_stats );
    planet_info_buffer *p;

    while( (p = itr()) ) {
        p->production = p->planet->getProduction();
        p->ships = p->planet->getFleet().getShipCount();
        p->killRate = p->planet->getKillPercentage();
    }
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

void PlanetInfo::emptyPlanetInfoList()
{
    planet_stats.first();

    planet_info_buffer *p;
    while( (p = planet_stats.take()) ) {
        delete p;
    }

}

void PlanetInfo::showPlanet( Planet *planet )
{
    if( planet->getPlayer()->isNeutral() ) {
        clearDisplay();

        QString temp;

        temp = i18n("Planet name: %1", planet->getName());
        infosLabel->setText( temp );
        return;
    }

    QString nameToShow = planet->getName();

    PlanetInfoListIterator itr( planet_stats );
    planet_info_buffer *p;

    while( (p = itr()) ) {
        if( p->planet  == planet ) {

            QString temp;

            temp = i18n("Planet name: %1", p->planet->getName()) + "<br />";
            temp = temp + i18n("Owner: %1", p->planet->getPlayer()->getColoredName()) + "<br />";
            temp = temp + i18n("Ships: %1", KGlobal::locale()->formatNumber(p->ships, 0) ) + "<br />";
            temp = temp + i18n("Production: %1", KGlobal::locale()->formatNumber(p->production, 0) ) + "<br />";
            temp = temp + i18n("Kill percent: %1", KGlobal::locale()->formatNumber(p->killRate, 3) ) + "<br />";
            infosLabel->setText( temp );
        }
    }
}

