#include <qlabel.h>
#include <qlayout.h>
#include <qcolor.h>
#include <qpalette.h>
#include <qcolor.h>
#include <kapplication.h>
#include <klocale.h>

#include "planet_info.h"
#include <kglobal.h>
#include "planet_info.moc"

PlanetInfo::PlanetInfo( QWidget *parent, QPalette palette )
    : QFrame( parent  )
{
    setPalette( palette );

    name = new QLabel( this );
    name->setMinimumWidth( 100 );
    owner = new QLabel( this );
    owner->setMinimumWidth( 100 );
    ships = new QLabel( this );
    ships->setMinimumWidth( 100 );
    production = new QLabel( this );
    production->setMinimumWidth( 100 );
    kill_percent = new QLabel( this );
    kill_percent->setMinimumWidth( 100 );

    clearDisplay();

    QVBoxLayout *layout1 = new QVBoxLayout( this );
    
    layout1->addWidget( name );
    layout1->addWidget( owner );
    layout1->addWidget( ships );
    layout1->addWidget( production );
    layout1->addWidget( kill_percent );
    layout1->addStretch(1);

    setMouseTracking( true );

    setMaximumSize( sizeHint() );
}

PlanetInfo::~PlanetInfo()
{
    emptyPlanetInfoList();
}

QSize PlanetInfo::sizeHint() const
{
    int height;
    
    height = name->sizeHint().height() +
    		owner->sizeHint().height() +
		ships->sizeHint().height() +
		production->sizeHint().height()+
		kill_percent->sizeHint().height();
		
    return QSize( 100, height );
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

void PlanetInfo::rescanPlanets( void )
{
    PlanetInfoListIterator itr( planet_stats );
    planet_info_buffer *p;

    while( (p = itr()) ) {
        p->production = p->planet->getProduction();
        p->ships = p->planet->getFleet().getShipCount();
        p->killRate = p->planet->getKillPercentage();
    }
}

void PlanetInfo::clearDisplay( void )
{
    QString temp;

    temp = i18n("Planet name: ");
    name->setText( temp );

    temp = i18n("Owner: ");
    owner->setText( temp );

    temp = i18n("Ships: ");
    ships->setText( temp );

    temp = i18n("Production: ");
    production->setText( temp );

    temp = i18n("Kill percent: ");
    kill_percent->setText( temp );

}

void PlanetInfo::emptyPlanetInfoList( void )
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

        temp = i18n("Planet name: %1").arg(planet->getName());
        name->setText( temp );
        return;
    }

    QString nameToShow = planet->getName();

    PlanetInfoListIterator itr( planet_stats );
    planet_info_buffer *p;

    while( (p = itr()) ) {
        if( p->planet  == planet ) {

            QString temp;

            temp = i18n("Planet name: %1").arg(p->planet->getName());
            name->setText( temp );

            temp = i18n("Owner: %1").arg(p->planet->getPlayer()->getName());
            owner->setText( temp );

            temp = i18n("Ships: %1").arg( KGlobal::locale()->formatNumber(p->ships, 0) );
            ships->setText( temp );

            temp = i18n("Production: %1").arg( KGlobal::locale()->formatNumber(p->production, 0) );
            production->setText( temp );

            temp = i18n("Kill percent: %1").arg( KGlobal::locale()->formatNumber(p->killRate, 3) );
            kill_percent->setText( temp );
        }
    }
}

