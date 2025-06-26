#include "GanttBarItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>

GanttBarItem::GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
                           int timeUnit, int offsetX, int offsetY, int passedBarHeight, QColor color,  bool isHighlighted):
//    m_defaultColor(Qt::blue),
    m_defaultColor(color),
    m_assignedColor(color),
    m_bisHighlighted(isHighlighted)
{
    int x = offsetX + startTime * timeUnit;
    int width = duration * timeUnit;

    setRect(x, offsetY, width, passedBarHeight);
    setBrush(color);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setHighlighted(isHighlighted);
}



//void GanttBarItem::setHighlighted(bool on) {
//    setBrush(on ? Qt::red : m_defaultColor);
//}

void GanttBarItem::setHighlighted(bool on) {
    m_isManuallyHighlighted = on;
    setBrush(on ? Qt::yellow : m_assignedColor);
}


//void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
//    m_dragStart = event->pos();
//    setHighlighted(true);
//    QGraphicsRectItem::mousePressEvent(event);
//}


void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_dragStart = event->pos();
    m_isManuallyHighlighted = !m_isManuallyHighlighted;
    setBrush(m_isManuallyHighlighted ? Qt::yellow : m_assignedColor);
    QGraphicsRectItem::mousePressEvent(event);
}




//void GanttBarItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
//    setHighlighted(false);
//    bool collided = false;

//    for (auto *item : collidingItems()) {
//        if (dynamic_cast<GanttBarItem*>(item)) {
//            collided = true;
//            break;
//        }
//    }

//    // Если была коллизия — временно цвет → чёрный
//    if (collided) {
//        setBrush(Qt::black);
//    } else {
//        setBrush(m_assignedColor);  // вернуть оригинальный цвет
//    }

//    QGraphicsRectItem::mouseReleaseEvent(event);
//}


void GanttBarItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    bool collided = false;
    for (auto *item : collidingItems()) {
        if (dynamic_cast<GanttBarItem*>(item)) {
            collided = true;
            break;
        }
    }

    if (collided) {
        if (!m_isManuallyHighlighted)
            setBrush(Qt::black);  // только если не вручную выделен
    } else {
        // возвращаем нужный цвет
        setBrush(m_isManuallyHighlighted ? Qt::yellow : m_assignedColor);
    }

    QGraphicsRectItem::mouseReleaseEvent(event);
}



