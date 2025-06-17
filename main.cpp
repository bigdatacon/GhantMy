//#include <QApplication>
//#include "GanttView.h"

//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    GanttView view;
//    view.setWindowTitle("Интерактивная диаграмма Ганта");
//    view.resize(1000, 600);
//    view.show();

//    return app.exec();
//}



// main.cpp
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include "GanttView.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget mainWidget;
    mainWidget.setWindowTitle("Диаграмма Ганта — два графика + зум");
    mainWidget.resize(1000, 800);

    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);

    GanttView *topChart = new GanttView("Top Chart");
    GanttView *bottomChart = new GanttView("Bottom Chart");

    layout->addWidget(topChart);
    layout->addWidget(bottomChart);

    mainWidget.show();
    return app.exec();
}
