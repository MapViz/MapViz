#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T17:16:31
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXX = $$system(mapnik-config --cxx)
QMAKE_LINK = $$system(mapnik-config --cxx)
QMAKE_CXXFLAGS += $$system(mapnik-config --cxxflags)
QMAKE_CXXFLAGS += $$system(mapnik-config --includes --dep-includes --defines)
QMAKE_LFLAGS += $$system(mapnik-config --libs)
QMAKE_LFLAGS += $$system(mapnik-config --ldflags --dep-libs)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapViz
TEMPLATE = app


SOURCES += main.cpp\
        mapviz.cpp \
    mapview.cpp \
    mapaction.cpp \
    viewaction.cpp

HEADERS  += mapviz.h \
    mapview.h \
    mapaction.h \
    viewaction.h

unix|win32: LIBS += -lmapnik

unix|win32: LIBS += -lcairo

RESOURCES += \
    mapviz.qrc

