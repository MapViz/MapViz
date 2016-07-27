#ifndef MAPVIEW_H
#define MAPVIEW_H
#include <iostream>
#include <QWidget>
#include <QPixmap>

#include <mapnik/map.hpp>
#include <mapnik/rule.hpp>
#include <mapnik/symbolizer.hpp>
#include <mapnik/feature_type_style.hpp>

class MapAction;

class MapView : public QWidget
{
    Q_OBJECT
public:
    // map tools
    enum mapTool {
        NoTool=1,
        Pan,
        ZoomIn,
        ZoomOut,
        Info,
        Place
    };

    explicit MapView(QWidget *parent = 0);
    void adddatatomap(const QString &datapath);
    void setCurrentTool(mapTool tool);
    void zoomFullExtent();
    void refreshMap();
    void drawFeature(mapnik::feature_ptr feat);
    std::string export2xml();
    QPixmap mapPix();
    mapnik::Map* getMap(){return map;}
    void zoom2Box(QPoint sp,QPoint ep);
    void panMap(QPoint sp, QPoint ep);

    void setMapAction(MapAction * action);
private:

    mapTool currentTool;
    int mStartX;
    int mStartY;
    int mEndX;
    int mEndY;
    bool drag;

    mapnik::Map* map;
    QPixmap mappix;
    mapnik::feature_type_style dfptsty;
    mapnik::feature_type_style dflisty;
    mapnik::feature_type_style dfplsty;

    MapAction* mAction;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* evt);
    void resizeEvent(QResizeEvent* evt);
    void mousePressEvent(QMouseEvent* evt);
    void mouseMoveEvent(QMouseEvent* evt);
    void mouseReleaseEvent(QMouseEvent* evt);
    void wheelEvent(QWheelEvent* evt);
};

#endif // MAPVIEW_H
