
#include "GanttView.h"
#include "GanttBarItem.h"
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QDebug>
#include <QSet>

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>

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
        axisIds.insert(m_title.contains("Машин") ? op.machineId : op.jobId);
    }
    QList<int> sortedAxisIds = axisIds.values();
    std::sort(sortedAxisIds.begin(), sortedAxisIds.end());
    int axisCount = sortedAxisIds.size();

    int spacingX = viewWidth / 18;
    int titleHeight = viewHeight / 10;
    int topPadding = viewHeight / 10;
    int bottomPadding = viewHeight / 10;

    qreal relativeOffsetX = viewWidth * (-0.04);  // 5% от ширины видимой области

    int availableHeight = viewHeight - titleHeight - topPadding - bottomPadding;
    int spacingY = availableHeight / axisCount;
    int barHeight = spacingY * 0.2;

    int maxFinishTime = 0;
    for (const auto& op : m_pDB->topOperations + m_pDB->bottomOperations)
        maxFinishTime = std::max(maxFinishTime, op.startTime + op.duration);

    int timeUnit = (viewWidth - spacingX * 2) / std::max(1, maxFinishTime);
    int baseFontSize = std::min(viewWidth, viewHeight) / 30;
    int yOffset = titleHeight + topPadding + baseFontSize;

    // Заголовок графика
    QGraphicsTextItem *titleItem = new QGraphicsTextItem(m_title);
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(baseFontSize);
    titleItem->setFont(titleFont);

    QFontMetricsF fm(titleFont);
    qreal titleWidth = fm.boundingRect(m_title).width();
    titleItem->setPos(viewWidth / 2 - titleWidth / 2, 0);
    m_scene->addItem(titleItem);

    // Подпись "Время, мин"
    QGraphicsTextItem *timeLabel = new QGraphicsTextItem("Время, мин");
    QFont timeFont;
    timeFont.setPointSize(baseFontSize * 0.7);
    timeFont.setBold(true);
    timeLabel->setFont(timeFont);
    QFontMetricsF fmTime(timeFont);
    qreal timeWidth = fmTime.boundingRect("Время, мин").width();
//    timeLabel->setPos(viewWidth / 2 - timeWidth / 2, yOffset - std::max(barHeight, baseFontSize) * 2.5);
    timeLabel->setPos(viewWidth / 2 - timeWidth / 2, yOffset - std::max(barHeight, baseFontSize) * 3.3);
    m_scene->addItem(timeLabel);

    // Вертикальная подпись
    QString yAxisLabel = m_title.contains("Машин") ? "Машины" : "Работы";
    QGraphicsTextItem *axisLabel = new QGraphicsTextItem(yAxisLabel);
    QFont axisFont;
    axisFont.setPointSize(baseFontSize * 0.7);
    axisFont.setBold(true);
    axisLabel->setFont(axisFont);
    axisLabel->setRotation(-90);
//    axisLabel->setPos(5, viewHeight / 2 + axisLabel->boundingRect().width() / 2);
    axisLabel->setPos(relativeOffsetX, viewHeight / 2 + axisLabel->boundingRect().width() / 2);
    m_scene->addItem(axisLabel);

    for (int t = 0; t <= maxFinishTime; ++t) {
        int x = spacingX + t * timeUnit;
        m_scene->addLine(x, yOffset, x, yOffset + spacingY * axisCount, QPen(Qt::lightGray));

        auto *label = new QGraphicsTextItem(QString::number(t));
        QFont labelFont;
//        labelFont.setPointSize(baseFontSize * 0.8);
        labelFont.setPointSize(baseFontSize * 0.7);
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

        auto *axisText = new QGraphicsTextItem(QString("%1%2").arg(m_title.contains("Машин") ? "М" : "J").arg(id));
        QFont labelFont;
        labelFont.setPointSize(baseFontSize * 0.8);
        axisText->setFont(labelFont);
        axisText->setPos(5, y - barHeight / 2);
        m_scene->addItem(axisText);

        for (const auto& op : m_operations) {
            int groupId = m_title.contains("Машин") ? op.machineId : op.jobId;
            if (groupId != id) continue;

            int barY = y - barHeight / 2;
            QColor color = m_jobColorMap.value(op.jobId, Qt::blue);

            QString labelText;
            QString tooltipText ;

            if (m_title.contains("Машин")) {
                labelText = QString("J%1").arg(op.jobId);

                tooltipText = QString("M%1; J%2; Start: %3; Setup: %4; Dur: %5; Cost: %6")
                    .arg(op.machineId)
                    .arg(op.jobId)
                    .arg(op.startTime)
                    .arg(op.setupTime)
                    .arg(op.duration)
                    .arg(op.cost);
            }
            else {
                QVector<OperationData> relatedTopOps = m_pDB->bottomOpIdToGroup.value(op.id);
                QSet<int> machineSet;
                for (const auto& relatedOp : relatedTopOps)
                    machineSet.insert(relatedOp.machineId);

                QList<int> sortedMachines = QList<int>::fromSet(machineSet);
                std::sort(sortedMachines.begin(), sortedMachines.end());

                QStringList machineLabels;
                QStringList machineLabelsfortooltip;
                for (int mid : sortedMachines){
                    machineLabels << QString("M%1").arg(mid);
                    machineLabelsfortooltip << QString::number(mid);
                }


                labelText = machineLabels.join(",");
//                tooltipText = machineLabelsfortooltip.join(",");

                tooltipText = QString("M%1; J%2; Start: %3; Setup: %4; Dur: %5; Cost: %6")
                    .arg(machineLabelsfortooltip.join(","))
                    .arg(op.jobId)
                    .arg(op.startTime)
                    .arg(op.setupTime)
                    .arg(op.duration)
                    .arg(op.cost);

            }

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
                op.isHighlighted,
                op.setupTime,
                op.cost,
                labelText
            );


            bar->setToolTip(tooltipText);

            m_scene->addItem(bar);

            qreal centerY = bar->sceneBoundingRect().center().y();
            qDebug() << "[PopulateScene][Bar]" << bar->getOpId()
                     << "Axis id:" << id
                     << "Center Y:" << centerY
                     << "Expected axis center Y:" << y;
        }

    }

    // Нижняя замыкающая линия
    int lastY = yOffset + spacingY * axisCount;
    m_scene->addLine(spacingX, lastY, spacingX + timeUnit * maxFinishTime, lastY, QPen(Qt::gray));

    int sceneWidth = spacingX + timeUnit * (maxFinishTime + 1);
    int sceneHeight = yOffset + spacingY * axisCount;
    int paddingRight = viewWidth / 8;
    m_scene->setSceneRect(0, 0, sceneWidth + paddingRight, sceneHeight + bottomPadding);

    m_scene->setProperty("db", QVariant::fromValue(static_cast<void*>(m_pDB)));
    m_scene->setProperty("view", QVariant::fromValue(static_cast<void*>(this)));

    m_axisCenters.clear();
    for (int i = 0; i < sortedAxisIds.size(); ++i) {
        int id = sortedAxisIds[i];
        int y = yOffset + spacingY * i;
        m_axisCenters[id] = y;  // здесь уже координата по сцене
        qDebug() << "[PopulateScene] Axis id:" << id << "index:" << i
                 << "Y (center):" << y
                 << "yOffset:" << yOffset
                 << "spacingY:" << spacingY;
    }




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

    clearHighlights();

    // Удалить старые стрелки
    for (QGraphicsItem *item : m_scene->items()) {
        if (item->data(0).toString() == "arrow") {
            m_scene->removeItem(item);
            delete item;
        }
    }

    // Найти текущий бар
    GanttBarItem* currentBar = nullptr;
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
            if (bar->getOpId() == opId) {
                bar->setHighlighted(true);
                currentBar = bar;
            }
        }
    }

    // Обработка предшественников (только для Top Chart)
    if (m_title.contains("Машин") && currentBar) {
        OperationData currentOp;
        bool found = false;
        for (const auto &op : m_pDB->topOperations) {
            if (op.id == opId) {
                currentOp = op;
                found = true;
                break;
            }
        }

        if (found && !currentOp.predecessors.isEmpty()) {
            QList<GanttBarItem*> predBars;
            for (const QString &predId : currentOp.predecessors) {
                for (QGraphicsItem *item : m_scene->items()) {
                    if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
                        if (bar->getOpId() == predId) {
                            bar->setHighlighted(true);
                            predBars << bar;
                        }
                    }
                }
            }

            // Рисуем стрелки от предшественников к следующему (или к текущему, если он один)
            for (int i = 0; i < predBars.size(); ++i) {
                if (i + 1 < predBars.size()) {
                    GanttBarItem::drawArrow(m_scene, predBars[i]->sceneBoundingRect().center(), predBars[i + 1]->sceneBoundingRect().center());
                } else {
                    GanttBarItem::drawArrow(m_scene, predBars[i]->sceneBoundingRect().center(), currentBar->sceneBoundingRect().center());
                }
            }
        }
    }

    // Подсветка связанных операций по времени
    QSet<QString> printedIds;
    for (const auto &pair : m_pDB->topOpIdToGroup) {
        for (const auto &op : pair.first + pair.second) {
            if (printedIds.contains(op.id) || op.id == opId) continue;
            if (op.jobId == jobId && (op.startTime + op.duration == m_startTime + m_duration)) {
                printedIds.insert(op.id);
                for (QGraphicsItem *item : m_scene->items()) {
                    if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
                        if (bar->getOpId() == op.id) bar->setHighlighted(true);
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
                for (QGraphicsItem *item : m_scene->items()) {
                    if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
                        if (bar->getOpId() == op.id) bar->setHighlighted(true);
                    }
                }
            }
        }
    }
}


//void GanttView::clearHighlights() {
//    for (QGraphicsItem *item : m_scene->items()) {
//        if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
//            bar->setHighlighted(false);
//        }
//    }
//}


void GanttView::clearHighlights() {
    // Сначала снимаем выделение у баров
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
            bar->setHighlighted(false);
        }
    }

    // Теперь удаляем стрелки
    QList<QGraphicsItem *> arrowsToRemove;
    for (QGraphicsItem *item : m_scene->items()) {
        if (item->data(0).toString() == "arrow") {
            arrowsToRemove << item;
        }
    }
    for (QGraphicsItem *arrow : arrowsToRemove) {
        m_scene->removeItem(arrow);
        delete arrow;
    }
}




void GanttView::startCostPulse() {
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
            bar->startPulse();
        }
    }
}

void GanttView::stopCostPulse() {
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
            bar->stopPulse();
        }
    }
}


void GanttView::updateOperations(const QVector<OperationData> &ops) {
    m_operations = ops;
    populateScene();
}



void GanttView::setEditMode(bool on) {
    m_beditMode = on;
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
            bar->setFlag(QGraphicsItem::ItemIsMovable, m_beditMode);
        }
    }

    if (!on) {
        snapBarsToAxis();
    }
}


void GanttView::snapBarsToAxis() {
    qDebug() << "=== Snap Bars Start ===";

    for (QGraphicsItem *item : m_scene->items()) {
        if (auto *bar = dynamic_cast<GanttBarItem*>(item)) {
            QRectF boundingRect = bar->sceneBoundingRect();
            qreal barCenterY = boundingRect.center().y();

            qDebug() << "Bar ID:" << bar->getOpId()
                     << "Scene center Y:" << barCenterY;

            // Найти ближайшую ось
            qreal minDistance = std::numeric_limits<qreal>::max();
            qreal closestYCenter = barCenterY;

            for (auto it = m_axisCenters.constBegin(); it != m_axisCenters.constEnd(); ++it) {
                qreal axisCenterY = it.value();
                qreal dist = std::abs(axisCenterY - barCenterY);

                qDebug() << "  Axis id:" << it.key() << "axisCenterY:" << axisCenterY << "dist:" << dist;

                if (dist < minDistance) {
                    minDistance = dist;
                    closestYCenter = axisCenterY;
                }
            }

            // Рассчитать сдвиг относительно текущего центра
            QPointF currentPos = bar->pos();
            QRectF localRect = bar->rect();

            qreal offsetY = closestYCenter - boundingRect.center().y();
            QPointF newPos = currentPos + QPointF(0, offsetY);

            bar->setPos(newPos);

            qDebug() << "  --> Snapped to center Y:" << closestYCenter
                     << "New pos:" << newPos;
        }
    }

    qDebug() << "=== Snap Bars End ===";
}




bool GanttView::isEditMode() const { return m_beditMode; }


void GanttView::showAddBarDialog() {
    if (!m_beditMode) return;

    QDialog dialog;
    dialog.setWindowTitle("Добавить новый бар");

    QFormLayout form(&dialog);

    QLineEdit *idEdit = new QLineEdit(&dialog);
    QLineEdit *startTimeEdit = new QLineEdit(&dialog);
    QLineEdit *durationEdit = new QLineEdit(&dialog);
    QLineEdit *setupTimeEdit = new QLineEdit(&dialog);
    QLineEdit *costEdit = new QLineEdit(&dialog);

    form.addRow("Job ID:", idEdit);
    form.addRow("Start Time:", startTimeEdit);
    form.addRow("Duration:", durationEdit);
    form.addRow("Setup Time:", setupTimeEdit);
    form.addRow("Cost:", costEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        // Читаем данные
        QString id = idEdit->text();
        int jobId = id.toInt();
        int startTime = startTimeEdit->text().toInt();
        int duration = durationEdit->text().toInt();
        int setupTime = setupTimeEdit->text().toInt();
        int cost = costEdit->text().toInt();

        // Создаем OperationData
        OperationData newOp;
        newOp.id = id;
        newOp.jobId = jobId;
        newOp.startTime = startTime;
        newOp.duration = duration;
        newOp.setupTime = setupTime;
        newOp.cost = cost;
        newOp.machineId = m_title.contains("Машин") ? jobId : 1;  // Допустим, если машинный график, jobId как machineId

        m_operations.append(newOp);

        // Перерисовываем
        populateScene();
    }
}
