#include <qpixmap.h>
#include <qpainter.h>

#include <kapp.h>
#include <kiconloader.h>

#include <iostream.h>

#include "map_widget.h"
#include "map_widget.moc"

ConquestMap::ConquestMap(  Map *newMap, QWidget *parent )
    : QTableView( parent ),
    SECTOR_HEIGHT( 28 ), SECTOR_WIDTH( 28 ),
    BOARD_HEIGHT( newMap->getRows() * SECTOR_HEIGHT ),
    BOARD_WIDTH( newMap->getColumns() * SECTOR_WIDTH ),
    map( newMap ), gridColor( 50, 80, 50 ),
    labelFont("Helvetica", 12 ),
    hiLiteRow( -1 ), hiLiteCol( -1 )
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
    
    QTimer *timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(squareBlink()) );
    timer->start( 500, false );

    setMouseTracking( true );
    
    show();

};

ConquestMap::~ConquestMap()
{
}


void
ConquestMap::mousePressEvent( QMouseEvent *e )
{
    int row, col;

    row = findRow( e->y() );
    col = findCol( e->x() );
    
    if( map->getSector( row, col ).hasPlanet() ) {
        emit planetSelected( map->getSector( row, col ).getPlanet() );
    }
}

void
ConquestMap::mouseMoveEvent( QMouseEvent *e )
{
    // highlight the square under the mouse
    int row, col;

    row = findRow( e->y() );
    col = findCol( e->x() );


    if( (hiLiteRow != -1) && (hiLiteCol != -1)  ) {
        QPainter p( this );

        int x, y;
        colXPos( hiLiteCol, &x );
        rowYPos( hiLiteRow, &y );

        p.translate( x,y );

        drawSector( &p, map->getSector(hiLiteRow,hiLiteCol) );

        hiLiteRow = -1;
        hiLiteCol = -1;

     }


    if( map->getSector( row, col ).hasPlanet() ) {
        QPainter p( this );

        int x, y;
        colXPos( col, &x );
        rowYPos( row, &y );

        p.translate( x,y );

        drawSector( &p, map->getSector(row,col), false, true );

        emit planetHighlighted(map->getSector( row, col ).getPlanet() );
        
        hiLiteRow = row;
        hiLiteCol = col;

    }
}

void
ConquestMap::unselectPlanet( void )
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
        QPainter p( this );

        int x, y;
        colXPos( col, &x );
        rowYPos( row, &y );

        p.translate( x,y );

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
    repaint( false );
}


void
ConquestMap::drawSector( QPainter *p, Sector &sector, bool borderStrobe, bool highlight )
{
    int x, y;  // sector graphic coords

    QColor labelColor( white );
    QPoint labelCorner;
    
    // calculate the x,y coordinates of the sector
    colXPos( sector.getRow(), &x );
    rowYPos( sector.getColumn(), &y );

    if( sector.hasPlanet() ) {
        KIconLoader *iconloader = KApplication::getKApplication()->getIconLoader();
        QPixmap pm;

        // simple (pathetic) way to "randomize"
        // the planet graphic
        switch( ((sector.getRow()+sector.getColumn()) % 9) + 1  ) {
        case 1 :
            pm = iconloader->loadApplicationIcon( IMAGE_PLANET_1 );
            labelCorner = QPoint( 18, 14 );
            break;
        case 2 :
            pm = iconloader->loadApplicationIcon( IMAGE_PLANET_2 );
            labelCorner = QPoint( 2, 14 );
            break;
        case 3 :
            pm = iconloader->loadApplicationIcon( IMAGE_PLANET_3 );
            labelCorner = QPoint( 2, 26 );
            break;
        case 4 :
            pm = iconloader->loadApplicationIcon( IMAGE_PLANET_4 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 5 :
            pm = iconloader->loadApplicationIcon( IMAGE_PLANET_5 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 6 :
            pm = iconloader->loadApplicationIcon( IMAGE_PLANET_6 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 7 :
            pm = iconloader->loadApplicationIcon( IMAGE_PLANET_7 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 8 :
            pm = iconloader->loadApplicationIcon( IMAGE_PLANET_8 );
            labelCorner = QPoint( 18, 26 );
            break;
        case 9 :
            pm = iconloader->loadApplicationIcon( IMAGE_PLANET_9 );
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

