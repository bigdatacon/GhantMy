// GanttDB.cpp
#include "GanttDB.h"

GanttDB::GanttDB() {
    connectDatabase();
}

void GanttDB::connectDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("gantt.db");
    if (!db.open()) {
        qWarning() << "Не удалось открыть БД:" << db.lastError().text();
    }
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS operations");
    query.exec(R"(
        CREATE TABLE operations (
            id TEXT PRIMARY KEY,
            machineId INTEGER,
            jobId INTEGER,
            startTime INTEGER,
            duration INTEGER,
            setupTime INTEGER,
            name TEXT,
            cost INTEGER,
            predecessors TEXT
        )
    )");
}

void GanttDB::loadFromJson(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть JSON файл:" << filename;
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QJsonArray topArray = root["top"].toArray();
    QJsonArray bottomArray = root["bottom"].toArray();

    auto parseArray = [](const QJsonArray &array) {
        QVector<OperationData> result;
        for (const auto &val : array) {
            QJsonObject obj = val.toObject();
            OperationData op;
            op.id = obj["id"].toString();
            op.machineId = obj["machineId"].toInt();
            op.jobId = obj["jobId"].toInt();
            op.startTime = obj["startTime"].toInt();
            op.duration = obj["duration"].toInt();
            op.setupTime = obj["setupTime"].toInt();
            op.name = obj["name"].toString();
            op.cost = obj["cost"].toInt();
//            op.predecessors = obj["predecessors"].toString().split(",", QString::SkipEmptyParts);
            op.predecessors = obj["predecessors"].toString().split(",", Qt::SkipEmptyParts);
//            op.predecessors = query.value("predecessors").toString().split(",", Qt::SkipEmptyParts);


            result.append(op);
        }
        return result;
    };

    topOperations = parseArray(topArray);
    bottomOperations = parseArray(bottomArray);
}

void GanttDB::writeToDatabase() {
    QSqlQuery query;
    auto insertOps = [&query](const QVector<OperationData> &ops) {
        for (const auto &op : ops) {
            query.prepare(R"(
                INSERT INTO operations (id, machineId, jobId, startTime, duration, setupTime, name, cost, predecessors)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
            )");
            query.addBindValue(op.id);
            query.addBindValue(op.machineId);
            query.addBindValue(op.jobId);
            query.addBindValue(op.startTime);
            query.addBindValue(op.duration);
            query.addBindValue(op.setupTime);
            query.addBindValue(op.name);
            query.addBindValue(op.cost);
            query.addBindValue(op.predecessors.join(","));
            if (!query.exec()) {
                qWarning() << "Ошибка вставки в БД:" << query.lastError().text();
            }
        }
    };
    insertOps(topOperations);
    insertOps(bottomOperations);
}


void GanttDB::loadFromDatabase() {
    topOperations.clear();
    bottomOperations.clear();

    QSqlQuery query("SELECT * FROM operations");
    while (query.next()) {
        OperationData op;
        op.id = query.value("id").toString();
        op.machineId = query.value("machineId").toInt();
        op.jobId = query.value("jobId").toInt();
        op.startTime = query.value("startTime").toInt();
        op.duration = query.value("duration").toInt();
        op.setupTime = query.value("setupTime").toInt();
        op.name = query.value("name").toString();
        op.cost = query.value("cost").toInt();
//        op.predecessors = query.value("predecessors").toString().split(",", QString::SkipEmptyParts);
//        op.predecessors = obj["predecessors"].toString().split(",", Qt::SkipEmptyParts);
        op.predecessors = query.value("predecessors").toString().split(",", Qt::SkipEmptyParts);


        if (op.id.endsWith("_up"))
            topOperations.append(op);
        else if (op.id.endsWith("_down"))
            bottomOperations.append(op);
    }
}
