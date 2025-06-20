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
};

class GanttDB {
public:
    QVector<OperationData> topOperations;
    QVector<OperationData> bottomOperations;
    int maxFinishTop = 0;
    int maxFinishBottom = 0;
    int uniqueJobCountTop = 0;
    int uniqueJobCountBottom = 0;

    static GanttDB& instance() {
        static GanttDB inst;
        return inst;
    }

    void loadFromJson(const QString &filename);
    void writeToDatabase();
    void loadFromDatabase();

private:
    GanttDB();
    void connectDatabase();
    QSqlDatabase db;
};
