//#include "GanttBarItem.h"
//#include <QGraphicsSceneMouseEvent>
//#include <QBrush>
//#include <QWheelEvent> // Не забудь подключить!

//GanttBarItem::GanttBarItem(int machineId, int jobId, int startTime, int duration, int timeUnit, int offsetX, int offsetY)
//    : m_defaultColor(Qt::blue) {
//    int x = offsetX + startTime * timeUnit;
//    int width = duration * timeUnit;
//    int height = 20;
//    setRect(x, offsetY, width, height);
//    setBrush(m_defaultColor);
//    setFlag(ItemIsMovable);
//    setFlag(ItemSendsGeometryChanges);
//    setAcceptHoverEvents(true);
//}

//void GanttBarItem::setHighlighted(bool on) {
//    setBrush(on ? Qt::red : m_defaultColor);
//}

//void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
//    m_dragStart = event->pos();
//    setHighlighted(true);
//    QGraphicsRectItem::mousePressEvent(event);
//}

//void GanttBarItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
//    QGraphicsRectItem::mouseMoveEvent(event);
//}

//void GanttBarItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
//    setHighlighted(false);

//    // Проверка наложений
//    QList<QGraphicsItem*> others = collidingItems();
//    for (auto *item : others) {
//        if (dynamic_cast<GanttBarItem*>(item)) {
//            setBrush(Qt::darkRed);
//            break;
//        }
//    }

//    QGraphicsRectItem::mouseReleaseEvent(event);
//}

#include "GanttBarItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>

GanttBarItem::GanttBarItem(int machineId, int jobId, int startTime, int duration, int timeUnit, int offsetX, int offsetY, int passedBarHeight)
    : m_defaultColor(Qt::blue)
{
    int x = offsetX + startTime * timeUnit;
    int width = duration * timeUnit;
//    int height = 20;
    int height = passedBarHeight; // Переданный аргумент

    setRect(x, offsetY, width, height);
    setBrush(m_defaultColor);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}


GanttBarItem::GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration, int timeUnit, int offsetX, int offsetY, int passedBarHeight)
    : m_defaultColor(Qt::blue)
{
    int x = offsetX + startTime * timeUnit;
    int width = duration * timeUnit;
//    int height = 20;
    int height = passedBarHeight; // Переданный аргумент

    setRect(x, offsetY, width, height);
    setBrush(m_defaultColor);
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


