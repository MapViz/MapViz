#include "mapview.h"
#include <mapnik/datasource_cache.hpp>
#include <mapnik/datasource.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/params.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/image.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/box2d.hpp>
#include <mapnik/save_map.hpp>
#include <mapnik/proj_transform.hpp>
#include <mapnik/projection.hpp>
#include <mapnik/geometry.hpp>
#include <mapnik/geometry_type.hpp>
#include <mapnik/view_transform.hpp>
#include <mapnik/transform_path_adapter.hpp>
#include <mapnik/vertex_adapters.hpp>
#include <mapnik/geometry_adapters.hpp>

#include <mapnik/util/container_adapter.hpp>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QFileInfo>
#include <QMessageBox>

#include "mapaction.h"

MapView::MapView(QWidget *parent) : QWidget(parent),
    currentTool(Pan),
    drag(false),
    mAction(NULL)
{
    map = new mapnik::Map();
    mapnik::datasource_cache::instance().register_datasources("/usr/local/lib/mapnik/input/");
    mapnik::rule rp;
    mapnik::markers_symbolizer ptsym;
    //mapnik::point_symbolizer ptsym;
    mapnik::put(ptsym,mapnik::keys::allow_overlap,true);
    //mapnik::put(ptsym,mapnik::keys::fill,mapnik::color(255,0,0));
    mapnik::put(ptsym,mapnik::keys::stroke_opacity,0);
//    mapnik::put(ptsym,mapnik::keys::width,10);
//    mapnik::put(ptsym,mapnik::keys::height,10);

    rp.append(std::move(ptsym));
    dfptsty.add_rule(std::move(rp));
    map->insert_style("Default Point Style", dfptsty);

    mapnik::rule rl;
    mapnik::line_symbolizer lisym;
    rl.append(std::move(lisym));
    dflisty.add_rule(std::move(rl));
    map->insert_style("Default Line Style", dflisty);

    mapnik::rule rpl;
    mapnik::polygon_symbolizer plsym;
    rpl.append(std::move(plsym));
    dfplsty.add_rule(std::move(rpl));
    map->insert_style("Default Poly Style", dfplsty);

}
void MapView::paintEvent(QPaintEvent *evt)
{
    if(mAction)
        mAction->paintEvent(evt);
    else
    {
        QPainter painter(this);
        painter.drawPixmap(QPoint(0,0),mappix);
        painter.end();
    }
    /*
    QPainter painter(this);
    if(drag)
    {
        if(currentTool==ZoomIn)
        {
            int dx = mEndX - mStartX;
            int dy = mEndY - mStartY;
            QPen pen;
            pen.setWidth(1);
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            painter.setPen(pen);
            painter.setBrush(QColor(200,200,255,128));
            painter.drawPixmap(QPoint(0,0),mappix);
            painter.drawRect(mStartX,mStartY,dx,dy);
        }
        else if (currentTool==Pan)
        {
            int dx = mEndX - mStartX;
            int dy = mEndY - mStartY;
            //painter.setBrush(QColor(200,200,200,128));
            //painter.drawRect(0,0,width(),height());
            painter.drawPixmap(QPoint(dx,dy),mappix);
        }
    }
    else
    {
        painter.drawPixmap(QPoint(0,0),mappix);
    }

    painter.end();*/
}

void MapView::resizeEvent(QResizeEvent *evt)
{
   if(map)
   {
       map->resize(this->width(),this->height());
       refreshMap();
   }
}

void MapView::mousePressEvent(QMouseEvent *evt)
{
    if(mAction)
        mAction->mousePressEvent(evt);
#if 0
    if(evt->button()==Qt::LeftButton)
    {
        //std::cout << currentTool;
        if (currentTool == Pan || currentTool == ZoomIn)
        {
            mStartX = evt->x();
            mStartY = evt->y();
            drag = true;
        }
        else if(currentTool == Info)
        {
            QVector<QPair<QString,QString> > featinfo;
            QString infostr;
            double x = evt->x();
            double y = evt->y();
            std::cout << "Map:" << x << "," << y << std::endl;
            mapnik::featureset_ptr fs = map->query_map_point(0,x,y);
            if (fs)
            {
                mapnik::feature_ptr feat = fs->next();
                if(feat)
                {
                    mapnik::feature_kv_iterator itr(*feat,true);
                    mapnik::feature_kv_iterator end(*feat);

                    for ( ;itr!=end; ++itr)
                    {
                        featinfo.push_back(QPair<QString,QString>(QString(std::get<0>(*itr).c_str()),
                                                              std::get<1>(*itr).to_string().c_str()));
                        infostr += QString(std::get<0>(*itr).c_str());
                        infostr += ":";
                        infostr += std::get<1>(*itr).to_string().c_str();
                        infostr += ",";
                    }

                    drawFeature(feat);
                    QMessageBox::information(this,"Feature Information",infostr);
                }
            }
            //std::cout << infostr.toStdString()<< std::endl;
        }
        else if(currentTool == Place)
        {
            double x = evt->x();
            double y = evt->y();
            mapnik::view_transform t(map->width(),map->height(),map->get_current_extent());
            t.backward(&x,&y);
            std::cout << x << "," << y << std::endl;
            /*mapnik::layer & layer = map->layers()[0];
            mapnik::projection mprj(map->srs());
            mapnik::projection lprj(layer.srs());
            mapnik::proj_transform prj_trans(mprj,lprj);
            mapnik::geometry::point<double> pt(x,y);
            std::cout << map->srs()<<std::endl;
            std::cout << layer.srs()<<std::endl;
            prj_trans.forward(pt);*/

            QString ptstr;
            ptstr.sprintf("x:%.2lf\ny:%.2lf",x,y);
            QMessageBox::information(this,"Place",ptstr);
        }
    }
#endif
}

void MapView::mouseMoveEvent(QMouseEvent *evt)
{
    if(mAction)
        mAction->mouseMoveEvent(evt);

    /*  if (currentTool == Pan || currentTool == ZoomIn)
    {
        mEndX = evt->x();
        mEndY = evt->y();
        update();
    }*/
}

void MapView::mouseReleaseEvent(QMouseEvent *evt)
{
    if(mAction)
        mAction->mouseReleaseEvent(evt);
#if 0
    if (evt->button()==Qt::LeftButton)
    {
        mEndX = evt->x();
        mEndY = evt->y();
        if (mEndX==mStartX || mEndY==mStartY)
            return;
        if (currentTool==ZoomIn)
        {
            drag=false;
            if (map)
            {
               mapnik::view_transform t(map->width(),map->height(),map->get_current_extent());
               mapnik::box2d<double> box = t.backward(mapnik::box2d<double>(mStartX,mStartY,mEndX,mEndY));
               map->zoom_to_box(box);
               refreshMap();
            }
        }
        else if (currentTool==Pan)
        {
            drag = false;
            if (map){
                int cx = int(0.5 * map->width());
                int cy = int(0.5 * map->height());
                int dx = mEndX - mStartX;
                int dy = mEndY - mStartY;
                map->pan(cx - dx ,cy - dy);
                refreshMap();
            }
        }

    }
#endif
}

void MapView::wheelEvent(QWheelEvent *evt)
{
    if (!map)
    {
       return;
    }

    QPoint corner(map->width(), map->height());
    QPoint zoomCoords;
    double zoom;
    if (evt->delta() > 0)
    {
       zoom = 0.5;
       QPoint center = corner / 2;
       QPoint delta = evt->pos() - center;
       zoomCoords = zoom * delta + center;
    }
    else
    {
       zoom = 2.0;
       zoomCoords = corner - evt->pos();
    }

    map->pan_and_zoom(zoomCoords.x(), zoomCoords.y(), zoom);
    refreshMap();
}

void MapView::adddatatomap(const QString &datapath)
{
    QFileInfo f(datapath);
    QString fname = f.fileName();
    fname = fname.split(".")[0];
    mapnik::parameters p;
    p["type"] = "shape";
    p["file"] = datapath.toStdString();
    mapnik::layer lyr(fname.toStdString());
    lyr.set_datasource(mapnik::datasource_cache::instance().create(p));
    int geomtype = lyr.datasource()->get_geometry_type().get();
    if(geomtype==1)
        lyr.add_style("Default Point Style");
    if(geomtype==2)
        lyr.add_style("Default Line Style");
    if(geomtype==3)
        lyr.add_style("Default Poly Style");

    map->add_layer(lyr);
    map->zoom_all();
    refreshMap();
}

void MapView::setCurrentTool(mapTool tool)
{
    currentTool = tool;
}

void MapView::refreshMap()
{
    mapnik::image_rgba8 buf(this->width(),this->height());
    mapnik::agg_renderer<mapnik::image_rgba8> ren(*map,buf);
    ren.apply();

    QImage img((uchar*)buf.data(),this->width(),this->height(),QImage::Format_ARGB32);
    mappix = QPixmap::fromImage(img.rgbSwapped());
    update();
}

void MapView::zoomFullExtent()
{
    if(map)
    {
        //std::cout << "Zoom Full" << std::endl;
        map->zoom_all();
        //map->zoom_to_box(map->get_current_extent());
        refreshMap();
    }
}

std::string MapView::export2xml()
{
    return mapnik::save_map_to_string(*map);
}

void MapView::drawFeature(mapnik::feature_ptr feat)
{
    mapnik::geometry::geometry<double> geom = feat->get_geometry();
    auto const& line = mapnik::util::get<mapnik::geometry::line_string<double> >(geom);
    mapnik::geometry::line_string_vertex_adapter<double> va(line);
    mapnik::view_transform t(map->width(),map->height(),map->get_current_extent());
    QPainterPath qpath;
    double sx,sy;
    sx = line[0].x;
    sy = line[0].y;
    t.forward(&sx,&sy);
    qpath.moveTo(sx,sy);
    for(int i=1;i<line.size();++i)
    {
        double x = line[i].x;
        double y = line[i].y;
        std::cout << line[i].x << "," << line[i].y << std::endl;
        t.forward(&x,&y);
        std::cout <<"Forward:" << x << "," << y << std::endl;
        qpath.lineTo(x,y);

    }
    QPainter painter(&mappix);
    QPen pen(QColor(255,0,0,96));
    pen.setWidth(5);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawPath(qpath);
    //refreshMap();
    //mapnik::geometry::line_string<double> line =
    /*using path_type = mapnik::transform_path_adapter<mapnik::view_transform,mapnik::vertex_adapter>;
    mapnik::view_transform t(map->width(),map->height(),map->get_current_extent());
    mapnik::layer & layer = map->layers()[0];
    mapnik::projection mprj(map->srs());
    mapnik::projection lprj(layer.srs());
    mapnik::proj_transform prj_trans(mprj,lprj);


    mapnik::geometry::geometry_types geomt = mapnik::geometry::geometry_type(geom);
    mapnik::vertex_adapter va(geomt);
    path_type path(t,va,prj_trans);
    if (va.size() > 0)
    {
        QPainterPath qpath;
        double x,y;
        va.vertex(&x,&y);
        qpath.moveTo(x,y);
        for (unsigned j = 1; j < geomt.size(); ++j)
        {
            va.vertex(&x,&y);
            qpath.lineTo(x,y);
        }
        QPainter painter(&mappix);
        QPen pen(QColor(255,0,0,96));
        pen.setWidth(3);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawPath(qpath);
        update();
    }*/

}

QPixmap MapView::mapPix()
{
    return mappix;
}

void MapView::setMapAction(MapAction *action)
{
    mAction = action;

    if (action != NULL)
    {
        action->setMapView(this);
    }
}

void MapView::zoom2Box(QPoint sp, QPoint ep)
{
    mapnik::view_transform t(map->width(),map->height(),map->get_current_extent());
    mapnik::box2d<double> box = t.backward(mapnik::box2d<double>(sp.x(),sp.y(),ep.x(),ep.y()));
    map->zoom_to_box(box);
    refreshMap();
}

void MapView::panMap(QPoint sp, QPoint ep)
{
    int cx = int(0.5 * map->width());
    int cy = int(0.5 * map->height());
    int dx = ep.x() - sp.x();
    int dy = ep.y() - sp.y();
    map->pan(cx - dx ,cy - dy);
    refreshMap();
}
