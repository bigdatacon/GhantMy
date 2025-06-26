#include "GanttBarItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsScene>
#include "GanttDB.h"
#include "GanttView.h"

GanttBarItem::GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
                           int timeUnit, int offsetX, int offsetY, int passedBarHeight, QColor color,  bool isHighlighted):
//    m_defaultColor(Qt::blue),
    m_defaultColor(color),
    m_assignedColor(color),
    m_bisHighlighted(isHighlighted),


   m_opId(id),
   m_jobId(jobId),
   m_startTime(startTime),
   m_duration(duration)


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


void GanttBarItem::setHighlighted(bool on) {
    m_isManuallyHighlighted = on;
    setBrush(on ? Qt::yellow : m_assignedColor);
}




void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_dragStart = event->pos();

    clearAllHighlightsExceptThis();  // Сбросить всё кроме текущего

    m_isManuallyHighlighted = !m_isManuallyHighlighted;
    setBrush(m_isManuallyHighlighted ? Qt::yellow : m_assignedColor);

    QGraphicsRectItem::mousePressEvent(event);


    auto *scenePtr = scene();
    if (!scenePtr) return;

    QObject *view = scenePtr->parent();
    auto *ganttView = qobject_cast<GanttView*>(view);

    if (ganttView) {
        ganttView->printLinkedOperations(m_opId, m_jobId);
    }



}






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


void GanttBarItem::clearAllHighlightsExceptThis() {
    if (!scene()) return;

    for (QGraphicsItem *item : scene()->items()) {
        auto *bar = qgraphicsitem_cast<GanttBarItem *>(item);
        if (bar && bar != this) {
            bar->setHighlighted(false);
        }
    }
}


