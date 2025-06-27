
// main.cpp
#include <QApplication>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include "GanttView.h"
#include "GanttDB.h"





int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GanttDB::instance().loadFromJson("operations.json");

    GanttDB::instance().writeToDatabase();
    GanttDB::instance().loadFromDatabase();

    QWidget mainWidget;
    mainWidget.setWindowTitle("Диаграмма Ганта — два графика + зум");
    mainWidget.resize(1000, 800);

    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);
    GanttView *topChart = new GanttView("Top Chart", GanttDB::instance().topOperations, &GanttDB::instance(), GanttDB::instance().maxFinishTop, GanttDB::instance().uniqueJobCountTop);
    GanttView *bottomChart = new GanttView("Bottom Chart", GanttDB::instance().bottomOperations, &GanttDB::instance(), GanttDB::instance().maxFinishBottom, GanttDB::instance().uniqueJobCountBottom);

//    GanttDB& db = GanttDB::instance();
    GanttDB::instance().topView = topChart;
    GanttDB::instance().bottomView = bottomChart;


    QSplitter *splitter = new QSplitter(Qt::Vertical);
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

