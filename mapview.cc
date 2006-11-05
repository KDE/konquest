#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QTimer>

#include <kapplication.h>
#include <kiconloader.h>
#include <kglobalsettings.h>

#include <kdebug.h>
#include "mapview.h"
#include "planet.h"
#include "player.h"
#include "map.h"
#include "sector.h"

MapView::MapView(  Map *newMap, QWidget *parent )
    : QFrame( parent ),
    SECTOR_HEIGHT( 28 ), SECTOR_WIDTH( 28 ),
    BOARD_HEIGHT( newMap->rows() * SECTOR_HEIGHT ),
    BOARD_WIDTH( newMap->columns() * SECTOR_WIDTH ),
    map( newMap ), gridColor( 50, 80, 50 ),
    hiLiteCoord( -1, -1 )
{
    labelFont = KGlobalSettings::generalFont();
    labelFont.setPointSize( 8 );

    setFixedSize( BOARD_WIDTH, BOARD_HEIGHT );

    connect( map, SIGNAL( update() ), this, SLOT( mapUpdate() ) );

    QTimer *timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(squareBlink()) );
    timer->start( 500 );

    setMouseTracking( true );
}

MapView::~MapView()
{
}

void
MapView::mousePressEvent( QMouseEvent *e )
{
    Coordinate c( e->y()/SECTOR_HEIGHT, e->x()/SECTOR_WIDTH );

    if( map->getSector( c ).hasPlanet() ) {
        emit planetSelected( map->getSector( c ).planet() );
    }
}

void
MapView::mouseMoveEvent( QMouseEvent *e )
{
    // highlight the square under the mouse
    Coordinate c( e->y()/SECTOR_HEIGHT, e->x()/SECTOR_WIDTH );

    // Check to make sure the mouse is in a valid grid location
    if( (c.y() < 0 || c.x() < 0) ||
        (c.y() >= BOARD_ROWS || c.x() >= BOARD_COLS) ) {
        return;
    }


    if( (hiLiteCoord.x() != -1) && (hiLiteCoord.y() != -1) ) {
        update( hiLiteCoord.y() * SECTOR_WIDTH, hiLiteCoord.x() * SECTOR_HEIGHT, SECTOR_WIDTH, SECTOR_HEIGHT );

        hiLiteCoord = Coordinate(-1,-1);
    }

    if( map->getSector( c ).hasPlanet() ) {
        update( c.y() * SECTOR_WIDTH, c.x() * SECTOR_HEIGHT, SECTOR_WIDTH, SECTOR_HEIGHT );

        emit planetHighlighted(map->getSector(c).planet() );

        hiLiteCoord = c;
    }
}

void
MapView::unselectPlanet()
{
    map->setSelectedSector();
}


void
MapView::paintEvent( QPaintEvent *ev )
{
    QRect r = ev->rect();
    int startRow = r.y() / SECTOR_HEIGHT;
    int endRow   = (r.y() + r.height()) / SECTOR_HEIGHT;
    int startCol = r.x() / SECTOR_WIDTH;
    int endCol = (r.x() + r.width()) / SECTOR_WIDTH;

    QPainter p(this);
    for(int r=startRow; r<endRow; ++r)
        for(int c=startCol;c<endCol; ++c)
            drawSector( &p, map->getSector( Coordinate(r,c) ) );
}

void
MapView::squareBlink()
{
    static bool blinkState = true;

    Coordinate c;
    if( map->selectedSector( c ) ) {
        update( c.y() * SECTOR_WIDTH, c.x() * SECTOR_HEIGHT, SECTOR_WIDTH, SECTOR_HEIGHT );
    }

    blinkState = !blinkState;
}


void
MapView::mapUpdate()
{
    update();
}


void
MapView::drawSector( QPainter *p, Sector &sector )
{
    QColor labelColor( Qt::white );
    QPoint labelCorner;

    QPoint sectorTopLeft(sector.coord().x() * SECTOR_WIDTH,
			 sector.coord().y() * SECTOR_HEIGHT);

    p->eraseRect( sectorTopLeft.x(), sectorTopLeft.y(), SECTOR_WIDTH, SECTOR_HEIGHT );

    if( sector.hasPlanet() ) {
        QPixmap pm;
        
        // simple (pathetic) way to "randomize"
        // the planet graphic
        // and also a really dirty hack to make the planet
        // name more visible (hard coded pixel offsets)
        
        switch( ((sector.coord().x()+sector.coord().y()) % 9) + 1  ) {
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

        p->drawPixmap( sectorTopLeft+pos, pm, QRect(0, 0, pm.width(), pm.height() ) );

        p->setFont( labelFont );
        p->setPen( labelColor );

        p->drawText( sectorTopLeft+labelCorner, sector.planet()->name() );

        QRect secRect = QRect(sectorTopLeft, QSize(SECTOR_WIDTH, SECTOR_HEIGHT ));
        bool doHighlight = secRect.contains( mapFromGlobal( QCursor::pos() ) );

        if( !doHighlight ) {
            QPen gridPen( sector.planet()->player()->color() );
            p->setPen( gridPen );
        } else {
            QPen gridPen( Qt::white );
            p->setPen( gridPen );
        }

    } else {
        QPen gridPen( gridColor );

        p->setPen( gridPen );
    }

    p->drawRect( sectorTopLeft.x(), sectorTopLeft.y(), SECTOR_WIDTH-1, SECTOR_HEIGHT-1 );

}

#include "mapview.moc"
