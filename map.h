#ifndef __MAP_H__
#define __MAP_H__

#include <QObject>
#include "sector.h"
#include "player.h"

// Board Size Constants
#define BOARD_ROWS 16
#define BOARD_COLS 16


//*****************************************************************
// class Map
//*****************************************************************


class Map : public QObject
{
    Q_OBJECT

public:
    Map();
    virtual ~Map();

    const int  rows()    const { return m_rows;    }
    const int  columns() const { return m_columns; }

    void       clearMap();
    void       populateMap( PlayerList &players, Player *neutral,
			    int numNeutralPlanets, PlanetList &thePlanets );
    
    double     distance( Planet *p1, Planet *p2 );

    bool selectedSector( Coordinate &c ) const;
    void setSelectedSector( Coordinate c );
    void setSelectedSector( const Planet & );
    void setSelectedSector();

    Sector &getSector( Coordinate c );

protected slots:
    void childSectorUpdate();

signals:
    void update();

protected:
    void  Freeze();
    void  Thaw();

private:
    Sector *findRandomFreeSector();
    
protected:
    // A map is a 2-D array of Sectors.
    Sector     m_grid[BOARD_ROWS][BOARD_COLS];
    const int  m_rows; // size of grid in sectors
    const int  m_columns;

    // To inhibit updates on the view.
    bool  m_freezeUpdates;

    // This is used to implement a selected sector,
    // one who's boarder flashes.
    bool        m_hasSelectedSector;
    Coordinate  m_selection;
};

#endif
