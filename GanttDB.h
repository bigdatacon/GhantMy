// GanttDB.h
#pragma once
#include <QString>
#include <QVector>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSet>


struct OperationData {
    QString id;
    int machineId;
    int jobId;
    int startTime;
    int duration;
    int setupTime;
    QString name;
    int cost;
    QStringList predecessors;
    bool isHighlighted = false;  // <- добавляем поле
};

class GanttView; // В начало файла, до объявления GanttDB

class GanttDB {
public:
    QVector<OperationData> topOperations;
    QVector<OperationData> bottomOperations;
    int maxFinishTop = 0;
    int maxFinishBottom = 0;
    int uniqueJobCountTop = 0;
    int uniqueJobCountBottom = 0;


    // Новые поля:
    QMap<int, QSet<int>> topJobToMachines;                 // JobId → множество MachineId
    QMap<int, QSet<int>> bottomJobToMachines;              // JobId → множество MachineId

    // Используется в populateScene для отображения подписей внутри баров
    QMap<QString, QPair<QVector<OperationData>, QVector<OperationData>>> topOpIdToGroup;
    QMap<QString, QVector<OperationData>> bottomOpIdToGroup;


    static GanttDB& instance() {
        static GanttDB inst;
        return inst;
    }

    void loadFromJson(const QString &filename);
    void writeToDatabase();
    void loadFromDatabase();


    // 👇 Новые поля для доступа к GanttView
    GanttView* topView = nullptr;
    GanttView* bottomView = nullptr;

private:
    GanttDB();
    void connectDatabase();
    QSqlDatabase db;
};
