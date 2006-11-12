#ifndef _MAPSCENE_H
#define _MAPSCENE_H

#include <KSvgRenderer>
#include <QGraphicsScene>

class Planet;
class PlanetItem;
class PlanetInfoItem;
class Map;

class MapScene: public QGraphicsScene {
    Q_OBJECT
    public:
        MapScene(Map *map);
        KSvgRenderer *renderer() { return m_renderer; }
        Map *map() { return m_map; }
        void unselectPlanet();
        void displayPlanetInfo (Planet *planet, QPoint pos);
        
    signals:
        void  planetSelected( Planet * );
        
    private:
        Map *m_map;
        KSvgRenderer *m_renderer;
        PlanetItem *m_selectedPlanetItem;
        PlanetInfoItem *m_planetInfoItem;
        
    private slots:
        void mapUpdate();
        void planetItemSelected (PlanetItem *);
};

#endif
