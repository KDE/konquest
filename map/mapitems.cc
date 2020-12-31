/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2008-2009 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>
    SPDX-FileCopyrightText: 2011 Jeffrey Kelling <overlordapophis@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mapitems.h"

#include <QBrush>
#include <QGraphicsScene>
#include <QPainter>
#include <QTimer>

#include <KLocalizedString>
#include <KColorScheme>

#include "mapscene.h"
#include "map.h"
#include "../planet.h"

/********************************
    PlanetItem
 *******************************/

PlanetItem::PlanetItem (MapScene *scene, Sector *sector, Game *game)
    : QGraphicsObject(),
      m_scene(scene),
      m_sector(sector),
      m_game(game),
      m_hovered(false),
      m_selected(false),
      m_blinkState(false)
{
    if (m_sector->planet() != nullptr) {
        m_lookName = QStringLiteral("planet_%1").arg(m_sector->planet()->planetLook() + 1);
    }
    setAcceptHoverEvents(true);

    m_blinkTimer = new QTimer(this);
    connect(m_blinkTimer, &QTimer::timeout, this, &PlanetItem::blinkPlanet);
    connect(m_sector, &Sector::update, this, &PlanetItem::updatePlanet);
}


void PlanetItem::updatePlanet()
{
    Planet  *planet = m_sector->planet();
    if (planet != nullptr) {
        m_lookName = QStringLiteral("planet_%1").arg(planet->planetLook() + 1);
        update();
    }
}


QRectF PlanetItem::boundingRect() const
{
    qreal size = m_scene->getSectorSize();
    return QRectF(m_sector->coord().y() * size + m_scene->itemsHorizontalOffset(),
                  m_sector->coord().x() * size + m_scene->itemsVerticalOffset(),
                  size,
                  size);
}

void PlanetItem::paint(QPainter *p, const QStyleOptionGraphicsItem * /*option*/,
                       QWidget * /*widget*/)
{
    if(!m_sector->planet())
        return;
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
    qreal sectorSize = m_scene->getSectorSize();
    QPointF sectorTopLeft(m_sector->coord().y() * sectorSize + m_scene->itemsHorizontalOffset(),
                          m_sector->coord().x() * sectorSize + m_scene->itemsVerticalOffset());

    QPixmap planetPix = renderPixmap(m_lookName, sectorSize, sectorSize);
    p->drawPixmap(sectorTopLeft, planetPix);

    if ( m_hovered || (m_selected && m_blinkState) ) {
        QBrush  backBrush = p->brush();

        backBrush.setColor(KColorScheme(QPalette::Active).background().color());
        backBrush.setStyle(Qt::SolidPattern);

        p->setOpacity(0.3);
        p->fillRect(boundingRect(), backBrush );
        p->setOpacity(1);
    }

    // Show the name of the planet (on top of bkgnd)

    QRectF TextRect(sectorTopLeft.x(), sectorTopLeft.y(), sectorSize, sectorSize);

    QPixmap nameBackgroundPix = renderPixmap(QStringLiteral("planet_name_background"), sectorSize, sectorSize);
    p->drawPixmap(TextRect.topLeft(), nameBackgroundPix);
    p->setFont(QFont(QStringLiteral("Times"), 16));
    p->drawText(TextRect, m_sector->planet()->name());

    // Show the number of ships on the planet.
    if((m_game->options().NeutralsShowShips || !m_sector->planet()->player()->isNeutral())
       && ((!m_game->options().BlindMap || m_game->currentPlayer() == m_sector->planet()->player())
           || (m_game->options().NeutralsShowShips && m_sector->planet()->player()->isNeutral())))
    {
        QString shipCount = QString::number(m_sector->planet()->ships());

        QPixmap shipsBackgroundPix = renderPixmap(QStringLiteral("planet_ship_count_background"),
                                                  sectorSize, sectorSize);
        p->drawPixmap(TextRect.topLeft(), shipsBackgroundPix);
        p->setFont(QFont(QStringLiteral("Times"), 16));
        p->drawText(TextRect, Qt::AlignRight | Qt::AlignBottom, shipCount);
    }
}

QPixmap PlanetItem::renderPixmap( const QString& svgId, int width, int height ) const
{
    QPixmap pix;
    QString cacheKey = QStringLiteral("%1%2x%3").arg(svgId).arg(width).arg(height);
    if (!m_scene->imageCache()->findPixmap(cacheKey, &pix)) {
        pix = QPixmap(width, height);
        pix.fill(Qt::transparent);
        QPainter pixPainter(&pix);
        m_scene->renderer()->render(&pixPainter, svgId, QRect(0, 0, width, height));
        m_scene->imageCache()->insertPixmap(cacheKey, pix);
    }

    return pix;
}


void PlanetItem::hoverEnterEvent( QGraphicsSceneHoverEvent * /*event*/ )
{
    m_hovered = true;

    Planet  *planet = m_sector->planet();
    m_scene->displayPlanetInfo(planet);

    update();
}

void PlanetItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * /*event*/ )
{
    m_hovered = false;
    m_scene->displayPlanetInfo(nullptr);

    update();
}


void PlanetItem::mousePressEvent( QGraphicsSceneMouseEvent * /*event*/ )
{
    m_selected = true;
    m_blinkTimer->start(500);
    update();

    Q_EMIT planetItemSelected(this);
}

void PlanetItem::select(  )
{
    m_selected = true;
    m_blinkTimer->start(500);
    update();
}

void PlanetItem::unselect() {
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


PlanetInfoItem::PlanetInfoItem (Game *game)
  : QGraphicsItem(),
    m_game(game),
    m_textDoc(),
    m_planet(nullptr)
{
}

void PlanetInfoItem::setPlanet (Planet *planet)
{
    m_planet = planet;

    QString  text = i18n("Planet name: %1", planet->name());
    if((m_game->options().NeutralsShowStats || !planet->player()->isNeutral())
       && ((!m_game->options().BlindMap || m_game->currentPlayer() == planet->player())
           || (m_game->options().NeutralsShowStats && planet->player()->isNeutral())))
    {
        text += QLatin1String("<br />") + i18n("Owner: %1", planet->player()->coloredName()
          + (m_game->options().NeutralsShowShips || !planet->player()->isNeutral() ?
             QLatin1String("<br />")
          + i18n("Ships: %1", planet->ships() ) :
             QString()));
        if( m_game->currentPlayer() == planet->player() )
        {
            int shipsNeeded = 0; // determine how many ships will be needed by standing orders
            for (AttackFleet* fleet : planet->player()->standingOrders()) {
                if(fleet->source == planet)
                    shipsNeeded += fleet->shipCount();
            }
            if(shipsNeeded)
                text += QLatin1String("<br />") + i18nc("regarding standing orders", "Ships due: %1", shipsNeeded);
        }
        text += QLatin1String("<br />")
          + i18n("Production: %1", planet->production())
          + QLatin1String("<br />")
          + i18n("Kill percent: %1", planet->killPercentage() );
    }
    m_textDoc.setHtml(text);
}


QRectF PlanetInfoItem::boundingRect() const
{
    return QRectF(0, 0, m_textDoc.idealWidth(), m_textDoc.size().height());
}

void PlanetInfoItem::paint(QPainter *p,
                           const QStyleOptionGraphicsItem * /*option*/,
                           QWidget * /*widget*/)
{
    QBrush  brush = p->brush();

    brush.setColor(KColorScheme(QPalette::Active).background().color());
    brush.setStyle(Qt::SolidPattern);

    p->setOpacity(0.7);
    p->fillRect(QRectF(0, 0,
                       m_textDoc.idealWidth() + 1,
                       m_textDoc.size().height() + 1),
                brush);
    p->setOpacity(1.0);

    m_textDoc.drawContents(p);
}
