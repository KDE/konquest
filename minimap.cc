#include <qpixmap.h>
#include <qpainter.h>

#include <kapplication.h>
#include <kiconloader.h>

#include "minimap.h"
#include "minimap.moc"

MiniMap::MiniMap(  QWidget *parent, const char *name )
    : QGridView( parent, name ),
    SECTOR_HEIGHT( 12 ), SECTOR_WIDTH( 12 ),
    BOARD_HEIGHT( 10 * SECTOR_HEIGHT ),
    BOARD_WIDTH( 10 * SECTOR_WIDTH ),
    map( 0 )
{
    setFrameStyle( NoFrame );
    setPaletteBackgroundColor( black );
    setMinimumSize( BOARD_HEIGHT, BOARD_WIDTH );

    setCellWidth( SECTOR_WIDTH );
    setCellHeight( SECTOR_HEIGHT );
    setNumRows( 10 );
    setNumCols( 10 );

    setMinimumSize( BOARD_HEIGHT, BOARD_WIDTH );
    setMaximumSize( BOARD_HEIGHT, BOARD_WIDTH );
}

void
MiniMap::setMap(Map *newMap)
{
    map = newMap;
    BOARD_HEIGHT = map->getRows() * SECTOR_HEIGHT;
    BOARD_WIDTH  = map->getColumns() * SECTOR_WIDTH;
    setNumRows( map->getRows() );
    setNumCols( map->getColumns() );

    setMinimumSize( BOARD_HEIGHT, BOARD_WIDTH );
    setMaximumSize( BOARD_HEIGHT, BOARD_WIDTH );

    connect( map, SIGNAL( update() ), this, SLOT( mapUpdate() ) );
}

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
    updateContents();
}


void
MiniMap::drawSector( QPainter *p, Sector &sector )
{
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

