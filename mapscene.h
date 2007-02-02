#ifndef _MAPSCENE_H
#define _MAPSCENE_H


#include <KSvgRenderer>

#include <QGraphicsScene>


class Planet;
class PlanetItem;
class PlanetInfoItem;
class Map;


class MapScene: public QGraphicsScene
{
    Q_OBJECT

    public:
        explicit MapScene(Map *map);
	~MapScene() {}

        KSvgRenderer  *renderer() const  { return m_renderer; }
        Map           *map()      const  { return m_map; }

        void           unselectPlanet();
        void           displayPlanetInfo(Planet *planet, const QPointF & pos);
        void           displayPlanetInfo(Planet *planet);
        
        void           drawBackground( QPainter * painter, const QRectF & rect );
        
    signals:
        void  planetSelected( Planet * );
        
    private slots:
        void  mapUpdate();
        void  planetItemSelected (PlanetItem *);
        
    private:
        Map             *m_map;
        KSvgRenderer    *m_renderer;
        PlanetItem      *m_selectedPlanetItem;
        PlanetInfoItem  *m_planetInfoItem;
};

#endif
