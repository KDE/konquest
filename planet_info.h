#ifndef _PLANET_INFO_H_
#define _PLANET_INFO_H_

#include <qframe.h>
#include <qstring.h>
#include <qlist.h>
#include <qpalette.h>

#include "gamecore.h"

struct planet_info_buffer {
    Planet *planet;
    int production;
    int ships;
    float killRate;
};

typedef QList<planet_info_buffer> PlanetInfoList;
typedef QListIterator<planet_info_buffer> PlanetInfoListIterator;

class PlanetInfo : public QFrame
{
    Q_OBJECT
        
public:
    PlanetInfo( QWidget *parent, QPalette palette );
    virtual ~PlanetInfo();

    void setPlanetList( PlanetList &newPlanets );
    void rescanPlanets( void );
    
public slots:
    void showPlanet( Planet * );
    
private:
    void emptyPlanetInfoList( void );
    void clearDisplay( void );
    
    PlanetList *planets;
    PlanetInfoList planet_stats;

    QLabel *name;
    QLabel *owner;
    QLabel *ships;
    QLabel *production;
    QLabel *kill_percent;
};

#endif  // _PLANET_INFO_H_

