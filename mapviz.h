#ifndef MAPVIZ_H
#define MAPVIZ_H

#include <QMainWindow>
#include "mapview.h"
#include <QToolBar>
#include <QActionGroup>
#include "viewaction.h"

class MapViz : public QMainWindow
{
    Q_OBJECT

public:
    MapViz(QWidget *parent = 0);
    ~MapViz();
public slots:
    void add_data(QString const&  path = QString());
    void pan_map();
    void full_extent();
    void zoom_in();
    void zoom_out();
    void get_info();
    void desc_place();
    void export_xml();
    void aboutF();
private:
    // methods
    void createActions();
    void createMenus();
    void createToolBars();

    MapView *mapview;
    // actions
    QAction *addData;
    QAction *panMap;
    QAction *fullExtent;
    QAction *zoomIn;
    QAction *zoomOut;
    QAction *getinfo;
    QAction *place;
    QAction *about;

    QAction *exportXML;

    // action group
    QActionGroup *mapActionGrp;

    // menus
    QMenu *fileMenu;
    QMenu *helpMenu;
    // toolbars
    QToolBar *dataToolBar;
    QToolBar *mapToolBar;

    ViewAction viewAction;
};

#endif // MAPVIZ_H
