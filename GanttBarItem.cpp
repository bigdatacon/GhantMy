#include "GanttBarItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsScene>
#include "GanttDB.h"
#include "GanttView.h"
#include <cmath> // убедитесь, что подключили этот заголовок
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>  // также если используется QDateTime для синуса
#include <QObject>


GanttBarItem::GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
                           int timeUnit, int offsetX, int offsetY, int passedBarHeight,
                           QColor color, bool isHighlighted, int setupTime, int cost, const QString &innerLabelTex)
    : m_opId(id),
      m_jobId(jobId),
      m_startTime(startTime),
      m_duration(duration),
      m_itimeUnit(timeUnit),
      m_isetupTime(setupTime),
      m_defaultColor(Qt::blue),
      m_assignedColor(color),
      m_bisHighlighted(isHighlighted),
      m_isManuallyHighlighted(isHighlighted),
      m_icost(cost),
      m_innerLabel(innerLabelTex)
{
    int x = offsetX + startTime * timeUnit;
    int width = duration * timeUnit;

    setRect(x, offsetY, width, passedBarHeight);
    setBrush(isHighlighted ? Qt::yellow : m_assignedColor);
//    setFlag(ItemIsMovable);

    // Было
    setFlag(ItemIsMovable);

    // Стало
    setFlag(ItemIsMovable, false); // По умолчанию не двигается


    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);

    m_pulseTimer = new QTimer();
    QObject::connect(m_pulseTimer, &QTimer::timeout, this, [this]() {
//        m_currentAlpha = 0.5 + 0.5 * std::sin(QDateTime::currentMSecsSinceEpoch() / (200.0 - m_icost));
        double amplitude = std::min(0.8, 0.3 + m_icost / 500.0);
        m_currentAlpha = 0.2 + amplitude * (0.5 + 0.5 * std::sin(QDateTime::currentMSecsSinceEpoch() / (200.0 - m_icost)));

        update();
    });


}

void GanttBarItem::setHighlighted(bool on) {
    m_isManuallyHighlighted = on;
    setBrush(on ? Qt::yellow : m_assignedColor);
}

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

    if (event->button() == Qt::RightButton) {
        // Формируем текст с инфо
        QString info = QString("ID: %1\nMachine ID: %2\nJob ID: %3\nStart: %4\nDuration: %5\nSetup: %6\nName: %7\nCost: %8")
                .arg(m_opId)
//                .arg(m_imachineId)
                .arg(m_jobId)
                .arg(m_startTime)
                .arg(m_duration)
                .arg(m_setupTime)
//                .arg(m_name)
                .arg(m_icost);

        QMessageBox::information(nullptr, "Информация о баре", info);
        return; // Чтобы не обрабатывать дальше
    }

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

    // 💡 Здесь добавляем вызов обновления стрелок
    auto *scenePtr = scene();
    if (scenePtr) {
        QVariant viewVar = scenePtr->property("view");
        if (viewVar.isValid()) {
            GanttView* view = static_cast<GanttView*>(viewVar.value<void*>());
            if (view) {
                view->printLinkedOperations(m_opId, m_jobId, m_startTime, m_duration);
            }
        }
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


void GanttBarItem::drawArrow(QGraphicsScene *scene, QPointF from, QPointF to) {
    QPen pen(Qt::red, 2);
    scene->addLine(QLineF(from, to), pen)->setData(0, "arrow");

    double angle = std::atan2(to.y() - from.y(), to.x() - from.x());
    double arrowSize = 8;

    QPointF arrowP1 = to - QPointF(arrowSize * std::cos(angle - M_PI / 6), arrowSize * std::sin(angle - M_PI / 6));
    QPointF arrowP2 = to - QPointF(arrowSize * std::cos(angle + M_PI / 6), arrowSize * std::sin(angle + M_PI / 6));

    QPolygonF arrowHead;
    arrowHead << to << arrowP1 << arrowP2;
    auto *arrow = scene->addPolygon(arrowHead, pen, QBrush(Qt::red));
    arrow->setData(0, "arrow");
}


void GanttBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF r = rect();
    int setupWidth = m_isetupTime * m_itimeUnit;

    // Финальный цвет
    QColor finalColor = m_isManuallyHighlighted ? Qt::yellow : m_assignedColor;

    if (m_pulsing ) {
        finalColor.setAlphaF(m_currentAlpha);

        double penWidth = std::min(10.0, 1.0 + m_icost / 50.0);
        QPen pen(Qt::black, penWidth);
        painter->setPen(pen);
    }


    if (m_isManuallyHighlighted) {
        painter->setBrush(Qt::yellow);
        painter->drawRect(r);
    } else if (setupWidth > 0 && setupWidth < r.width()) {
        // Наладка
        QRectF setupRect(r.left(), r.top(), setupWidth, r.height());
        QBrush hatchBrush(finalColor, Qt::DiagCrossPattern);
        painter->setBrush(hatchBrush);
        painter->drawRect(setupRect);

        // Основная часть
        QRectF workRect(r.left() + setupWidth, r.top(), r.width() - setupWidth, r.height());
        painter->setBrush(finalColor);
        painter->drawRect(workRect);
    } else {
        // Без наладки
        painter->setBrush(finalColor);
        painter->drawRect(r);
    }

    // Рисуем текст
    QFont font = painter->font();
    font.setPointSizeF(std::max(r.height() * 0.4, 8.0));
    painter->setFont(font);

    painter->drawText(r, Qt::AlignCenter, m_innerLabel);
}


void GanttBarItem::startPulse() {
    if (!m_pulsing) {
        m_pulsing = true;
        m_pulseTimer->start(50);
    }
}

void GanttBarItem::stopPulse() {
    if (m_pulsing) {
        m_pulsing = false;
        m_pulseTimer->stop();
        m_currentAlpha = 1.0;
        update();
    }
}
