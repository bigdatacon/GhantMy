//#include <QApplication>
//#include "GanttView.h"

// main.cpp
#include <QApplication>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include "GanttView.h"
#include "GanttDB.h"


//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    GanttView view;
//    view.setWindowTitle("Интерактивная диаграмма Ганта");
//    view.resize(1000, 600);
//    view.show();

//    return app.exec();
//}



//// main.cpp
//#include <QApplication>
//#include <QVBoxLayout>
//#include <QWidget>
//#include "GanttView.h"

//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    QWidget mainWidget;
//    mainWidget.setWindowTitle("Диаграмма Ганта — два графика + зум");
//    mainWidget.resize(1000, 800);

//    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);

//    GanttView *topChart = new GanttView("Top Chart");
//    GanttView *bottomChart = new GanttView("Bottom Chart");

//    layout->addWidget(topChart);
//    layout->addWidget(bottomChart);

//    mainWidget.show();
//    return app.exec();
//}


// main.cpp
#include <QApplication>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include "GanttView.h"

//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    QWidget mainWidget;
//    mainWidget.setWindowTitle("Диаграмма Ганта — два графика + зум");
//    mainWidget.resize(1000, 800);

//    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);

//    QSplitter *splitter = new QSplitter(Qt::Vertical);
//    GanttView *topChart = new GanttView("Top Chart");
//    GanttView *bottomChart = new GanttView("Bottom Chart");

//    // Настройка минимальных размеров и растягивания
//    topChart->setMinimumHeight(400);
//    bottomChart->setMinimumHeight(400);

//    splitter->addWidget(topChart);
//    splitter->addWidget(bottomChart);
//    splitter->setStretchFactor(0, 1);
//    splitter->setStretchFactor(1, 1);

//    layout->addWidget(splitter);

//    mainWidget.show();
//    return app.exec();
//}


//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    GanttDB::instance().loadFromJson("operations.json");
//    GanttDB::instance().writeToDatabase();
//    GanttDB::instance().loadFromDatabase();

//    QWidget mainWidget;
//    mainWidget.setWindowTitle("Диаграмма Ганта — два графика + зум");
//    mainWidget.resize(1000, 800);

//    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);

//    QSplitter *splitter = new QSplitter(Qt::Vertical);
//    GanttView *topChart = new GanttView("Top Chart");
//    GanttView *bottomChart = new GanttView("Bottom Chart");

//    // Настройка минимальных размеров и растягивания
//    topChart->setMinimumHeight(100);
//    bottomChart->setMinimumHeight(100);

//    splitter->addWidget(topChart);
//    splitter->addWidget(bottomChart);
//    splitter->setStretchFactor(0, 1);
//    splitter->setStretchFactor(1, 1);

//    layout->addWidget(splitter);

//    mainWidget.show();
//    return app.exec();
//} // Обновлённый populateScene (вставить в GanttView.cpp)


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GanttDB::instance().loadFromJson("operations.json");
    GanttDB::instance().writeToDatabase();
    GanttDB::instance().loadFromDatabase();

    QWidget mainWidget;
    mainWidget.setWindowTitle("Диаграмма Ганта — два графика + зум");
    mainWidget.resize(1000, 800);

    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    GanttView *topChart = new GanttView("Top Chart", GanttDB::instance().topOperations, &GanttDB::instance());
    GanttView *bottomChart = new GanttView("Bottom Chart", GanttDB::instance().bottomOperations, &GanttDB::instance());

    topChart->setMinimumHeight(400);
    bottomChart->setMinimumHeight(400);

    splitter->addWidget(topChart);
    splitter->addWidget(bottomChart);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    mainWidget.show();
    return app.exec();
}

