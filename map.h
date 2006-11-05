#ifndef __MAP_H__
#define __MAP_H__

#include <QObject>
#include "sector.h"
#include "player.h"

//*****************************************************************
// class Map
//*****************************************************************

class Map : public QObject
{
    Q_OBJECT

public:
    Map();
    virtual ~Map();

    const int rows() const { return m_rows; }
    const int columns() const { return m_columns; }

    void populateMap( PlayerList &players, Player *neutral,
                      int numNeutralPlanets, PlanetList &thePlanets );
    void clearMap();
    
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
    void Freeze();
    void Thaw();
    bool freezeUpdates;

private:
    Sector &findRandomFreeSector();
    
protected:
    Sector grid[BOARD_ROWS][BOARD_COLS];  // a map is a 2-D array of sectors;
    const int m_rows; // size of grid in sectors
    const int m_columns;

    // This is used to implement a selected sector,
    // one who's boarder flashes.
    bool hasSelectedSector;
    Coordinate  sel;
};

#endif
