#include <QApplication>
#include "GanttView.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GanttView view;
    view.setWindowTitle("Интерактивная диаграмма Ганта");
    view.resize(1000, 600);
    view.show();

    return app.exec();
}
