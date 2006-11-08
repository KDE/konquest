#include <QLabel>
#include <QPalette>
#include <klocale.h>

#include "planet_info.h"
#include <kglobal.h>
#include "planet_info.moc"
#include "player.h"

PlanetInfo::PlanetInfo( QWidget *parent, QPalette palette )
    : QLabel( parent  )
{
    setPalette( palette );

    setText( i18n("Planet information"));
    setMouseTracking( true );
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
    setText( temp );
}

