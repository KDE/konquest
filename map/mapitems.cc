/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2008-2009 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>
    Copyright 2011 Jeffrey Kelling <overlordapophis@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "mapitems.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QBrush>
#include <QTimer>

#include <kiconloader.h>
#include <kglobalsettings.h>
#include <klocale.h>
#include <kdebug.h>
#include <kcolorscheme.h>

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
    if (m_sector->planet() != NULL) {
        m_lookName = QString("planet_%1").arg(m_sector->planet()->planetLook() + 1);
    }
    setAcceptsHoverEvents(true);

    m_blinkTimer = new QTimer(this);
    connect(m_blinkTimer, SIGNAL(timeout()), this, SLOT(blinkPlanet()));
    connect(m_sector,     SIGNAL(update()),  this, SLOT(updatePlanet()));
}


void PlanetItem::updatePlanet()
{
    Planet  *planet = m_sector->planet();
    if (planet != NULL) {
        m_lookName = QString("planet_%1").arg(planet->planetLook() + 1);
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

    QPixmap nameBackgroundPix = renderPixmap("planet_name_background", sectorSize, sectorSize);
    p->drawPixmap(TextRect.topLeft(), nameBackgroundPix);
    p->setFont(QFont("Times", 16));
    p->drawText(TextRect, m_sector->planet()->name());

    // Show the number of ships on the planet.
    if((m_game->options().NeutralsShowShips || !m_sector->planet()->player()->isNeutral())
       && ((!m_game->options().BlindMap || m_game->currentPlayer() == m_sector->planet()->player())
           || (m_game->options().NeutralsShowShips && m_sector->planet()->player()->isNeutral())))
    {
        QString shipCount = QString::number(m_sector->planet()->ships());

        QPixmap shipsBackgroundPix = renderPixmap("planet_ship_count_background",
                                                  sectorSize, sectorSize);
        p->drawPixmap(TextRect.topLeft(), shipsBackgroundPix);
        p->setFont(QFont("Times", 16));
        p->drawText(TextRect, Qt::AlignRight | Qt::AlignBottom, shipCount);
    }
}

QPixmap PlanetItem::renderPixmap( const QString& svgId, int width, int height ) const
{
    QPixmap pix;
    QString cacheKey = QString("%1%2x%3").arg(svgId).arg(width).arg(height);
    if (!m_scene->pixmapCache()->find(cacheKey, pix)) {
        pix = QPixmap(width, height);
        pix.fill(Qt::transparent);
        QPainter pixPainter(&pix);
        m_scene->renderer()->render(&pixPainter, svgId, QRect(0, 0, width, height));
        m_scene->pixmapCache()->insert(cacheKey, pix);
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
    m_scene->displayPlanetInfo(NULL);

    update();
}


void PlanetItem::mousePressEvent( QGraphicsSceneMouseEvent * /*event*/ )
{
    m_selected = true;
    m_blinkTimer->start(500);
    update();

    emit planetItemSelected(this);
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
    m_planet(NULL)
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
        text += QString("<br />" + i18n("Owner: %1", planet->player()->coloredName())
          + (m_game->options().NeutralsShowShips || !planet->player()->isNeutral() ?
             QString("<br />"
          + i18n("Ships: %1", planet->ships() )) :
             QString()));
        if( m_game->currentPlayer() == planet->player() )
        {
            int shipsNeeded = 0; // determine how many ships will be needed by standing orders
            foreach(AttackFleet* fleet, planet->player()->standingOrders()) {
                if(fleet->source == planet)
                    shipsNeeded += fleet->shipCount();
            }
            if(shipsNeeded)
                text += QString("<br />" + i18nc("regarding standing orders", "Ships due: %1", shipsNeeded));
        }
        text += QString("<br />"
          + i18n("Production: %1", planet->production() )
          + "<br />"
          + i18n("Kill percent: %1", planet->killPercentage() ));
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
