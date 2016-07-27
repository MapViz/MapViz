#ifndef MAPACTION_H
#define MAPACTION_H
#include "mapview.h"
class MapView;

class MapAction
{
public:
    MapAction();
    virtual ~MapAction(){}
    MapView* mapView() {return mMapView;}
    void setMapView(MapView *mapview){ mMapView = mapview; }

    virtual void paintEvent(QPaintEvent *evt) = 0;
    virtual void mousePressEvent(QMouseEvent* evt) = 0;
    virtual void mouseMoveEvent(QMouseEvent* evt) = 0;
    virtual void mouseReleaseEvent(QMouseEvent* evt) = 0;
    virtual void wheelEvent(QWheelEvent* evt) = 0;

protected:
    MapView* mMapView;

};

#endif // MAPACTION_H
