#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QMouseEvent>
#include <QTimer>
#include <QtDebug>

#include <kapplication.h>
#include <kiconloader.h>
#include <kglobalsettings.h>
#include <klocale.h>

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
      m_map( newMap ), 
      m_blinkState(true), 
      m_gridColor( 50, 80, 50 ),
      m_hiLiteCoord( -1, -1 ), 
      m_planetRenderer(IMAGES_SVG)
{
    m_labelFont = KGlobalSettings::generalFont();
    m_labelFont.setPointSize( 8 );

    setMinimumSize( BOARD_WIDTH, BOARD_HEIGHT );
    setFrameStyle(QFrame::Box);
    connect( m_map, SIGNAL( update() ), this, SLOT( mapUpdate() ) );

    m_blinkTimer = new QTimer( this );
    connect( m_blinkTimer, SIGNAL(timeout()), this, SLOT(squareBlink()) );

    setMouseTracking( true );
    
    /*int i = 0;
    while (true) {
        if (planetRenderer.elementExists(QString("planet_%1").arg(i + 1))) {
            qDebug() << i << "does exist";
            i++;
        } else {
            qDebug() << QString("planet_%1").arg(i + 1) << "doesn't";
            break;
        }
    }
    qDebug() << i << "planets available...";*/
}

MapView::~MapView()
{
}

void
MapView::mousePressEvent( QMouseEvent *e )
{
    if ((e->x() > BOARD_WIDTH) || (e->y() > BOARD_HEIGHT))
        return;
    Coordinate c( e->y()/SECTOR_HEIGHT, e->x()/SECTOR_WIDTH );

    if( m_map->sector( c )->hasPlanet() ) {
        emit planetSelected( m_map->sector( c )->planet() );
    }
}

void
MapView::resizeEvent (QResizeEvent *e)
{
    int new_board_size = qMin(e->size().width(), e->size().height());
    BOARD_HEIGHT  = new_board_size;
    BOARD_WIDTH   = new_board_size;
    SECTOR_HEIGHT = new_board_size/16;
    SECTOR_WIDTH  = new_board_size/16;
    update(0, 0, new_board_size, new_board_size);
    //qDebug() << e->size() << "=>" << new_board_size;
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

    if( m_hiLiteCoord.x() != -1 && m_hiLiteCoord.y() != -1 ) {
        update( m_hiLiteCoord.y() * SECTOR_WIDTH, 
		m_hiLiteCoord.x() * SECTOR_HEIGHT, SECTOR_WIDTH, SECTOR_HEIGHT );
        m_hiLiteCoord = Coordinate(-1,-1);
    }

    if( m_map->sector( c )->hasPlanet() ) {
        update( c.y() * SECTOR_WIDTH, c.x() * SECTOR_HEIGHT, 
		SECTOR_WIDTH, SECTOR_HEIGHT );

        Planet  *planet = m_map->sector(c)->planet();
        QString  temp = i18n("Planet name: %1", planet->name());
        if( !planet->player()->isNeutral() ) {
            temp += "<br />" + i18n("Owner: %1", planet->player()->coloredName());
            temp += "<br />" + i18n("Ships: %1", KGlobal::locale()->formatNumber(planet->fleet().shipCount(), 0) );
            temp += "<br />" + i18n("Production: %1", KGlobal::locale()->formatNumber(planet->production(), 0) );
            temp += "<br />" + i18n("Kill percent: %1", KGlobal::locale()->formatNumber(planet->killPercentage(), 3) );
        }
        setToolTip(temp);
        emit planetHighlighted(planet );
        m_hiLiteCoord = c;
    }
}

void
MapView::unselectPlanet()
{
    m_blinkTimer->stop ();
    m_blinkState = false;

    Coordinate  c = m_map->selectedSector();
    update( c.x() * SECTOR_WIDTH, c.y() * SECTOR_HEIGHT, 
	    SECTOR_WIDTH, SECTOR_HEIGHT );
    m_map->setSelectedSector();
}


void
MapView::paintEvent( QPaintEvent *ev )
{
    QRect r = ev->rect();
    int startRow = r.y() / SECTOR_HEIGHT;
    int endRow   = qMin((r.y() + r.height()) / SECTOR_HEIGHT, BOARD_ROWS);
    int startCol = r.x() / SECTOR_WIDTH;
    int endCol   = qMin((r.x() + r.width()) / SECTOR_WIDTH, BOARD_COLS);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    for (int r = startRow; r < endRow; ++r)
        for (int c = startCol; c < endCol; ++c)
            drawSector( &p, m_map->sector( Coordinate(r,c) ) );
    if ( m_map->hasSelectedSector() ) {
        drawSector(&p, m_map->sector(m_map->selectedSector()));
    }
}

void
MapView::squareBlink()
{
    if( m_map->hasSelectedSector( ) ) {
        Coordinate  c = m_map->selectedSector();
        update( c.x() * SECTOR_WIDTH, c.y() * SECTOR_HEIGHT, 
		SECTOR_WIDTH, SECTOR_HEIGHT );
    }

    m_blinkState = !m_blinkState;
}


void
MapView::mapUpdate()
{
    if (m_map->hasSelectedSector() && !m_blinkTimer->isActive())
        m_blinkTimer->start(500);
    update();
}


void
MapView::drawSector( QPainter *p, Sector *sector )
{
    QColor labelColor( Qt::white );
    QPoint sectorTopLeft(sector->coord().x() * SECTOR_WIDTH,
			 sector->coord().y() * SECTOR_HEIGHT);

    p->eraseRect( sectorTopLeft.x(), sectorTopLeft.y(), SECTOR_WIDTH, SECTOR_HEIGHT );
    
    if( sector->hasPlanet() ) {
        QRect secRect = QRect(sectorTopLeft, QSize(SECTOR_WIDTH, SECTOR_HEIGHT ));
        QPen gridPen;
        if( secRect.contains( mapFromGlobal( QCursor::pos() ) )
	    || m_blinkState && (m_map->selectedSector() == sector->coord()) ) {
            gridPen = QPen( Qt::white );
        } else {
            gridPen = QPen( sector->planet()->player()->color() );
        }
        p->setPen(gridPen);
        
	//p->drawRect( sectorTopLeft.x(), sectorTopLeft.y(), 
	//	     SECTOR_WIDTH-1, SECTOR_HEIGHT-1 );
        if (!sector->planet()->player()->isNeutral()) {
            QBrush backBrush = p->brush();
            backBrush.setColor(gridPen.color());
            backBrush.setStyle(Qt::SolidPattern);
            //p->setOpacity(0.5);
#if 1
	    // Show the owner of the planet by painting the background
	    // of the planet with the appropriate color.
            p->fillRect(sectorTopLeft.x(), sectorTopLeft.y(), 
			SECTOR_WIDTH-1, SECTOR_HEIGHT-1, backBrush );
#else
	    // Add a colored circle around the planet to show the owner.
            QPainterPath path;
            path.addEllipse(sectorTopLeft.x()-1, sectorTopLeft.y()-1,
			    SECTOR_WIDTH+1, SECTOR_HEIGHT+1);
            p->fillPath(path, backBrush);
#endif
            p->setOpacity(1);
        }
        int planetLook = sector->planet()->planetLook();
        
        m_planetRenderer.render(p, QString("planet_%1").arg(planetLook + 1),
				QRect(sectorTopLeft.x(), sectorTopLeft.y(), 
				      SECTOR_WIDTH, SECTOR_HEIGHT ));
        p->setFont( m_labelFont );
        p->setPen( Qt::white );

	// Show the name of the planet.
        p->drawText( sectorTopLeft + QPoint(2, 12), sector->planet()->name() );

	// Show the number of ships on the planet.
        if (!sector->planet()->player()->isNeutral()) {
            QString shipCount = QString::number(sector->planet()
						->fleet().shipCount());
            QFontMetrics m = p->fontMetrics();
            
            p->drawText( sectorTopLeft + QPoint(SECTOR_WIDTH
						- m.width(shipCount), 
						SECTOR_HEIGHT), shipCount );
        }

    }
#if 1
    else {
	// Draw a grid around each sector to make a better overview.
        QPen gridPen( m_gridColor );
        p->setPen( gridPen );
        p->drawRect( sectorTopLeft.x(), sectorTopLeft.y(),
		     SECTOR_WIDTH-1, SECTOR_HEIGHT-1 );
    }
#endif
}

#include "mapview.moc"
