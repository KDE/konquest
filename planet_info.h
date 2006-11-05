#ifndef _PLANET_INFO_H_
#define _PLANET_INFO_H_

#include <QFrame>
#include <QString>
#include <qpalette.h>
#include <QLabel>

#include "planet.h"

class PlanetInfo : public QLabel
{
    Q_OBJECT
        
public:
    PlanetInfo( QWidget *parent, QPalette palette );
    
public slots:
    void showPlanet( Planet * );

};

#endif  // _PLANET_INFO_H_

