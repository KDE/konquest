/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>

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
#ifndef KONQUEST_MAPITEMS_H
#define KONQUEST_MAPITEMS_H


#include <QGraphicsObject>
#include <QTextDocument>
#include "sector.h"

class Game;
class MapScene;

class PlanetItem : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit PlanetItem(MapScene *scene, Sector *sector, Game *game);
    ~PlanetItem() {}

    QRectF   boundingRect() const override;
    void     paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;

    void     hoverEnterEvent ( QGraphicsSceneHoverEvent *event ) override;
    void     hoverLeaveEvent ( QGraphicsSceneHoverEvent *event ) override;
    void     mousePressEvent ( QGraphicsSceneMouseEvent *event ) override;
    void     unselect ();
    void     select ();
    Sector  *sector () { return m_sector; }

Q_SIGNALS:
    void     planetItemSelected (PlanetItem *);

private:
    void     updatePlanet ();
    void     blinkPlanet ();

private:
    QPixmap renderPixmap( const QString& svgId, int width, int height ) const;

    MapScene  *m_scene;
    Sector    *m_sector;

    Game      *m_game;

    bool       m_hovered;
    bool       m_selected;
    bool       m_blinkState;
    QTimer    *m_blinkTimer;
    QString    m_lookName;
};


class PlanetInfoItem : public QGraphicsItem
{
public:
    explicit PlanetInfoItem(Game*);
    ~PlanetInfoItem() {}

    QRectF   boundingRect() const override;
    void     setPlanet (Planet *planet);
    Planet  *planet () { return m_planet; }
    void     paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;

private:
    Game          *m_game;
    QTextDocument  m_textDoc;
    Planet        *m_planet;
};

#endif // KONQUEST_MAPITEMS_H
