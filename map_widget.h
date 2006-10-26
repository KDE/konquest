#ifndef _MAP_WIDGET_H
#define _MAP_WIDGET_H


#include <QFrame>

#include "gamecore.h"
#include "images.h"

class ConquestMap : public QFrame
{
    Q_OBJECT

    // Constructors
public:
    ConquestMap( Map *newMap, QWidget *parent = 0 );
    virtual ~ConquestMap();

    // Interface
public:
    void unselectPlanet();

private:
    virtual void mousePressEvent( QMouseEvent *e );
    virtual void mouseMoveEvent( QMouseEvent *e );

private slots:
    void mapUpdate();
    void squareBlink();

signals:
    void planetSelected( Planet * );
    void planetHighlighted( Planet * );

private:
    const int SECTOR_HEIGHT;
    const int SECTOR_WIDTH;

    const int BOARD_HEIGHT;
    const int BOARD_WIDTH;

    void drawSector( QPainter *, Sector & );
    void paintEvent( QPaintEvent* );

    Map *map;
    QColor gridColor;
    QFont labelFont;

    Coordinate hiLiteCoord;
};


#endif
