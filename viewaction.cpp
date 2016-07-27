#include "viewaction.h"
#include <QPainter>
#include <QPaintEvent>
ViewAction::ViewAction() :
    mDrag(false)
{
    qDebug("ViewAction");
}

void ViewAction::paintEvent(QPaintEvent *evt)
{
    QPainter painter(mMapView);
    if(mDrag)
    {
        if(mActionType==zoomin)
        {
            int dx = mEPt.x() - mBPt.x();
            int dy = mEPt.y() - mBPt.y();
            QPen pen;
            pen.setWidth(1);
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            painter.setPen(pen);
            painter.setBrush(QColor(200,200,255,128));
            painter.drawPixmap(QPoint(0,0),mMapView->mapPix());
            painter.drawRect(mBPt.x(),mBPt.y(),dx,dy);
        }
        else if(mActionType==pan)
        {
            int dx = mEPt.x() - mBPt.x();
            int dy = mEPt.y() - mBPt.y();
            painter.drawPixmap(QPoint(dx,dy),mMapView->mapPix());
        }
    }
    else
    {
        painter.drawPixmap(QPoint(0,0),mMapView->mapPix());
    }

    painter.end();
}

void ViewAction::mousePressEvent(QMouseEvent *evt)
{
    if(evt->button()==Qt::LeftButton)
    {
        if (mActionType==pan || mActionType == zoomin)
        {
            mBPt = evt->pos();
            mDrag = true;
        }
    }
}

void ViewAction::mouseMoveEvent(QMouseEvent *evt)
{
    if (mActionType==pan || mActionType == zoomin)
    {
        mEPt = evt->pos();
        mMapView->update();
        //mDrag = true;
    }
}

void ViewAction::mouseReleaseEvent(QMouseEvent *evt)
{
    if (evt->button()==Qt::LeftButton)
    {
        mEPt = evt->pos();
        mDrag = false;
        if(mEPt==mBPt)
            return;
        if (mActionType == zoomin)
        {
            mMapView->zoom2Box(mBPt,mEPt);

        }
        else if(mActionType==pan)
        {

            mMapView->panMap(mBPt,mEPt);

        }

    }
}

void ViewAction::wheelEvent(QWheelEvent *evt)
{

}
