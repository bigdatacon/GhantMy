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




//void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
//    m_dragStart = event->pos();

//    clearAllHighlightsExceptThis();  // Сбросить всё кроме текущего

//    m_isManuallyHighlighted = !m_isManuallyHighlighted;
//    setBrush(m_isManuallyHighlighted ? Qt::yellow : m_assignedColor);

//    QGraphicsRectItem::mousePressEvent(event);


//    auto *scenePtr = scene();
//    if (!scenePtr) return;

//    QObject *view = scenePtr->parent();
//    auto *ganttView = qobject_cast<GanttView*>(view);

//    if (ganttView) {
//        ganttView->printLinkedOperations(m_opId, m_jobId, m_startTime, m_duration);
//    }

//}

//void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
//    m_dragStart = event->pos();

//    auto *scenePtr = scene();
//    if (!scenePtr) return;

//    QVariant viewVar = scenePtr->property("view");
//    if (viewVar.isValid()) {
//        auto *view = static_cast<GanttView*>(viewVar.value<void*>());
//        if (view) {
//            view->printLinkedOperations(m_opId, m_jobId, m_startTime, m_duration);
//        }
//    }

//    QGraphicsRectItem::mousePressEvent(event);
//}

void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_dragStart = event->pos();

    auto *scenePtr = scene();
    if (!scenePtr) return;

    QVariant dbVar = scenePtr->property("db");
    QVariant viewVar = scenePtr->property("view");
    if (!dbVar.isValid() || !viewVar.isValid()) return;

    GanttDB* db = static_cast<GanttDB*>(dbVar.value<void*>());
    GanttView* currentView = static_cast<GanttView*>(viewVar.value<void*>());

    if (!db || !currentView) return;

    // Очистить оба графика
    if (db->topView) db->topView->clearHighlights();
    if (db->bottomView) db->bottomView->clearHighlights();

    // Подсветить в обоих графиках
    if (db->topView) db->topView->printLinkedOperations(m_opId, m_jobId, m_startTime, m_duration);
    if (db->bottomView) db->bottomView->printLinkedOperations(m_opId, m_jobId, m_startTime, m_duration);
    QGraphicsRectItem::mousePressEvent(event);

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


QString GanttBarItem::getOpId() const {
    return m_opId;
}


