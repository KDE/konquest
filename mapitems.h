/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright <inge@lysator.liu.se>
    Copyright <pinaraf@gmail.com>

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
#ifndef _MAPITEMS_H_
#define _MAPITEMS_H_


#include <QGraphicsObject>
#include <QTextDocument>
#include <QObject>
#include "sector.h"


class MapScene;

class PlanetItem : public QGraphicsObject
{
    Q_OBJECT

public:
    PlanetItem(MapScene *scene, Sector *sector, GameLogic *gamelogic);
    ~PlanetItem() {}

    QRectF   boundingRect() const;
    void     paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

    void     hoverEnterEvent ( QGraphicsSceneHoverEvent *event );
    void     hoverLeaveEvent ( QGraphicsSceneHoverEvent *event );
    void     mousePressEvent ( QGraphicsSceneMouseEvent *event );
    void     unselect ();
    void     select ();
    Sector  *sector () { return m_sector; }

signals:
    void     planetItemSelected (PlanetItem *);

private slots:
    void     updatePlanet ();
    void     blinkPlanet ();

private:
    QPixmap renderPixmap( const QString& svgId, int width, int height ) const;

        MapScene  *m_scene;
        Sector    *m_sector;

        GameLogic *m_gamelogic;

        bool       m_hovered;
        bool       m_selected;
        bool       m_blinkState;
        QTimer    *m_blinkTimer;
        QString    m_lookName;
};


class PlanetInfoItem : public QGraphicsItem
{
public:
    PlanetInfoItem(GameLogic*);
    ~PlanetInfoItem() {}

    QRectF   boundingRect() const;
    void     setPlanet (Planet *planet);
    Planet  *planet () { return m_planet; }
    void     paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

private:
    GameLogic     *m_gamelogic;
    QTextDocument  m_textDoc;
    Planet        *m_planet;
};


#endif
