#include "GanttBarItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>

GanttBarItem::GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
                           int timeUnit, int offsetX, int offsetY, int passedBarHeight, QColor color)
{
    int x = offsetX + startTime * timeUnit;
    int width = duration * timeUnit;

    setRect(x, offsetY, width, passedBarHeight);
    setBrush(color);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}



void GanttBarItem::setHighlighted(bool on) {
    setBrush(on ? Qt::red : m_defaultColor);
}

void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_dragStart = event->pos();
    setHighlighted(true);
    QGraphicsRectItem::mousePressEvent(event);
}

void GanttBarItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    setHighlighted(false);
    QList<QGraphicsItem*> others = collidingItems();
    for (auto *item : others) {
        if (dynamic_cast<GanttBarItem*>(item)) {
            setBrush(Qt::darkRed);
            break;
        }
    }
    QGraphicsRectItem::mouseReleaseEvent(event);
}


