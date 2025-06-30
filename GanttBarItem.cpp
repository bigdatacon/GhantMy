#include "GanttBarItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QGraphicsScene>
#include "GanttDB.h"
#include "GanttView.h"
#include <cmath> // убедитесь, что подключили этот заголовок
#include <QMessageBox>


//GanttBarItem::GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
//                           int timeUnit, int offsetX, int offsetY, int passedBarHeight,
//                           QColor color, bool isHighlighted, int setupTime, int cost, const QString &innerLabelTex )
//    : m_opId(id),
//      m_jobId(jobId),
//      m_startTime(startTime),
//      m_duration(duration),
//      m_itimeUnit(timeUnit),  // сохраняем
//      m_isetupTime(setupTime),  // сохраняем
//      m_defaultColor(Qt::blue),
//      m_assignedColor(color),
//      m_bisHighlighted(isHighlighted),
//      m_isManuallyHighlighted(isHighlighted),
//      m_icost(cost)
//{
//    int x = offsetX + startTime * timeUnit;
//    int width = duration * timeUnit;

//    setRect(x, offsetY, width, passedBarHeight);
//    setBrush(isHighlighted ? Qt::yellow : m_assignedColor);
//    setFlag(ItemIsMovable);
//    setFlag(ItemSendsGeometryChanges);
//    setAcceptHoverEvents(true);

//    // Добавляем текст внутрь бара
//    m_innerLabel = new QGraphicsTextItem(innerLabelTex, this);
//    QFont textFont;
//    textFont.setPointSizeF(std::max(passedBarHeight * 0.4, 8.0)); // минимум 8
//    m_innerLabel->setFont(textFont);

//    m_innerLabel->adjustSize();
//    qreal textX = 2;  // небольшой левый отступ
//    qreal textY = (passedBarHeight - m_innerLabel->boundingRect().height()) / 2;
//    m_innerLabel->setPos(textX, textY);

//}


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
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);

//    // Добавляем текст внутрь бара
//    m_innerLabel = new QGraphicsTextItem(innerLabelTex, this);
//    QFont textFont;
//    textFont.setPointSizeF(std::max(passedBarHeight * 0.4, 8.0)); // минимум 8
//    m_innerLabel->setFont(textFont);

//    m_innerLabel->setDefaultTextColor(Qt::white);


//    m_innerLabel->adjustSize();
//    qreal textX = 2;  // небольшой левый отступ
//    qreal textY = (passedBarHeight - m_innerLabel->boundingRect().height()) / 2;
//    m_innerLabel->setPos(textX, textY);

    // 💬 Вывод всех данных на печать
    qDebug() << "Создан GanttBarItem:";
    qDebug() << "  ID:" << id;
    qDebug() << "  Machine ID:" << machineId;
    qDebug() << "  Job ID:" << jobId;
    qDebug() << "  Start Time:" << startTime;
    qDebug() << "  Duration:" << duration;
    qDebug() << "  Time Unit:" << timeUnit;
    qDebug() << "  Setup Time:" << setupTime;
    qDebug() << "  Cost:" << cost;
    qDebug() << "  Inner Label Text:" << innerLabelTex;
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


//void GanttBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    Q_UNUSED(option);
//    Q_UNUSED(widget);

//    QRectF r = rect();

//    int setupWidth = m_isetupTime * m_itimeUnit;

//    if (setupWidth > 0 && setupWidth < r.width()) {
//        // Наладка (штриховка)
//        QRectF setupRect(r.left(), r.top(), setupWidth, r.height());

//        QBrush hatchBrush(m_assignedColor, Qt::DiagCrossPattern);
//        painter->setBrush(hatchBrush);
//        painter->drawRect(setupRect);

//        // Основная работа
//        QRectF workRect(r.left() + setupWidth, r.top(), r.width() - setupWidth, r.height());
//        painter->setBrush(m_isManuallyHighlighted ? Qt::yellow : m_assignedColor);
//        painter->drawRect(workRect);
//    } else {
//        // Без наладки
//        painter->setBrush(m_isManuallyHighlighted ? Qt::yellow : m_assignedColor);
//        painter->drawRect(r);
//    }
//}


void GanttBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF r = rect();

    int setupWidth = m_isetupTime * m_itimeUnit;

    if (setupWidth > 0 && setupWidth < r.width()) {
        // Наладка (штриховка)
        QRectF setupRect(r.left(), r.top(), setupWidth, r.height());

        QBrush hatchBrush(m_assignedColor, Qt::DiagCrossPattern);
        painter->setBrush(hatchBrush);
        painter->drawRect(setupRect);

        // Основная работа
        QRectF workRect(r.left() + setupWidth, r.top(), r.width() - setupWidth, r.height());
        painter->setBrush(m_isManuallyHighlighted ? Qt::yellow : m_assignedColor);
        painter->drawRect(workRect);
    } else {
        // Без наладки
        painter->setBrush(m_isManuallyHighlighted ? Qt::yellow : m_assignedColor);
        painter->drawRect(r);
    }

    // 💬 Рисуем текст поверх бара
//    painter->setPen(Qt::white); // цвет текста
    QFont font = painter->font();
    font.setPointSizeF(std::max(r.height() * 0.4, 8.0));
    painter->setFont(font);

    // Центрируем текст
    QRectF textRect = r;
    painter->drawText(textRect, Qt::AlignCenter, m_innerLabel);
}

