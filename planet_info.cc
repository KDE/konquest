#include <qlabel.h>
#include <qlayout.h>
#include <qcolor.h>
#include <qpalette.h>
#include <kapp.h>

#include <iostream.h>

#include "planet_info.h"
#include "planet_info.moc"

PlanetInfo::PlanetInfo( QWidget *parent, QPalette palette )
    : QFrame( parent  )
{
    setPalette( palette );

    name = new QLabel( this );
    name->setPalette( palette );

    owner = new QLabel( this );
    owner->setPalette( palette );

    ships = new QLabel( this );
    ships->setPalette( palette );

    production = new QLabel( this );
    production->setPalette( palette );

    kill_percent = new QLabel( this );
    kill_percent->setPalette( palette );

    clearDisplay();

    QVBoxLayout *layout = new QVBoxLayout( this );
    layout->addWidget( name );
    layout->addWidget( owner );
    layout->addWidget( ships );
    layout->addWidget( production );
    layout->addWidget( kill_percent );

    setMaximumSize( 32767, 80 );
}

PlanetInfo::~PlanetInfo()
{
    emptyPlanetInfoList();
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

    temp = i18n("Planet Name : ");
    name->setText( temp );

    temp = i18n("Owner : ");
    owner->setText( temp );

    temp = i18n("Ships : ");
    ships->setText( temp );

    temp = i18n("Production : ");
    production->setText( temp );

    temp = i18n("Kill Percent : ");
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
        
        temp = i18n("Planet Name : ") + planet->getName();
        name->setText( temp );
        return;
    }
    
    QString nameToShow = planet->getName();

    PlanetInfoListIterator itr( planet_stats );
    planet_info_buffer *p;

    while( (p = itr()) ) {
        if( p->planet  == planet ) {

            QString temp;

            temp = i18n("Planet Name : ") + p->planet->getName();
            name->setText( temp );

            temp = i18n("Owner : ") + p->planet->getPlayer()->getName();
            owner->setText( temp );

            temp.sprintf( i18n("Ships : %d"),  p->ships );
            ships->setText( temp );

            temp.sprintf( i18n("Production : %d"), p->production );
            production->setText( temp );

            temp.sprintf( i18n("Kill Percent : %.3f"), p->killRate );
            kill_percent->setText( temp );
        }
    }
}


