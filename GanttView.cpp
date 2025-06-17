
#include "GanttView.h"
#include "GanttBarItem.h"
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QDebug>


GanttView::GanttView(const QString &title, QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this)), m_title(title)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
//    setTransformationAnchor(AnchorUnderMouse);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);


//    populateScene();
}

//void GanttView::populateScene() {
//    m_scene->clear();  // Удаляет все старые линии, бары, подписи
//    int viewWidth = viewport()->width();
//    int viewHeight = viewport()->height();

//    const int machineCount = 3;
//    const int tasksPerMachine = 3;


//    int spacingX = viewWidth / 20;      // отступ от левого края
//    int spacingY = viewHeight / (machineCount + 2); // строка на машину
//    int barHeight = spacingY * 0.6;     // бар занимает 60% строки
//    int timeUnit = viewWidth / 30;      // ширина единицы времени

//    int titleHeight = viewHeight / 15; // адаптивная высота заголовка
//    int yOffset = spacingY + titleHeight;  // заголовок + одна строка сверху


////    const int machineCount = 3;
////    const int tasksPerMachine = 3;
////    const int timeUnit = 40;
////    const int barHeight = 20;
////    const int spacingY = 60;
////    const int spacingX = 50;

//    // Название графика
//    QGraphicsTextItem *titleItem = new QGraphicsTextItem(m_title);
//    QFont titleFont;
//    titleFont.setBold(true);
//    titleFont.setPointSize(12);
//    titleItem->setFont(titleFont);
//    titleItem->setPos(spacingX, 0);
//    m_scene->addItem(titleItem);

////    int yOffset = 40;  // отступ вниз после заголовка

//    // Ось X
//    for (int t = 0; t <= 10; ++t) {
//        int x = spacingX + t * timeUnit;
//        m_scene->addLine(x, yOffset, x, yOffset + spacingY * machineCount, QPen(Qt::lightGray));
//        auto *label = new QGraphicsTextItem(QString::number(t));
//        label->setPos(x - 5, yOffset - 20);
//        m_scene->addItem(label);
//    }

//    // Ось Y и бары
//    for (int m = 0; m < machineCount; ++m) {
//        int y = yOffset + spacingY * m;
//        m_scene->addLine(spacingX, y, spacingX + timeUnit * 10, y, QPen(Qt::gray));

//        auto *machineLabel = new QGraphicsTextItem(QString("М%1").arg(m + 1));
//        machineLabel->setPos(5, y - barHeight / 2);
//        m_scene->addItem(machineLabel);

//        for (int j = 0; j < tasksPerMachine; ++j) {
//            int startTime = (j + m) * 2;
//            int duration = 2 + (j % 2);
//            auto *bar = new GanttBarItem(m, j, startTime, duration, timeUnit, spacingX, y - barHeight / 2);
//            m_scene->addItem(bar);
//        }
//    }

////    constexpr int paddingX = 500;
////    constexpr int paddingY = 200;

////    int sceneWidth = spacingX + timeUnit * 11;
////    int sceneHeight = yOffset + spacingY * machineCount;

////    QRectF contentRect(0, 0, sceneWidth, sceneHeight);
////    m_scene->setSceneRect(contentRect.adjusted(-paddingX, -paddingY, paddingX, paddingY));

//    int sceneWidth = spacingX + timeUnit * 11;
//    int sceneHeight = yOffset + spacingY * machineCount;

//    int paddingRight = viewWidth / 8;
//    int paddingBottom = viewHeight / 10;

//    // Только вправо и вниз — не смещаем вверх/влево
//    m_scene->setSceneRect(0, 0, sceneWidth + paddingRight, sceneHeight + paddingBottom);

//}


//void GanttView::populateScene() {
//    m_scene->clear();

//    int viewWidth = viewport()->width();
//    int viewHeight = viewport()->height();

//    const int machineCount = 3;
//    const int tasksPerMachine = 3;

//    int spacingX = viewWidth / 20;
//    int spacingY = viewHeight / (machineCount + 3);
//    int barHeight = spacingY * 0.6;
//    int timeUnit = (viewWidth - spacingX * 2) / 12;

//    int titleHeight = spacingY;
//    int yOffset = spacingY + titleHeight;

//    // Заголовок
//    QGraphicsTextItem *titleItem = new QGraphicsTextItem(m_title);
//    QFont titleFont;
//    titleFont.setBold(true);
//    titleFont.setPointSize(12);
//    titleItem->setFont(titleFont);
//    titleItem->setPos(spacingX, spacingY / 3);
//    m_scene->addItem(titleItem);

//    // Ось X
//    for (int t = 0; t <= 10; ++t) {
//        int x = spacingX + t * timeUnit;
//        m_scene->addLine(x, yOffset, x, yOffset + spacingY * machineCount, QPen(Qt::lightGray));
//        auto *label = new QGraphicsTextItem(QString::number(t));
//        label->setPos(x - 5, yOffset - spacingY / 1.5);
//        m_scene->addItem(label);
//    }

//    // Ось Y и бары
//    for (int m = 0; m < machineCount; ++m) {
//        int y = yOffset + spacingY * m;
//        m_scene->addLine(spacingX, y, spacingX + timeUnit * 10, y, QPen(Qt::gray));

//        auto *machineLabel = new QGraphicsTextItem(QString("М%1").arg(m + 1));
//        machineLabel->setPos(5, y - barHeight / 2);
//        m_scene->addItem(machineLabel);

//        for (int j = 0; j < tasksPerMachine; ++j) {
//            int startTime = (j + m) * 2;
//            int duration = 2 + (j % 2);
//            auto *bar = new GanttBarItem(m, j, startTime, duration, timeUnit, spacingX, y - barHeight / 2);
//            m_scene->addItem(bar);
//        }
//    }

//    int sceneWidth = spacingX + timeUnit * 11;
//    int sceneHeight = yOffset + spacingY * machineCount;

//    int paddingRight = viewWidth / 8;
//    int paddingBottom = viewHeight / 10;

//    m_scene->setSceneRect(0, 0, sceneWidth + paddingRight, sceneHeight + paddingBottom);
//}

//void GanttView::populateScene() {
//    m_scene->clear();

//    int viewWidth = viewport()->width();
//    int viewHeight = viewport()->height();

//    const int machineCount = 3;
//    const int tasksPerMachine = 3;

//    int spacingX = viewWidth / 20;
////    int spacingY = viewHeight / (machineCount + 3);
//    int spacingY = viewHeight / (machineCount );
//    int barHeight = spacingY * 0.6;
////    int timeUnit = (viewWidth - spacingX * 2) / 8;  // уменьшено для более раннего скролла

//    int titleHeight = spacingY;
//    int yOffset = spacingY + titleHeight;

//    int maxFinishTime = 0;
//    for (int m = 0; m < machineCount; ++m) {
//        for (int j = 0; j < tasksPerMachine; ++j) {
//            int startTime = (j + m) * 2;
//            int duration = 2 + (j % 2);
//            int finishTime = startTime + duration;
//            if (finishTime > maxFinishTime) maxFinishTime = finishTime;
//        }
//    }
//    int timeUnit = (viewWidth - spacingX * 2) / maxFinishTime;

//    // Заголовок
//    QGraphicsTextItem *titleItem = new QGraphicsTextItem(m_title);
//    QFont titleFont;
//    titleFont.setBold(true);

//    int baseFontSize = std::min(viewWidth, viewHeight) / 30;  // адаптивный размер шрифта
//    titleFont.setPointSize(baseFontSize);
//    titleItem->setFont(titleFont);

//    QFontMetricsF fm(titleFont);
//    qreal titleWidth = fm.boundingRect(m_title).width();
//    qreal centerX = viewWidth / 2 - titleWidth / 2;
//    titleItem->setPos(centerX, spacingY / 3);
//    m_scene->addItem(titleItem);

//    // Ось X
//    for (int t = 0; t <= 10; ++t) {
//        int x = spacingX + t * timeUnit;
//        m_scene->addLine(x, yOffset, x, yOffset + spacingY * machineCount, QPen(Qt::lightGray));
//        auto *label = new QGraphicsTextItem(QString::number(t));
//        QFont labelFont;
//        labelFont.setPointSize(baseFontSize * 0.8);
//        label->setFont(labelFont);
//        label->setPos(x - 5, yOffset - spacingY / 1.5);
//        m_scene->addItem(label);
//    }

//    // Ось Y и бары
//    for (int m = 0; m < machineCount; ++m) {
//        int y = yOffset + spacingY * m;
//        m_scene->addLine(spacingX, y, spacingX + timeUnit * 10, y, QPen(Qt::gray));

//        auto *machineLabel = new QGraphicsTextItem(QString("М%1").arg(m + 1));
//        QFont labelFont;
//        labelFont.setPointSize(baseFontSize * 0.8);
//        machineLabel->setFont(labelFont);
//        machineLabel->setPos(5, y - barHeight / 2);
//        m_scene->addItem(machineLabel);

//        for (int j = 0; j < tasksPerMachine; ++j) {
//            int startTime = (j + m) * 2;
//            int duration = 2 + (j % 2);
//            auto *bar = new GanttBarItem(m, j, startTime, duration, timeUnit, spacingX, y - barHeight / 2);
//            m_scene->addItem(bar);
//        }
//    }


//    int sceneWidth = spacingX + timeUnit * 11;
//    int sceneHeight = yOffset + spacingY * machineCount;

//    int paddingRight = viewWidth / 8;
//    int paddingBottom = viewHeight / 10;

//    m_scene->setSceneRect(0, 0, sceneWidth + paddingRight, sceneHeight + paddingBottom);
//}


void GanttView::populateScene() {
    m_scene->clear();

    int viewWidth = viewport()->width();
    int viewHeight = viewport()->height();

    const int machineCount = 3;
    const int tasksPerMachine = 3;

    int spacingX = viewWidth / 20;
    int titleHeight = viewHeight / 12;
    int topPadding = viewHeight / 10;
    int bottomPadding = viewHeight / 10;

    int availableHeight = viewHeight - titleHeight - topPadding - bottomPadding;
    int spacingY = availableHeight / machineCount;
    int barHeight = spacingY * 0.2;

    int maxFinishTime = 0;
    for (int m = 0; m < machineCount; ++m) {
        for (int j = 0; j < tasksPerMachine; ++j) {
            int startTime = (j + m) * 2;
            int duration = 2 + (j % 2);
            int finishTime = startTime + duration;
            if (finishTime > maxFinishTime) maxFinishTime = finishTime;
        }
    }

    int timeUnit = (viewWidth - spacingX * 2) / maxFinishTime;
    int baseFontSize = std::min(viewWidth, viewHeight) / 30;
    int yOffset = titleHeight + topPadding + baseFontSize;

    // Заголовок
    QGraphicsTextItem *titleItem = new QGraphicsTextItem(m_title);
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(baseFontSize);
    titleItem->setFont(titleFont);

    QFontMetricsF fm(titleFont);
    qreal titleWidth = fm.boundingRect(m_title).width();
    qreal centerX = viewWidth / 2 - titleWidth / 2;
    titleItem->setPos(centerX, 0);
    m_scene->addItem(titleItem);

    // Ось X
    for (int t = 0; t <= maxFinishTime; ++t) {
        int x = spacingX + t * timeUnit;
        qDebug() << "[AxisX] T" << t << ", X pos:" << x << ", Y start:" << yOffset << ", Y end:" << (yOffset + spacingY * machineCount);
        m_scene->addLine(x, yOffset, x, yOffset + spacingY * machineCount, QPen(Qt::lightGray));
        auto *label = new QGraphicsTextItem(QString::number(t));
        QFont labelFont;
        labelFont.setPointSize(baseFontSize * 0.8);
        label->setFont(labelFont);
//        label->setPos(x - 5, yOffset - baseFontSize * 2.0);
        QFontMetricsF labelMetrics(labelFont);
        qreal labelWidth = labelMetrics.boundingRect(QString::number(t)).width();
        int labelY = yOffset - std::max(barHeight, baseFontSize) * 2;

        label->setPos(x - labelWidth / 2, labelY);

        qDebug() << "[Label] T" << t << ", Y pos:" << yOffset - baseFontSize * 2.0;
        m_scene->addItem(label);
    }

    // Ось Y и бары
    for (int m = 0; m < machineCount; ++m) {
        int y = yOffset + spacingY * m;
        m_scene->addLine(spacingX, y, spacingX + timeUnit * maxFinishTime, y, QPen(Qt::gray));

        auto *machineLabel = new QGraphicsTextItem(QString("М%1").arg(m + 1));
        QFont labelFont;
        labelFont.setPointSize(baseFontSize * 0.8);
        machineLabel->setFont(labelFont);
        machineLabel->setPos(5, y - barHeight / 2);
        m_scene->addItem(machineLabel);

        for (int j = 0; j < tasksPerMachine; ++j) {
            int startTime = (j + m) * 2;
            int duration = 2 + (j % 2);
//            int barY = y + 1 - barHeight;  // Совмещение нижней границы бара с осью
            int barY = y -barHeight/2;  // Совмещение нижней границы бара с осью


            qDebug() << "[Bar] Machine M" << m+1
                     << ", Task J" << j
                     << ", Y pos (bar top):" << barY
                     << ", bar_height:" << barHeight
                     << ", axis_Y:" << y;
//            auto *bar = new GanttBarItem(m, j, startTime, duration, timeUnit, spacingX, barY);
            auto *bar = new GanttBarItem(m, j, startTime, duration, timeUnit, spacingX, barY, barHeight);

            m_scene->addItem(bar);
        }
    }

    int sceneWidth = spacingX + timeUnit * (maxFinishTime + 1);
    int sceneHeight = yOffset + spacingY * machineCount;

    int paddingRight = viewWidth / 8;
    m_scene->setSceneRect(0, 0, sceneWidth + paddingRight, sceneHeight + bottomPadding);
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

