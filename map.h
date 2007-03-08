#ifndef __MAP_H__
#define __MAP_H__

#include <QObject>
#include <QList>
#include "sector.h"
#include "player.h"

//*****************************************************************
// class Map
//*****************************************************************


class Map : public QObject
{
    Q_OBJECT

public:
    Map(int rows, int cols);
    virtual ~Map();

    int  rows()     { return m_rows;    }
    int  columns()  { return m_columns; }

    void       clearMap();
    void       resizeMap(int rows, int cols);
    void       populateMap( QList<Player *> &players, Player *neutral,
			    int numNeutralPlanets, QList<Planet *> &thePlanets );
    
    double     distance( Planet *p1, Planet *p2 );

    bool hasSelectedSector() const { return m_hasSelectedSector; }
    Coordinate selectedSector() const { return m_selection; }
    void setSelectedSector( Coordinate c );
    void setSelectedSector();

    Sector *sector( Coordinate coord )  { return &m_grid[coord.y()][coord.x()]; }


protected slots:
    void childSectorUpdate();

signals:
    void update();
    void mapPopulated();

private:
    Sector *findRandomFreeSector();
    
protected:
    // A map is a 2-D array of Sectors.
    QList<QList<Sector> > m_grid;
    int  m_rows; // size of grid in sectors
    int  m_columns;

    // This is used to implement a selected sector,
    // one who's boarder flashes.
    bool        m_hasSelectedSector;
    Coordinate  m_selection;
};

#endif
