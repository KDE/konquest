#ifndef _MAPITEMS_H_
#define _MAPITEMS_H_

#include <QGraphicsItem>
#include <QObject>

class MapScene;
class Sector;

class PlanetItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    public:
        PlanetItem (MapScene *scene, Sector *sector);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget);
        void hoverEnterEvent ( QGraphicsSceneHoverEvent *event );
        void hoverLeaveEvent ( QGraphicsSceneHoverEvent *event );
        void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    signals:
        void planetSelected (Planet *);
    private slots:
        void updatePlanet ();
    private:
        MapScene *m_scene;
        Sector *m_sector;
        bool hovered;
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
