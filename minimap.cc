#include <qpixmap.h>
#include <qpainter.h>

#include <kapp.h>
#include <kiconloader.h>

#include <iostream.h>

#include "minimap.h"
#include "minimap.moc"

MiniMap::MiniMap(  Map *newMap, QWidget *parent )
    : QTableView( parent ),
    SECTOR_HEIGHT( 12 ), SECTOR_WIDTH( 12 ),
    BOARD_HEIGHT( newMap->getRows() * SECTOR_HEIGHT ),
    BOARD_WIDTH( newMap->getColumns() * SECTOR_WIDTH ),
    map( newMap )
{
    setBackgroundColor( black );
    setMinimumSize( BOARD_HEIGHT, BOARD_WIDTH );

    setCellWidth( SECTOR_WIDTH );
    setCellHeight( SECTOR_HEIGHT );
    setNumRows( map->getRows() );
    setNumCols( map->getColumns() );

    setMinimumSize( BOARD_HEIGHT, BOARD_WIDTH );
    setMaximumSize( BOARD_HEIGHT, BOARD_WIDTH );

    connect( map, SIGNAL( update() ), this, SLOT( mapUpdate() ) );

    show();
};

MiniMap::~MiniMap()
{
}


void
MiniMap::paintCell( QPainter *p, int row, int col )
{
    drawSector( p, map->getSector( row, col ) );
}

void
MiniMap::mapUpdate()
{
    repaint( false );
}


void
MiniMap::drawSector( QPainter *p, Sector &sector )
{
    int x, y;  // sector graphic coords

    // calculate the x,y coordinates of the sector
    colXPos( sector.getRow(), &x );
    rowYPos( sector.getColumn(), &y );

    QRect r( 0, 0, SECTOR_WIDTH, SECTOR_HEIGHT );

    p->setPen( black );
    p->setBrush( black );
    p->drawRect( r );

    if( sector.hasPlanet() ) {
        p->setPen( sector.getPlanet()->getPlayer()->getColor() );
        p->setBrush( sector.getPlanet()->getPlayer()->getColor() );
        
        p->drawPie( r, 0, (360 * 16)-1 );
    }
}

