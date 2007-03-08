#include <QGraphicsScene>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QtDebug>
#include <QTimer>
#include <QTextDocument>

#include <kiconloader.h>
#include <kglobalsettings.h>
#include <klocale.h>
#include <kdebug.h>

#include "mapscene.h"
#include "map.h"
#include "mapitems.moc"


/********************************
    PlanetItem
 *******************************/

PlanetItem::PlanetItem (MapScene *scene, Sector *sector)
    : QObject(scene),
      QGraphicsItem(),
      m_scene(scene),
      m_sector(sector),
      m_hovered(false),
      m_selected(false),
      m_blinkState(false)
{
    setAcceptsHoverEvents(true);
    
    m_blinkTimer = new QTimer(this);
    connect(m_blinkTimer, SIGNAL(timeout()), this, SLOT(blinkPlanet()));
    connect(m_sector,     SIGNAL(update()),  this, SLOT(updatePlanet()));
}


void PlanetItem::updatePlanet()
{
    Planet  *planet = m_sector->planet();
    if (planet != NULL) {
        update();
    }
}


QRectF PlanetItem::boundingRect() const
{
    return QRectF(m_sector->coord().x() * m_scene->width()  / m_scene->map()->columns(),
                  m_sector->coord().y() * m_scene->height() / m_scene->map()->rows(),
                  m_scene->width()  / m_scene->map()->columns(),
                  m_scene->height() / m_scene->map()->rows());
}

void PlanetItem::paint(QPainter *p, const QStyleOptionGraphicsItem * /*option*/,
                       QWidget * /*widget*/)
{
    // Display a frame around the planet
    if (!m_sector->planet()->player()->isNeutral()) {
        QBrush backBrush = p->brush();

        backBrush.setColor(m_sector->planet()->player()->color());
        backBrush.setStyle(Qt::SolidPattern);

        p->setOpacity(0.5);
        p->fillRect(boundingRect(), backBrush );
        p->setOpacity(1);
    }

    // Display the planet
    int  planetLook = m_sector->planet()->planetLook();
    m_scene->renderer()->render(p, QString("planet_%1").arg(planetLook + 1),
                                boundingRect());
    
    if ( m_hovered || (m_selected && m_blinkState) ) {
        QBrush  backBrush = p->brush();

        backBrush.setColor(Qt::white);
        backBrush.setStyle(Qt::SolidPattern);

        p->setOpacity(0.3);
        p->fillRect(boundingRect(), backBrush );
        p->setOpacity(1);
    }
    
    // Show the name of the planet.
    QPointF  sectorTopLeft(m_sector->coord().x() * m_scene->width()/m_scene->map()->columns(),
                          m_sector->coord().y() * m_scene->height()/m_scene->map()->rows());
    p->drawText( sectorTopLeft + QPoint(2, 12), m_sector->planet()->name() );

    // Show the number of ships on the planet.
    if (!m_sector->planet()->player()->isNeutral()) {
        QString       shipCount = QString::number(m_sector->planet()->fleet()
                                                  .shipCount());
        QFontMetrics  m = p->fontMetrics();

        p->drawText( sectorTopLeft
                     + QPointF(m_scene->width()/m_scene->map()->columns() - m.width(shipCount),
                              m_scene->height()/m_scene->map()->rows()), shipCount );
    }
}


void PlanetItem::hoverEnterEvent( QGraphicsSceneHoverEvent * /*event*/ )
{
    m_hovered = true;

    Planet  *planet = m_sector->planet();
    m_scene->displayPlanetInfo(planet,
                               QPointF(m_sector->coord().x() * m_scene->width()/m_scene->map()->columns(),
                                      m_sector->coord().y() * m_scene->height()/m_scene->map()->rows()));

    update();
}

void PlanetItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * /*event*/ )
{
    m_hovered = false;
    m_scene->displayPlanetInfo(NULL, QPoint());

    update();
}


void PlanetItem::mousePressEvent( QGraphicsSceneMouseEvent * /*event*/ )
{
    m_selected = true;
    m_blinkTimer->start(500);
    update();

    emit planetItemSelected(this);
}


void PlanetItem::unselect() {
    // Unselect...
    m_blinkTimer->stop();
    m_blinkState = false;
    m_selected   = false;

    update();
}

void PlanetItem::blinkPlanet()
{
    m_blinkState = !m_blinkState;

    update();
}


/********************************
    PlanetInfoItem
 *******************************/


PlanetInfoItem::PlanetInfoItem ()
  : QGraphicsItem(),
    m_textDoc(),
    m_planet(NULL)
{
}

void PlanetInfoItem::setPlanet (Planet *planet)
{
    m_planet = planet;

    QString  text = i18n("Planet name: %1", planet->name());
    if( !planet->player()->isNeutral() ) {
        text += "<br />" + i18n("Owner: %1", planet->player()->coloredName())
          + "<br />"
          + i18n("Ships: %1", 
                 KGlobal::locale()->formatNumber(planet->fleet().shipCount(), 0) )
          + "<br />"
          + i18n("Production: %1", 
                 KGlobal::locale()->formatNumber(planet->production(), 0) )
          + "<br />"
          + i18n("Kill percent: %1", 
                 KGlobal::locale()->formatNumber(planet->killPercentage(), 3) );
    }
    m_textDoc.setHtml(text);
}


QRectF PlanetInfoItem::boundingRect() const
{
    return QRectF(0, 0, 
                  m_textDoc.idealWidth(), m_textDoc.size().height());
}

void PlanetInfoItem::paint(QPainter *p, 
                           const QStyleOptionGraphicsItem */*option*/, 
                           QWidget */*widget*/)
{
    QPen    pen   = p->pen();
    QBrush  brush = p->brush();

    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);

    p->setOpacity(0.7);
    p->fillRect(QRectF(0, 0, 
                       m_textDoc.idealWidth() + 1,
                       m_textDoc.size().height() + 1), 
                brush);
    p->setOpacity(1.0);

    m_textDoc.drawContents(p);
}
