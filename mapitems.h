#ifndef _MAPITEMS_H_
#define _MAPITEMS_H_

#include <QGraphicsItem>
#include <QObject>
#include "sector.h"

class MapScene;

class PlanetItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    public:
        PlanetItem (MapScene *scene, Sector *sector);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget);
        void hoverEnterEvent ( QGraphicsSceneHoverEvent *event );
        void hoverLeaveEvent ( QGraphicsSceneHoverEvent *event );
        void mousePressEvent ( QGraphicsSceneMouseEvent *event );
        void unselect ();
        Sector *sector () { return m_sector; }
    signals:
        void planetItemSelected (PlanetItem *);
    private slots:
        void updatePlanet ();
        void blinkPlanet ();
    private:
        MapScene *m_scene;
        Sector *m_sector;
        bool hovered;
        bool selected;
        bool blinkState;
        QTimer *blinkTimer;
};

class BackgroundItem : public QGraphicsItem {
    public:
        BackgroundItem(MapScene *scene);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget);
    private:
        MapScene *m_scene;
};

#endif
