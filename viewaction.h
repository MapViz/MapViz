#ifndef VIEWACTION_H
#define VIEWACTION_H
#include "mapaction.h"

class ViewAction : public MapAction
{
public:
    typedef enum
    {
        zoomin,
        zoomout,
        pan
    }ActionType;

    ViewAction();
    void paintEvent(QPaintEvent* evt);
    void mouseMoveEvent(QMouseEvent* evt);
    void mousePressEvent(QMouseEvent* evt);
    void mouseReleaseEvent(QMouseEvent* evt);
    void wheelEvent(QWheelEvent* evt);

    ActionType actionType()
    {
        return mActionType;
    }

    void setActionType(ActionType T)
    {
        mActionType = T;
    }

private:
    ActionType mActionType;
    QPoint mBPt;
    QPoint mEPt;
    bool mDrag;
};

#endif // VIEWACTION_H
