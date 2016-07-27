#include "mapviz.h"
#include <QMenu>
#include <QMenuBar>

#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>

#include "viewaction.h"
MapViz::MapViz(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolBars();
    mapview = new MapView();
    setCentralWidget(mapview);
    resize(800,600);
    viewAction.setActionType(ViewAction::pan);
    mapview->setMapAction(&viewAction);
}

MapViz::~MapViz()
{

}

void MapViz::createActions()
{
    addData = new QAction(QIcon(":/images/icons/mActionAddVector.png"),tr("Add Data"),this);
    connect(addData, SIGNAL(triggered()),this,SLOT(add_data()));

    exportXML = new QAction(tr("Export XML"),this);
    connect(exportXML, SIGNAL(triggered()),this,SLOT(export_xml()));

    about = new QAction(tr("About"),this);
    connect(about, SIGNAL(triggered()),this,SLOT(aboutF()));

    panMap = new QAction(QIcon(":/images/icons/mActionPan.png"),tr("Pan"),this);
    panMap->setCheckable(true);
    connect(panMap, SIGNAL(triggered()),this,SLOT(pan_map()));

    fullExtent = new QAction(QIcon(":/images/icons/mActionZoomFullExtent.png"),tr("Full Extent"),this);
    connect(fullExtent, SIGNAL(triggered()),this,SLOT(full_extent()));

    zoomIn = new QAction(QIcon(":/images/icons/mActionZoomIn.png"),tr("Zoom In"),this);
    zoomIn->setCheckable(true);
    connect(zoomIn, SIGNAL(triggered()),this,SLOT(zoom_in()));

    getinfo = new QAction(QIcon(":/images/icons/mActionInfo.png"),tr("Information"),this);
    getinfo->setCheckable(true);
    connect(getinfo, SIGNAL(triggered()),this,SLOT(get_info()));

    place = new QAction(QIcon(":/images/icons/mActionPlace.png"),tr("Place"),this);
    place->setCheckable(true);
    connect(place, SIGNAL(triggered()),this,SLOT(desc_place()));

    zoomOut = new QAction(QIcon(":/images/icons/mActionZoomOut.png"),tr("Zoom Out"),this);
    connect(zoomOut, SIGNAL(triggered()),this,SLOT(zoom_out()));

    mapActionGrp = new QActionGroup(this);
    mapActionGrp->addAction(panMap);
    mapActionGrp->addAction(zoomIn);
    mapActionGrp->addAction(getinfo);
    mapActionGrp->addAction(place);
    panMap->setChecked(true);

}

void MapViz::createMenus()
{
    fileMenu = new QMenu(tr("&File"),this);
    fileMenu->addAction(addData);
    fileMenu->addAction(exportXML);
    menuBar()->addMenu(fileMenu);

    helpMenu = new QMenu(tr("&Help"),this);
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(about);

}

void MapViz::createToolBars()
{
    dataToolBar = addToolBar(tr("Data Tools"));
    dataToolBar->addAction(addData);
    mapToolBar = addToolBar(tr("Map Tools"));

    mapToolBar->addAction(fullExtent);
    mapToolBar->addAction(panMap);
    mapToolBar->addAction(zoomIn);
    mapToolBar->addAction(getinfo);
    mapToolBar->addAction(place);

    //mapToolBar->addAction(zoomOut);

}

void MapViz::add_data(const QString &path)
{
    //QMessageBox::information(this,"Info","Add Data");
    QString filename = QFileDialog::getOpenFileName(this,"Add Data","","Shapefile (*.shp)");
    if(!filename.isEmpty())
    {
        std::cout << "Add Data:" << filename.toStdString() << std::endl;
        mapview->adddatatomap(filename);
    }

}

void MapViz::pan_map()
{
    //mapview->setCurrentTool(MapView::Pan);
    viewAction.setActionType(ViewAction::pan);

}

void MapViz::full_extent()
{
    std::cout << "Zoom Full" << std::endl;
    mapview->zoomFullExtent();
}

void MapViz::zoom_in()
{

    viewAction.setActionType(ViewAction::zoomin);
    //mapview->setMapAction(&viewAction);
    /*mapview->setCurrentTool(MapView::ZoomIn);
    std::cout << "Zoom In" << std::endl;*/
}

void MapViz::zoom_out()
{

}

void MapViz::get_info()
{
    mapview->setCurrentTool(MapView::Info);
}

void MapViz::desc_place()
{
    mapview->setCurrentTool(MapView::Place);
}

void MapViz::export_xml()
{
    std::cout << mapview->export2xml() << std::endl;

    QMessageBox::information(this,"Map Style",QString::fromStdString(mapview->export2xml()));
}

void MapViz::aboutF()
{
    QMessageBox::about(this,"About","MapViz-A Experimental Map Analysis and Visualization System.\n by Lu Wei.");
}
