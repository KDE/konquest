#include <qpixmap.h>
#include <qpainter.h>
#include <qcolor.h>

#include <kapplication.h>
#include <kiconloader.h>
#include <kglobalsettings.h>

#include <kglobal.h>
#include "map_widget.moc"

ConquestMap::ConquestMap(  Map *newMap, QWidget *parent )
    : QGridView( parent ),
    SECTOR_HEIGHT( 28 ), SECTOR_WIDTH( 28 ),
    BOARD_HEIGHT( newMap->getRows() * SECTOR_HEIGHT ),
    BOARD_WIDTH( newMap->getColumns() * SECTOR_WIDTH ),
    map( newMap ), gridColor( 50, 80, 50 ),
    hiLiteRow( -1 ), hiLiteCol( -1 )
{
    labelFont = KGlobalSettings::generalFont();
    labelFont.setPointSize( 8 );

    setFrameStyle( NoFrame );
    setPaletteBackgroundColor( black );
    setMinimumSize( BOARD_HEIGHT, BOARD_WIDTH );

    setCellWidth( SECTOR_WIDTH );
    setCellHeight( SECTOR_HEIGHT );
    setNumRows( map->getRows() );
    setNumCols( map->getColumns() );

    setMinimumSize( BOARD_HEIGHT, BOARD_WIDTH );
    setMaximumSize( BOARD_HEIGHT, BOARD_WIDTH );

    connect( map, SIGNAL( update() ), this, SLOT( mapUpdate() ) );

    QTimer *timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(squareBlink()) );
    timer->start( 500, false );

    viewport()->setMouseTracking( true );
    setMouseTracking( true );
	
    show();


}

ConquestMap::~ConquestMap()
{
}

	
void
ConquestMap::contentsMousePressEvent( QMouseEvent *e )
{
    int row, col;

    row = rowAt( e->y() );
    col = columnAt( e->x() );

    if( map->getSector( row, col ).hasPlanet() ) {
        emit planetSelected( map->getSector( row, col ).getPlanet() );
    }

}

void
ConquestMap::contentsMouseMoveEvent( QMouseEvent *e )
{
    // highlight the square under the mouse
    int row, col;

    row = rowAt( e->y() );
    col = columnAt( e->x() );

    // Check to make sure the mouse is in a valid grid location
    if( (row < 0 || col < 0) ||
    	(row >= BOARD_ROWS || col >= BOARD_COLS) )  {
	    return;
    }


    if( (hiLiteRow != -1) && (hiLiteCol != -1)  ) {
        QPainter p( viewport() );

        p.translate( hiLiteCol * cellWidth(), hiLiteRow * cellHeight() );

        drawSector( &p, map->getSector(hiLiteRow,hiLiteCol) );

        hiLiteRow = -1;
        hiLiteCol = -1;

     }

    if( map->getSector( row, col ).hasPlanet() ) {
        QPainter p( viewport() );

        p.translate( col * cellWidth(),row * cellHeight() );

        drawSector( &p, map->getSector(row,col), false, true );
        emit planetHighlighted(map->getSector( row, col ).getPlanet() );

        hiLiteRow = row;
        hiLiteCol = col;

    }

}

void
ConquestMap::unselectPlanet()
{
    map->setSelectedSector();
}


void
ConquestMap::paintCell( QPainter *p, int row, int col )
{
    drawSector( p, map->getSector( row, col ) );
}

void
ConquestMap::squareBlink()
{
    static bool blinkState = true;

    int row, col;
    if( map->selectedSector( row, col ) ) {
        QPainter p( this, true );

        p.translate( col * cellWidth(), row * cellHeight() );

        if( blinkState ) {
            drawSector( &p, map->getSector(row,col), true );
        } else {
            drawSector( &p, map->getSector(row,col), false );
        }
    }

    if( blinkState )
        blinkState = false;
    else
        blinkState = true;
}


void
ConquestMap::mapUpdate()
{
    viewport()->repaint(false);
}


void
ConquestMap::drawSector( QPainter *p, Sector &sector, bool borderStrobe, bool highlight )
{
    QColor labelColor( white );
    QPoint labelCorner;

    if( sector.hasPlanet() ) {
        QPixmap pm;

        // simple (pathetic) way to "randomize"
        // the planet graphic
	// and also a really dirty hack to make the planet
	// name more visible (hard coded pixel offsets)
        switch( ((sector.getRow()+sector.getColumn()) % 9) + 1  ) {
        case 1 :
            pm = QPixmap( IMAGE_PLANET_1 );
            labelCorner = QPoint( 18, 14 );
            break;
        case 2 :
            pm = QPixmap( IMAGE_PLANET_2 );
            labelCorner = QPoint( 2, 14 );
            break;
        case 3 :
            pm = QPixmap( IMAGE_PLANET_3 );
            labelCorner = QPoint( 2, 26 );
            break;
        case 4 :
            pm = QPixmap( IMAGE_PLANET_4 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 5 :
            pm = QPixmap( IMAGE_PLANET_5 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 6 :
            pm = QPixmap( IMAGE_PLANET_6 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 7 :
            pm = QPixmap( IMAGE_PLANET_7 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 8 :
            pm = QPixmap( IMAGE_PLANET_8 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 9 :
            pm = QPixmap( IMAGE_PLANET_9 );
            labelCorner = QPoint( 18, 26 );
            break;
        }

        QPoint pos;

        pos.setX( ( SECTOR_HEIGHT / 2 ) - ( pm.height() / 2 ) );
        pos.setY( ( SECTOR_WIDTH / 2 ) - ( pm.width() / 2 ) );

        p->drawPixmap( pos, pm, QRect(0, 0, pm.height(), pm.width() ) );

        p->setFont( labelFont );
        p->setPen( labelColor );
	
        p->drawText( labelCorner, sector.getPlanet()->getName() );

        if( borderStrobe ) {
            QPen gridPen( sector.getPlanet()->getPlayer()->getColor() );
            p->setPen( gridPen );
        } else if( highlight ) {
            QPen gridPen( white );
            p->setPen( gridPen );
        } else {
            QPen gridPen( gridColor );
            p->setPen( gridPen );
        }

    } else {
        p->eraseRect( 0, 0, SECTOR_WIDTH, SECTOR_HEIGHT );

        QPen gridPen( gridColor );

        p->setPen( gridPen );
    }

    p->drawRect( 0, 0, SECTOR_HEIGHT, SECTOR_WIDTH );

}


