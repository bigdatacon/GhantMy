#include "GanttBarItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPainter>
#include "GanttView.h"


GanttBarItem::GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
                           int timeUnit, int offsetX, int offsetY, int passedBarHeight, QColor color):
    m_defaultColor(Qt::blue),
    m_assignedColor(color),
    m_opId(id)
{
    int x = offsetX + startTime * timeUnit;
    int width = duration * timeUnit;

    setRect(x, offsetY, width, passedBarHeight);
    setBrush(color);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}



//void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
//    m_dragStart = event->pos();
//    setHighlighted(true);
//    QGraphicsRectItem::mousePressEvent(event);
//}

//void GanttBarItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
//    setHighlighted(false);
//    QList<QGraphicsItem*> others = collidingItems();
//    for (auto *item : others) {
//        if (dynamic_cast<GanttBarItem*>(item)) {
//            setBrush(Qt::darkRed);
//            break;
//        }
//    }
//    QGraphicsRectItem::mouseReleaseEvent(event);
//}

void GanttBarItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    setHighlighted(false);
    bool collided = false;

    for (auto *item : collidingItems()) {
        if (dynamic_cast<GanttBarItem*>(item)) {
            collided = true;
            break;
        }
    }

    // Если была коллизия — временно цвет → чёрный
    if (collided) {
        setBrush(Qt::black);
    } else {
        setBrush(m_assignedColor);  // вернуть оригинальный цвет
    }

    QGraphicsRectItem::mouseReleaseEvent(event);
}

//void GanttBarItem::setHighlighted(bool highlight) {
//    m_highlighted = highlight;
//    update();
//}

QString GanttBarItem::getOpId() const {
    return m_opId;
}


void GanttBarItem::setHighlighted(bool on) { m_highlighted = on; update(); }
bool GanttBarItem::isHighlighted() const { return m_highlighted; }


void GanttBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QColor fillColor = m_highlighted ? QColor(Qt::yellow).lighter(150) : m_assignedColor;
    painter->setBrush(fillColor);
    painter->drawRect(rect());
}




void GanttBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
    if (auto *view = qobject_cast<GanttView*>(scene()->views().first())) {
        bool isTop = view->getTitle().contains("Top");
        view->highlightRelatedGroup(m_opId, isTop);
    }
}


