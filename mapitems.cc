#include <QGraphicsScene>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QtDebug>
#include <QTimer>
#include <QTextDocument>

#include <kapplication.h>
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
      hovered(false),
      selected(false),
      blinkState(false)
{
    setAcceptsHoverEvents(true);
    connect(m_sector, SIGNAL(update()), this, SLOT(updatePlanet()));
    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinkPlanet()));
}

void PlanetItem::updatePlanet() {
    Planet  *planet = m_sector->planet();
    if (planet != NULL) {
        update();
    }
}

QRectF PlanetItem::boundingRect() const {
    return QRectF(m_sector->coord().x() * m_scene->width()/BOARD_COLS, m_sector->coord().y() * m_scene->height()/BOARD_ROWS, m_scene->width()/BOARD_COLS, m_scene->height()/BOARD_ROWS);
}

void PlanetItem::paint(QPainter *p, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) {
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
    int planetLook = m_sector->planet()->planetLook();
    m_scene->renderer()->render(p, QString("planet_%1").arg(planetLook + 1), boundingRect());
    
    if (hovered || (selected && blinkState)) {
        QBrush backBrush = p->brush();
        backBrush.setColor(Qt::white);
        backBrush.setStyle(Qt::SolidPattern);
        p->setOpacity(0.3);
        p->fillRect(boundingRect(), backBrush );
        p->setOpacity(1);
    }
    
    // Show the name of the planet.
    QPoint sectorTopLeft (m_sector->coord().x() * m_scene->width()/BOARD_COLS, m_sector->coord().y() * m_scene->height()/BOARD_ROWS);
    p->drawText( sectorTopLeft + QPoint(2, 12), m_sector->planet()->name() );

    // Show the number of ships on the planet.
    if (!m_sector->planet()->player()->isNeutral()) {
        QString shipCount = QString::number(m_sector->planet()->fleet().shipCount());
        QFontMetrics m = p->fontMetrics();
        p->drawText( sectorTopLeft + QPoint(m_scene->width()/BOARD_COLS - m.width(shipCount), m_scene->height()/BOARD_ROWS), shipCount );
    }
}

void PlanetItem::hoverEnterEvent ( QGraphicsSceneHoverEvent */*event*/ ) {
    hovered = true;
    Planet  *planet = m_sector->planet();
    m_scene->displayPlanetInfo(planet, QPoint(m_sector->coord().x() * m_scene->width()/BOARD_COLS, m_sector->coord().y() * m_scene->height()/BOARD_ROWS));
    update();
}

void PlanetItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent */*event*/ ) {
    hovered = false;
    m_scene->displayPlanetInfo(NULL, QPoint());
    update();
}

void PlanetItem::mousePressEvent ( QGraphicsSceneMouseEvent * /*event*/ ) {
    selected = true;
    blinkTimer->start(500);
    update();
    emit planetItemSelected(this);
}

void PlanetItem::unselect() {
    // Unselect...
    blinkTimer->stop();
    blinkState = false;
    selected = false;
    update();
}

void PlanetItem::blinkPlanet() {
    blinkState = !blinkState;
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

void PlanetInfoItem::setPlanet (Planet *planet) {
    m_planet = planet;
    QString text = i18n("Planet name: %1", planet->name());
    if( !planet->player()->isNeutral() ) {
        text += "<br />" + i18n("Owner: %1", planet->player()->coloredName());
        text += "<br />" + i18n("Ships: %1", KGlobal::locale()->formatNumber(planet->fleet().shipCount(), 0) );
        text += "<br />" + i18n("Production: %1", KGlobal::locale()->formatNumber(planet->production(), 0) );
        text += "<br />" + i18n("Kill percent: %1", KGlobal::locale()->formatNumber(planet->killPercentage(), 3) );
    }
    m_textDoc.setHtml(text);
}

QRectF PlanetInfoItem::boundingRect() const {
    return QRectF(0, 0, m_textDoc.idealWidth(), m_textDoc.size().height());
}

void PlanetInfoItem::paint(QPainter *p, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) {
    QPen pen = p->pen();
    QBrush brush = p->brush();
    p->setOpacity(0.7);
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    p->fillRect(QRectF(0, 0, m_textDoc.idealWidth() + 1, m_textDoc.size().height() + 1), brush);
    p->setOpacity(1.0);
    m_textDoc.drawContents(p);
}

/********************************
    BackgroundItem
 *******************************/

BackgroundItem::BackgroundItem (MapScene *scene)
    : QGraphicsItem(),
      m_scene(scene)
{
    
}

QRectF BackgroundItem::boundingRect() const {
    return QRectF(0, 0, m_scene->width(), m_scene->height());
}

void BackgroundItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qreal s_w = m_scene->width()/BOARD_COLS;
    qreal s_h = m_scene->height()/BOARD_ROWS;
    m_scene->renderer()->render(painter, "background", QRectF(0, 0, 16*s_w, 16*s_h));
    QPen pen = painter->pen();
    pen.setColor(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->setOpacity(0.5);
    for (int i = 0 ; i <= BOARD_COLS ; i++) {
        painter->drawLine(i*s_w, 0, i*s_w, 16*s_h);
    }
    for (int j = 0 ; j <= BOARD_ROWS ; j++) {
        painter->drawLine(0, j*s_h, 16*s_w, j*s_h);
    }
}
