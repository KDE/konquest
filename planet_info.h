#ifndef _PLANET_INFO_H_
#define _PLANET_INFO_H_

#include <q3frame.h>
#include <qstring.h>
#include <q3ptrlist.h>
#include <qpalette.h>
//Added by qt3to4:
#include <QLabel>

#include "gamecore.h"

class QLabel;

struct planet_info_buffer {
    Planet *planet;
    int production;
    int ships;
    float killRate;
};

typedef Q3PtrList<planet_info_buffer> PlanetInfoList;
typedef Q3PtrListIterator<planet_info_buffer> PlanetInfoListIterator;

class PlanetInfo : public Q3Frame
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

    QLabel *name;
    QLabel *owner;
    QLabel *ships;
    QLabel *production;
    QLabel *kill_percent;
};

#endif  // _PLANET_INFO_H_

