#include <qpixmap.h>
#include <qpainter.h>
#include <QColor>
//Added by qt3to4:
#include <QMouseEvent>

#include <kapplication.h>
#include <kiconloader.h>
#include <kglobalsettings.h>

#include <kglobal.h>
#include "map_widget.moc"

ConquestMap::ConquestMap(  Map *newMap, QWidget *parent )
    : Q3GridView( parent ),
    SECTOR_HEIGHT( 28 ), SECTOR_WIDTH( 28 ),
    BOARD_HEIGHT( newMap->getRows() * SECTOR_HEIGHT ),
    BOARD_WIDTH( newMap->getColumns() * SECTOR_WIDTH ),
    map( newMap ), gridColor( 50, 80, 50 ),
    hiLiteCoord( -1, -1 )
{
    labelFont = KGlobalSettings::generalFont();
    labelFont.setPointSize( 8 );

    setFrameStyle( NoFrame );
    QPalette pal = palette();
    pal.setColor( backgroundRole(), Qt::black );
    setPalette(pal);
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
    timer->start( 500 );

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
    Coordinate c( rowAt( e->y() ), columnAt( e->x() ));

    if( map->getSector( c ).hasPlanet() ) {
        emit planetSelected( map->getSector( c ).getPlanet() );
    }

}

void
ConquestMap::contentsMouseMoveEvent( QMouseEvent *e )
{
    // highlight the square under the mouse
    Coordinate c( rowAt( e->y() ), columnAt( e->x() ));

    // Check to make sure the mouse is in a valid grid location
    if( (c.y() < 0 || c.x() < 0) ||
        (c.y() >= BOARD_ROWS || c.x() >= BOARD_COLS) ) {
        return;
    }


    if( (hiLiteCoord.x() != -1) && (hiLiteCoord.y() != -1) ) {
        QPainter p( viewport() );

        p.translate( hiLiteCoord.y() * cellWidth(), hiLiteCoord.x() * cellHeight() );

        drawSector( &p, map->getSector(hiLiteCoord) );

        hiLiteCoord = Coordinate(-1,-1);
    }

    if( map->getSector( c ).hasPlanet() ) {
        QPainter p( viewport() );

        p.translate( c.y() * cellWidth(),c.x() * cellHeight() );

        drawSector( &p, map->getSector(c), false, true );
        emit planetHighlighted(map->getSector(c).getPlanet() );

        hiLiteCoord = c;
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
    drawSector( p, map->getSector( Coordinate(row, col) ) );
}

void
ConquestMap::squareBlink()
{
    static bool blinkState = true;

    Coordinate c;
    if( map->selectedSector( c ) ) {
        QPainter p( this );

        p.translate( c.y() * cellWidth(), c.x() * cellHeight() );
        drawSector( &p, map->getSector(c), blinkState );
    }

    blinkState = !blinkState;
}


void
ConquestMap::mapUpdate()
{
    viewport()->repaint();
}


void
ConquestMap::drawSector( QPainter *p, Sector &sector, bool borderStrobe, bool highlight )
{
    QColor labelColor( Qt::white );
    QPoint labelCorner;

    if( sector.hasPlanet() ) {
        QPixmap pm;

        // simple (pathetic) way to "randomize"
        // the planet graphic
        // and also a really dirty hack to make the planet
        // name more visible (hard coded pixel offsets)
        switch( ((sector.getCoord().x()+sector.getCoord().y()) % 9) + 1  ) {
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
            QPen gridPen( Qt::white );
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


