#ifndef _PLANET_INFO_H_
#define _PLANET_INFO_H_

#include <QFrame>
#include <QString>
#include <QList>
#include <qpalette.h>
#include <QLabel>

#include "planet.h"


struct planet_info_buffer {
    Planet *planet;
    int production;
    int ships;
    float killRate;
};

typedef QList<planet_info_buffer*> PlanetInfoList;

class PlanetInfo : public QFrame
{
    Q_OBJECT
        
public:
    PlanetInfo( QWidget *parent, QPalette palette );
    virtual ~PlanetInfo();

    void setPlanetList( PlanetList &newPlanets );
    void rescanPlanets();
    QSize sizeHint() const;
    
public slots:
    void showPlanet( Planet * );

private:
    void emptyPlanetInfoList();
    void clearDisplay();
    
    PlanetList *planets;
    PlanetInfoList planet_stats;

    QLabel *infosLabel;
};

#endif  // _PLANET_INFO_H_

