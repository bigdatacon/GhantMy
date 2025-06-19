// GanttDB.cpp
#include "GanttDB.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

GanttDB::GanttDB(const QString &dbPath) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
}

bool GanttDB::initialize() {
    if (!m_db.open()) {
        qDebug() << "Cannot open DB:" << m_db.lastError().text();
        return false;
    }
    return createTables();
}

bool GanttDB::createTables() {
    QSqlQuery query;
    const QString ddl = R"(
        CREATE TABLE IF NOT EXISTS %1 (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            machine_id INTEGER,
            job_id INTEGER,
            start_time INTEGER,
            duration INTEGER,
            setup_time INTEGER,
            predecessors TEXT
        )
    )";
    return query.exec(ddl.arg("TopChartData")) && query.exec(ddl.arg("BottomChartData"));
}

void GanttDB::populateSampleData() {
    QSqlQuery q;
    q.exec("DELETE FROM TopChartData");
    q.exec("DELETE FROM BottomChartData");

    for (int m = 0; m < 3; ++m) {
        for (int j = 0; j < 3; ++j) {
            OperationData op;
            op.machineId = m;
            op.jobId = j;
            op.startTime = (j + m) * 2;
            op.duration = 2 + (j % 2);
            op.setupTime = op.duration / 2;
            op.predecessors = QStringList();

            if (j == 2) {
                op.predecessors << QString::number((m * 3) + 1) << QString::number(((m - 1) * 3));
            }
            insertOperation("TopChartData", op);
            insertOperation("BottomChartData", op);
        }
    }
}

void GanttDB::insertOperation(const QString &tableName, const OperationData &data) {
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (machine_id, job_id, start_time, duration, setup_time, predecessors) "
                         "VALUES (?, ?, ?, ?, ?, ?)").arg(tableName));
    query.addBindValue(data.machineId);
    query.addBindValue(data.jobId);
    query.addBindValue(data.startTime);
    query.addBindValue(data.duration);
    query.addBindValue(data.setupTime);
    query.addBindValue(data.predecessors.join(","));
    if (!query.exec()) {
        qDebug() << "Insert error:" << query.lastError();
    }
}

QVector<OperationData> GanttDB::loadTable(const QString &tableName) {
    QVector<OperationData> data;
    QSqlQuery query(QString("SELECT id, machine_id, job_id, start_time, duration, setup_time, predecessors FROM %1").arg(tableName));
    while (query.next()) {
        OperationData op;
        op.id = query.value(0).toInt();
        op.machineId = query.value(1).toInt();
        op.jobId = query.value(2).toInt();
        op.startTime = query.value(3).toInt();
        op.duration = query.value(4).toInt();
        op.setupTime = query.value(5).toInt();
        op.predecessors = query.value(6).toString().split(",", Qt::SkipEmptyParts);
        data.append(op);
    }
    return data;
}
