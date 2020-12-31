/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
