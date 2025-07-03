
// main.cpp
#include <QApplication>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include "GanttView.h"
#include "GanttDB.h"
#include <QPushButton>




//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    GanttDB::instance().loadFromJson("operations.json");

//    GanttDB::instance().writeToDatabase();
//    GanttDB::instance().loadFromDatabase();

//    QWidget mainWidget;
//    mainWidget.setWindowTitle("Диаграмма Ганта — два графика + зум");
//    mainWidget.resize(1000, 800);

//    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);
//    GanttView *topChart = new GanttView("График Машин", GanttDB::instance().topOperations, &GanttDB::instance(), GanttDB::instance().maxFinishTop, GanttDB::instance().uniqueJobCountTop);
//    GanttView *bottomChart = new GanttView("График Работ", GanttDB::instance().bottomOperations, &GanttDB::instance(), GanttDB::instance().maxFinishBottom, GanttDB::instance().uniqueJobCountBottom);

////    GanttDB& db = GanttDB::instance();
//    GanttDB::instance().topView = topChart;
//    GanttDB::instance().bottomView = bottomChart;


//    QSplitter *splitter = new QSplitter(Qt::Vertical);
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

//#include <QApplication>
//#include <QSplitter>
//#include <QVBoxLayout>
//#include <QWidget>
//#include <QPushButton>
//#include <QHBoxLayout>
//#include "GanttView.h"
//#include "GanttDB.h"

//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    GanttDB::instance().loadFromJson("operations.json");
//    GanttDB::instance().writeToDatabase();
//    GanttDB::instance().loadFromDatabase();

//    QWidget mainWidget;
//    mainWidget.setWindowTitle("Диаграмма Ганта — два графика + меню");
//    mainWidget.resize(1000, 800);

//    // Верхняя панель с кнопками
//    QWidget *menuWidget = new QWidget();
//    QHBoxLayout *menuLayout = new QHBoxLayout(menuWidget);
//    QPushButton *showCostButton = new QPushButton("ShowCost");
//    menuLayout->addWidget(showCostButton);
//    menuLayout->addStretch(); // Чтобы кнопка ушла влево

//    // Графики
//    GanttView *topChart = new GanttView("График Машин", GanttDB::instance().topOperations, &GanttDB::instance(), GanttDB::instance().maxFinishTop, GanttDB::instance().uniqueJobCountTop);
//    GanttView *bottomChart = new GanttView("График Работ", GanttDB::instance().bottomOperations, &GanttDB::instance(), GanttDB::instance().maxFinishBottom, GanttDB::instance().uniqueJobCountBottom);

//    GanttDB::instance().topView = topChart;
//    GanttDB::instance().bottomView = bottomChart;

//    // Сплиттер для графиков
//    QSplitter *splitter = new QSplitter(Qt::Vertical);
//    topChart->setMinimumHeight(400);
//    bottomChart->setMinimumHeight(400);
//    splitter->addWidget(topChart);
//    splitter->addWidget(bottomChart);
//    splitter->setStretchFactor(0, 1);
//    splitter->setStretchFactor(1, 1);

//    // Основный лэйаут
//    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);
//    layout->addWidget(menuWidget);    // Добавляем меню сверху
//    layout->addWidget(splitter);      // Графики снизу

//    mainWidget.show();
//    return app.exec();
//}


#include <QApplication>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "GanttView.h"
#include "GanttDB.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GanttDB::instance().loadFromJson("operations.json");
    GanttDB::instance().writeToDatabase();
    GanttDB::instance().loadFromDatabase();

    QWidget mainWidget;
    mainWidget.setWindowTitle("Диаграмма Ганта — два графика + меню");
    mainWidget.resize(1000, 800);

    // Верхняя панель с кнопками
    QWidget *menuWidget = new QWidget();
    QHBoxLayout *menuLayout = new QHBoxLayout(menuWidget);
    QPushButton *showCostButton = new QPushButton("ShowCost");
    QPushButton *offCostButton = new QPushButton("OffCost");
    // Кнопка Refresh
    QPushButton *refreshButton = new QPushButton("Refresh");
    menuLayout->addWidget(refreshButton);
    QPushButton *editModeBtn = new QPushButton("Edit Mode");
    QPushButton *viewModeBtn = new QPushButton("View Mode");

    QPushButton *saveButton = new QPushButton("Сохранить изменения");
    menuLayout->addWidget(saveButton);


    //кнопка добавить бар
    QPushButton *addBarButton = new QPushButton("Add Bar");
    menuLayout->addWidget(addBarButton);


    menuLayout->addWidget(showCostButton);
    menuLayout->addWidget(offCostButton);
    menuLayout->addStretch();

    menuLayout->addWidget(editModeBtn);
    menuLayout->addWidget(viewModeBtn);



    // Графики
    GanttView *topChart = new GanttView("График Машин", GanttDB::instance().topOperations, &GanttDB::instance(), GanttDB::instance().maxFinishTop, GanttDB::instance().uniqueJobCountTop);
    GanttView *bottomChart = new GanttView("График Работ", GanttDB::instance().bottomOperations, &GanttDB::instance(), GanttDB::instance().maxFinishBottom, GanttDB::instance().uniqueJobCountBottom);

    GanttDB::instance().topView = topChart;
    GanttDB::instance().bottomView = bottomChart;

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    topChart->setMinimumHeight(400);
    bottomChart->setMinimumHeight(400);
    splitter->addWidget(topChart);
    splitter->addWidget(bottomChart);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    QVBoxLayout *layout = new QVBoxLayout(&mainWidget);
    layout->addWidget(menuWidget);
    layout->addWidget(splitter);




    // Обработка кнопок
    QObject::connect(showCostButton, &QPushButton::clicked, [&]() {
        if (GanttDB::instance().topView) GanttDB::instance().topView->startCostPulse();
        if (GanttDB::instance().bottomView) GanttDB::instance().bottomView->startCostPulse();
    });

    QObject::connect(offCostButton, &QPushButton::clicked, [&]() {
        if (GanttDB::instance().topView) GanttDB::instance().topView->stopCostPulse();
        if (GanttDB::instance().bottomView) GanttDB::instance().bottomView->stopCostPulse();
    });

    // Слот для Refresh
    QObject::connect(refreshButton, &QPushButton::clicked, [&]() {
        if (GanttDB::instance().topView) {
            GanttDB::instance().topView->clearHighlights();
        }
        if (GanttDB::instance().bottomView) {
            GanttDB::instance().bottomView->clearHighlights();
        }
        GanttDB::instance().loadFromDatabase();
        if (GanttDB::instance().topView) {
            GanttDB::instance().topView->updateOperations(GanttDB::instance().topOperations);
        }
        if (GanttDB::instance().bottomView) {
            GanttDB::instance().bottomView->updateOperations(GanttDB::instance().bottomOperations);
        }
    });

    QObject::connect(editModeBtn, &QPushButton::clicked, [&]() {
        topChart->setEditMode(true);
        bottomChart->setEditMode(true);
    });

    QObject::connect(viewModeBtn, &QPushButton::clicked, [&]() {
        topChart->setEditMode(false);
        bottomChart->setEditMode(false);
    });



    // Коннект
    QObject::connect(addBarButton, &QPushButton::clicked, [&]() {
        if (topChart->isEditMode()) {
            topChart->showAddBarDialog();
        }
        if (bottomChart->isEditMode()) {
            bottomChart->showAddBarDialog();
        }
    });

    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        qDebug() << "==== Сохраняем текущие данные в БД ====";

        // Сохраняем текущие данные в БД
        GanttDB::instance().writeToDatabase();

        // Перезагружаем из БД (для проверки)
        GanttDB::instance().loadFromDatabase();

        // Обновляем графики
        if (GanttDB::instance().topView) {
            GanttDB::instance().topView->updateOperations(GanttDB::instance().topOperations);
        }
        if (GanttDB::instance().bottomView) {
            GanttDB::instance().bottomView->updateOperations(GanttDB::instance().bottomOperations);
        }

        // Выводим debug
        qDebug() << "--- TOP OPERATIONS ---";
        for (const auto &op : GanttDB::instance().topOperations) {
            qDebug() << "ID:" << op.id
                     << "Machine:" << op.machineId
                     << "Job:" << op.jobId
                     << "Start:" << op.startTime
                     << "Dur:" << op.duration
                     << "Setup:" << op.setupTime
                     << "Name:" << op.name
                     << "Cost:" << op.cost
                     << "Preds:" << op.predecessors;
        }

        qDebug() << "--- BOTTOM OPERATIONS ---";
        for (const auto &op : GanttDB::instance().bottomOperations) {
            qDebug() << "ID:" << op.id
                     << "Machine:" << op.machineId
                     << "Job:" << op.jobId
                     << "Start:" << op.startTime
                     << "Dur:" << op.duration
                     << "Setup:" << op.setupTime
                     << "Name:" << op.name
                     << "Cost:" << op.cost
                     << "Preds:" << op.predecessors;
        }

        qDebug() << "=== Сохранение и проверка завершены ===";
    });


    mainWidget.show();
    return app.exec();
}


