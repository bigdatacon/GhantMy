
#include "GanttView.h"
#include "GanttBarItem.h"
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QDebug>
#include <QSet>



GanttView::GanttView(const QString &title, const QVector<OperationData> &operations, GanttDB* db, int maxFinishTime, int uniqueJobCount)
    : QGraphicsView(), m_title(title), m_operations(operations), m_pDB(db), m_maxFinishTime(maxFinishTime), m_uniqueJobCount(uniqueJobCount) {
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    generateJobColorMap();
}


void GanttView::generateJobColorMap() {
    QSet<int> uniqueJobIds;
    for (const auto& op : m_pDB->topOperations)
        uniqueJobIds.insert(op.jobId);
    for (const auto& op : m_pDB->bottomOperations)
        uniqueJobIds.insert(op.jobId);

    QList<int> sortedIds = uniqueJobIds.values();
    std::sort(sortedIds.begin(), sortedIds.end());

    for (int i = 0; i < sortedIds.size(); ++i) {
        int hue = (i * 360 / sortedIds.size()) % 360;
        QColor color = QColor::fromHsv(hue, 200, 255);
        m_jobColorMap[sortedIds[i]] = color;
    }
}

void GanttView::populateScene() {
    m_scene->clear();

    QMultiMap<int, int> jobToMachines;
    for (const auto& op : m_pDB->topOperations + m_pDB->bottomOperations) {
        jobToMachines.insert(op.jobId, op.machineId);
    }



    int viewWidth = viewport()->width();
    int viewHeight = viewport()->height();

    QSet<int> axisIds;
    for (const auto& op : m_operations) {
        axisIds.insert(m_title.contains("Top") ? op.machineId : op.jobId);
    }
    QList<int> sortedAxisIds = axisIds.values();
    std::sort(sortedAxisIds.begin(), sortedAxisIds.end());
    int axisCount = sortedAxisIds.size();

    int spacingX = viewWidth / 20;
    int titleHeight = viewHeight / 12;
    int topPadding = viewHeight / 10;
    int bottomPadding = viewHeight / 10;

    int availableHeight = viewHeight - titleHeight - topPadding - bottomPadding;
    int spacingY = availableHeight / axisCount;
    int barHeight = spacingY * 0.2;

    int maxFinishTime = 0;
    for (const auto& op : m_pDB->topOperations + m_pDB->bottomOperations)
        maxFinishTime = std::max(maxFinishTime, op.startTime + op.duration);

    int timeUnit = (viewWidth - spacingX * 2) / std::max(1, maxFinishTime);
    int baseFontSize = std::min(viewWidth, viewHeight) / 30;
    int yOffset = titleHeight + topPadding + baseFontSize;

    QGraphicsTextItem *titleItem = new QGraphicsTextItem(m_title);
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(baseFontSize);
    titleItem->setFont(titleFont);

    QFontMetricsF fm(titleFont);
    qreal titleWidth = fm.boundingRect(m_title).width();
    titleItem->setPos(viewWidth / 2 - titleWidth / 2, 0);
    m_scene->addItem(titleItem);

    for (int t = 0; t <= maxFinishTime; ++t) {
        int x = spacingX + t * timeUnit;
        m_scene->addLine(x, yOffset, x, yOffset + spacingY * axisCount, QPen(Qt::lightGray));

        auto *label = new QGraphicsTextItem(QString::number(t));
        QFont labelFont;
        labelFont.setPointSize(baseFontSize * 0.8);
        label->setFont(labelFont);
        QFontMetricsF labelMetrics(labelFont);
        qreal labelWidth = labelMetrics.boundingRect(QString::number(t)).width();
        int labelY = yOffset - std::max(barHeight, baseFontSize) * 2;
        label->setPos(x - labelWidth / 2, labelY);
        m_scene->addItem(label);
    }

    for (int i = 0; i < sortedAxisIds.size(); ++i) {
        int id = sortedAxisIds[i];
        int y = yOffset + spacingY * i;
        m_scene->addLine(spacingX, y, spacingX + timeUnit * maxFinishTime, y, QPen(Qt::gray));

        auto *axisLabel = new QGraphicsTextItem(QString("%1%2").arg(m_title.contains("Top") ? "М" : "J").arg(id));



        QFont labelFont;
        labelFont.setPointSize(baseFontSize * 0.8);
        axisLabel->setFont(labelFont);
        axisLabel->setPos(5, y - barHeight / 2);
        m_scene->addItem(axisLabel);

        for (const auto& op : m_operations) {
            int groupId = m_title.contains("Top") ? op.machineId : op.jobId;
            if (groupId != id) continue;

            int barY = y - barHeight / 2;
            QColor color = m_jobColorMap.value(op.jobId, Qt::blue);

            auto *bar = new GanttBarItem(
                op.id,
                op.machineId,
                op.jobId,
                op.startTime,
                op.duration,
                timeUnit,
                spacingX,
                barY,
                barHeight,
                color,
                op.isHighlighted

            );
            bar->setToolTip(QString("%1\nSetup: %2\nCost: %3").arg(op.name).arg(op.setupTime).arg(op.cost));
            m_scene->addItem(bar);

            if (m_title.contains("Top")) {
                // На верхнем графике показываем jobId (как было)
                QGraphicsTextItem* jobLabel = new QGraphicsTextItem(QString("J%1").arg(op.jobId));
                QFont labelFont;
                labelFont.setPointSize(baseFontSize * 0.6);
                jobLabel->setFont(labelFont);
                int barX = spacingX + op.startTime * timeUnit;
                jobLabel->setPos(barX + 2, barY);  // немного вправо от начала бара
                m_scene->addItem(jobLabel);
            }
        else {
            QVector<OperationData> relatedTopOps = m_pDB->bottomOpIdToGroup.value(op.id);

            QSet<int> machineSet;
            for (const auto& relatedOp : relatedTopOps)
                machineSet.insert(relatedOp.machineId);

            // Преобразуем в список и сортируем
            QList<int> sortedMachines = QList<int>::fromSet(machineSet);
            std::sort(sortedMachines.begin(), sortedMachines.end());

            QStringList machineLabels;
            for (int mid : sortedMachines)
                machineLabels << QString("M%1").arg(mid);

            QGraphicsTextItem* machineLabel = new QGraphicsTextItem(machineLabels.join(","));
            QFont labelFont;
            labelFont.setPointSize(baseFontSize * 0.6);
            machineLabel->setFont(labelFont);
            int barX = spacingX + op.startTime * timeUnit;
            machineLabel->setPos(barX + 2, barY);
            m_scene->addItem(machineLabel);
        }


        }
    }

    int sceneWidth = spacingX + timeUnit * (maxFinishTime + 1);
    int sceneHeight = yOffset + spacingY * axisCount;
    int paddingRight = viewWidth / 8;
    m_scene->setSceneRect(0, 0, sceneWidth + paddingRight, sceneHeight + bottomPadding);
//    m_scene->setProperty("view", QVariant::fromValue(static_cast<void*>(this)));

    m_scene->setProperty("db", QVariant::fromValue(static_cast<void*>(m_pDB)));
    m_scene->setProperty("view", QVariant::fromValue(static_cast<void*>(this)));


}


void GanttView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        qreal currentScale = transform().m11();  // текущий масштаб

        double scaleFactor = (event->angleDelta().y() > 0) ? 1.1 : 0.9;
        double newScale = currentScale * scaleFactor;

        if (newScale < 0.5 || newScale > 4.0)
            return;  // ограничение масштаба

        scale(scaleFactor, scaleFactor);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}



void GanttView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    if (!m_bInitialized && viewport()->width() > 0) {
        m_bInitialized = true;
        populateScene();  // запускается только при первом реальном размере
    }
}


void GanttView::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem* clickedItem = m_scene->itemAt(scenePos, QTransform());

    if (!clickedItem || !dynamic_cast<GanttBarItem*>(clickedItem)) {
        clearHighlights();  // если клик вне бара
    }

    QGraphicsView::mousePressEvent(event);
}



void GanttView::printLinkedOperations(const QString &opId, int jobId, int m_startTime, int m_duration) {
    if (!m_pDB) return;

    clearHighlights();  // Снять все подсветки

    QSet<QString> printedIds;

    qDebug() << "=== Связанные операции для jobId =" << jobId << ", opId =" << opId << "==="
             << " start_time : " << m_startTime << " duration : " <<  m_duration;

    for (const auto &pair : m_pDB->topOpIdToGroup) {
        for (const auto &op : pair.first + pair.second) {
            if (printedIds.contains(op.id) || op.id == opId) continue;

            if (op.jobId == jobId && (op.startTime + op.duration == m_startTime + m_duration)) {
                printedIds.insert(op.id);
                qDebug() << "Top: id=" << op.id;
                // Найти и подсветить этот бар на сцене
                for (QGraphicsItem *item : m_scene->items()) {
                    if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
                        if (bar->getOpId() == op.id)
                            bar->setHighlighted(true);
                    }
                }
            }
        }
    }

    for (const auto &list : m_pDB->bottomOpIdToGroup) {
        for (const auto &op : list) {
            if (printedIds.contains(op.id) || op.id == opId) continue;

            if (op.jobId == jobId && (op.startTime + op.duration == m_startTime + m_duration)) {
                printedIds.insert(op.id);
                qDebug() << "Bottom: id=" << op.id;
                for (QGraphicsItem *item : m_scene->items()) {
                    if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
                        if (bar->getOpId() == op.id)
                            bar->setHighlighted(true);
                    }
                }
            }
        }
    }

    // Подсветить и сам текущий бар
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
            if (bar->getOpId() == opId)
                bar->setHighlighted(true);
        }
    }
}



void GanttView::clearHighlights() {
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
            bar->setHighlighted(false);
        }
    }
}
