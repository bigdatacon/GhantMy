#include "GanttView.h"
#include "GanttBarItem.h"
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QWheelEvent> // Не забудь подключить!


GanttView::GanttView(QWidget *parent) : QGraphicsView(parent), m_scene(new QGraphicsScene(this)) {
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(AnchorUnderMouse);

    // Размеры
    const int machineCount = 3;
    const int tasksPerMachine = 3;
    const int timeUnit = 40;
    const int barHeight = 20;
    const int spacingY = 60;
    const int spacingX = 50;

    // Рисуем ось X
    for (int t = 0; t <= 10; ++t) {
        int x = spacingX + t * timeUnit;
        m_scene->addLine(x, 0, x, spacingY * (machineCount + 1), QPen(Qt::lightGray));
        auto *label = new QGraphicsTextItem(QString::number(t));
        label->setPos(x - 5, 0);
        m_scene->addItem(label);
    }

    // Рисуем ось Y и бары
    for (int m = 0; m < machineCount; ++m) {
        int y = spacingY * (m + 1);
        m_scene->addLine(spacingX, y, spacingX + timeUnit * 10, y, QPen(Qt::gray));

        auto *machineLabel = new QGraphicsTextItem(QString("М%1").arg(m + 1));
        machineLabel->setPos(0, y - 10);
        m_scene->addItem(machineLabel);

        for (int j = 0; j < tasksPerMachine; ++j) {
            int startTime = (j + m) * 2;
            int duration = 2 + (j % 2);
            auto *bar = new GanttBarItem(m, j, startTime, duration, timeUnit, spacingX, y - barHeight / 2);
            m_scene->addItem(bar);
        }
    }
}


void GanttView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        double scaleFactor = (event->angleDelta().y() > 0) ? 1.1 : 0.9;
        scale(scaleFactor, 1.0);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}
