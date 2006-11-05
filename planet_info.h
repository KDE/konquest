#ifndef _PLANET_INFO_H_
#define _PLANET_INFO_H_

#include <QFrame>
#include <QString>
#include <qpalette.h>
#include <QLabel>

#include "planet.h"

class PlanetInfo : public QFrame
{
    Q_OBJECT
        
public:
    PlanetInfo( QWidget *parent, QPalette palette );
    virtual ~PlanetInfo();

    QSize sizeHint() const;
    
public slots:
    void showPlanet( Planet * );

private:
    void clearDisplay();

    QLabel *infosLabel;
};

#endif  // _PLANET_INFO_H_

