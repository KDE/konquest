#include "mapview.h"
#include "mapview.moc"
#include "mapscene.h"
#include <QGraphicsScene>
#include <QResizeEvent>

MapView::MapView( MapScene* scene, QWidget *parent )
    : QGraphicsView(scene, parent), m_scene(scene)
{
    setCacheMode( QGraphicsView::CacheBackground );
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);

    setMinimumSize( sizeHint() );
    resize( sizeHint() );
}

void MapView::resizeEvent( QResizeEvent* ev )
{
    m_scene->setSceneRect(QRectF(0, 0, ev->size().width(), ev->size().height()));
    QGraphicsView::resizeEvent(ev);
}

QSize MapView::sizeHint() const
{
    return QSize( (int)m_scene->width(), (int)m_scene->height() );
}
