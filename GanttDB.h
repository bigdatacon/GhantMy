// GanttDB.h
#pragma once
#include <QString>
#include <QVector>
#include <QSqlDatabase>
#include <QStringList>


struct OperationData {
    int id;
    int machineId;
    int jobId;
    int startTime;
    int duration;
    int setupTime;
    QStringList predecessors; // Строка вида "1,4,6"
};

class GanttDB {
public:
    GanttDB(const QString &dbPath);
    bool initialize();
    void populateSampleData();

    QVector<OperationData> loadTable(const QString &tableName);
    void insertOperation(const QString &tableName, const OperationData &data);

private:
    QSqlDatabase m_db;
    bool createTables();
};
