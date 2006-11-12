#ifndef _MAPSCENE_H
#define _MAPSCENE_H

#include <KSvgRenderer>
#include <QGraphicsScene>

class Planet;
class PlanetItem;
class Map;

class MapScene: public QGraphicsScene {
    Q_OBJECT
    public:
        MapScene(Map *map);
        KSvgRenderer *renderer() { return m_renderer; }
        Map *map() { return m_map; }
        void unselectPlanet();
        
    signals:
        void  planetSelected( Planet * );
        
    private:
        Map *m_map;
        KSvgRenderer *m_renderer;
        PlanetItem *m_selectedPlanetItem;
        
    private slots:
        void mapUpdate();
        void planetItemSelected (PlanetItem *);
};

#endif
