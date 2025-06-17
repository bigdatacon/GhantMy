//#include "GanttView.h"
//#include "GanttBarItem.h"
//#include <QGraphicsLineItem>
//#include <QGraphicsTextItem>
//#include <QWheelEvent> // Не забудь подключить!


//GanttView::GanttView(QWidget *parent) : QGraphicsView(parent), m_scene(new QGraphicsScene(this)) {
//    setScene(m_scene);
//    setRenderHint(QPainter::Antialiasing);
//    setDragMode(QGraphicsView::ScrollHandDrag);
//    setTransformationAnchor(AnchorUnderMouse);

//    // Размеры
//    const int machineCount = 3;
//    const int tasksPerMachine = 3;
//    const int timeUnit = 40;
//    const int barHeight = 20;
//    const int spacingY = 60;
//    const int spacingX = 50;

//    // Рисуем ось X
//    for (int t = 0; t <= 10; ++t) {
//        int x = spacingX + t * timeUnit;
//        m_scene->addLine(x, 0, x, spacingY * (machineCount + 1), QPen(Qt::lightGray));
//        auto *label = new QGraphicsTextItem(QString::number(t));
//        label->setPos(x - 5, 0);
//        m_scene->addItem(label);
//    }

//    // Рисуем ось Y и бары
//    for (int m = 0; m < machineCount; ++m) {
//        int y = spacingY * (m + 1);
//        m_scene->addLine(spacingX, y, spacingX + timeUnit * 10, y, QPen(Qt::gray));

//        auto *machineLabel = new QGraphicsTextItem(QString("М%1").arg(m + 1));
//        machineLabel->setPos(0, y - 10);
//        m_scene->addItem(machineLabel);

//        for (int j = 0; j < tasksPerMachine; ++j) {
//            int startTime = (j + m) * 2;
//            int duration = 2 + (j % 2);
//            auto *bar = new GanttBarItem(m, j, startTime, duration, timeUnit, spacingX, y - barHeight / 2);
//            m_scene->addItem(bar);
//        }
//    }
//}


//void GanttView::wheelEvent(QWheelEvent *event) {
//    if (event->modifiers() & Qt::ControlModifier) {
//        double scaleFactor = (event->angleDelta().y() > 0) ? 1.1 : 0.9;
////        scale(scaleFactor, 1.0);
//        scale(scaleFactor, scaleFactor);
//    } else {
//        QGraphicsView::wheelEvent(event);
//    }
//}


#include "GanttView.h"
#include "GanttBarItem.h"
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QWheelEvent>

GanttView::GanttView(const QString &title, QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this)), m_title(title)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(AnchorUnderMouse);

    populateScene();
}

void GanttView::populateScene() {
    const int machineCount = 3;
    const int tasksPerMachine = 3;
    const int timeUnit = 40;
    const int barHeight = 20;
    const int spacingY = 60;
    const int spacingX = 50;

    // Название графика
    QGraphicsTextItem *titleItem = new QGraphicsTextItem(m_title);
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(12);
    titleItem->setFont(titleFont);
    titleItem->setPos(spacingX, 0);
    m_scene->addItem(titleItem);

    int yOffset = 40;  // отступ вниз после заголовка

    // Ось X
    for (int t = 0; t <= 10; ++t) {
        int x = spacingX + t * timeUnit;
        m_scene->addLine(x, yOffset, x, yOffset + spacingY * machineCount, QPen(Qt::lightGray));
        auto *label = new QGraphicsTextItem(QString::number(t));
        label->setPos(x - 5, yOffset - 20);
        m_scene->addItem(label);
    }

    // Ось Y и бары
    for (int m = 0; m < machineCount; ++m) {
        int y = yOffset + spacingY * m;
        m_scene->addLine(spacingX, y, spacingX + timeUnit * 10, y, QPen(Qt::gray));

        auto *machineLabel = new QGraphicsTextItem(QString("М%1").arg(m + 1));
        machineLabel->setPos(5, y - barHeight / 2);
        m_scene->addItem(machineLabel);

        for (int j = 0; j < tasksPerMachine; ++j) {
            int startTime = (j + m) * 2;
            int duration = 2 + (j % 2);
            auto *bar = new GanttBarItem(m, j, startTime, duration, timeUnit, spacingX, y - barHeight / 2);
            m_scene->addItem(bar);
        }
    }
//    // Установка границ сцены — предотвращает уход за пределы
//    // Установка границ сцены — добавим запас по ширине и высоте
//    int sceneWidth = spacingX + timeUnit * 11;
//    int sceneHeight = yOffset + spacingY * machineCount;

//    // Добавим небольшой запас (например, по 200 пикселей)
//    m_scene->setSceneRect(-200, -200, sceneWidth + 400, sceneHeight + 400);

    constexpr int paddingX = 500;
    constexpr int paddingY = 200;

    int sceneWidth = spacingX + timeUnit * 11;
    int sceneHeight = yOffset + spacingY * machineCount;

    QRectF contentRect(0, 0, sceneWidth, sceneHeight);
    m_scene->setSceneRect(contentRect.adjusted(-paddingX, -paddingY, paddingX, paddingY));




}

void GanttView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        double scaleFactor = (event->angleDelta().y() > 0) ? 1.1 : 0.9;
        scale(scaleFactor, scaleFactor);  // зум в обе оси
    } else {
        QGraphicsView::wheelEvent(event);
    }
}
