#ifndef _MAPVIEW_H
#define _MAPVIEW_H

#include <QGraphicsView>

class MapScene;

class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapView( MapScene* scene, QWidget *parent = 0 );
    
private:
    virtual void resizeEvent( QResizeEvent* );
    virtual QSize sizeHint() const;

    MapScene* m_scene;
};

#endif
