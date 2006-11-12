#ifndef _MAPSCENE_H
#define _MAPSCENE_H

#include <KSvgRenderer>
#include <QGraphicsScene>

class Planet;
class Map;

class MapScene: public QGraphicsScene {
    Q_OBJECT
    public:
        MapScene(Map *map);
        KSvgRenderer *renderer() { return m_renderer; }
        Map *map() { return m_map; }
        
    signals:
        void  planetSelected( Planet * );
        
    private:
        Map *m_map;
        KSvgRenderer *m_renderer;
        
    private slots:
        void mapUpdate();
        void planetItemSelected (Planet *);
};

#endif
