#ifndef _MINIMAP_H
#define _MINIMAP_H

#include <qwidget.h>
#include <qframe.h>
#include <qpixmap.h>
#include <qtablevw.h>
#include <qtimer.h>

#include "gamecore.h"
#include "images.h"


class MiniMap : public QTableView
{
    Q_OBJECT

    // Constructors
public:
    MiniMap( Map *newMap, QWidget *parent = 0 );
    virtual ~MiniMap();

protected:
    void paintCell( QPainter *p, int row, int col );

private slots:
    void mapUpdate();

private:
    const int SECTOR_HEIGHT;
    const int SECTOR_WIDTH;

    const int BOARD_HEIGHT;
    const int BOARD_WIDTH;

    void drawSector( QPainter *, Sector & );

    Map *map;
};


#endif // _MINIMAP_H_
