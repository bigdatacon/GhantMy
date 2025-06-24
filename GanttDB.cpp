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

    qDebug() << "TopArray size:" << topArray.size();
    for (int i = 0; i < topArray.size(); ++i) {
        qDebug() << "TopArray[" << i << "]:" << topArray[i];
    }

    qDebug() << "BottomArray size:" << bottomArray.size();
    for (int i = 0; i < bottomArray.size(); ++i) {
        qDebug() << "BottomArray[" << i << "]:" << bottomArray[i];
    }

    auto parseArray = [](const QJsonArray &array, const QString &label) {
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

            QJsonArray predArray = obj["predecessors"].toArray();
            for (const auto &p : predArray) {
                op.predecessors.append(p.toString());
            }

            qDebug() << label << ":" << op.id << op.jobId << op.startTime << op.duration << (op.startTime + op.duration)
                     << " machineId:" << op.machineId << " name:" << op.name << " cost:" << op.cost
                     << " predecessors:" << op.predecessors;

            result.append(op);
        }
        return result;
    };

    topOperations = parseArray(topArray, "Top");
    bottomOperations = parseArray(bottomArray, "Bottom");

    // Вычисляем maxFinish и uniqueJobCount
    maxFinishTop = 0;
    maxFinishBottom = 0;
    QSet<int> jobsTop, jobsBottom;

    topJobToMachines.clear();
    bottomJobToMachines.clear();
//    topBarGroups.clear();
//    bottomBarGroups.clear();
    topOpIdToGroup.clear();
    bottomOpIdToGroup.clear();

    for (const auto &topOp : topOperations) {
        maxFinishTop = std::max(maxFinishTop, topOp.startTime + topOp.duration);
        jobsTop.insert(topOp.jobId);
    }
    for (const auto &bottomOp : bottomOperations) {
        maxFinishBottom = std::max(maxFinishBottom, bottomOp.startTime + bottomOp.duration);
        jobsBottom.insert(bottomOp.jobId);
    }

    uniqueJobCountTop = jobsTop.size();
    uniqueJobCountBottom = jobsBottom.size();

    // Построение сопоставлений ID → связанные операции (верхние и нижние)
    for (const auto &op : topOperations) {
        QString key = op.id;
        QVector<OperationData> topGroup, bottomGroup;

        for (const auto &other : topOperations) {
            if (other.jobId == op.jobId && (other.startTime + other.duration) == (op.startTime + op.duration)) {
                topGroup.append(other);
            }
        }
        for (const auto &other : bottomOperations) {
            if (other.jobId == op.jobId && (other.startTime + other.duration) == (op.startTime + op.duration)) {
                bottomGroup.append(other);
            }
        }
        topOpIdToGroup[key] = qMakePair(topGroup, bottomGroup);
    }

    for (const auto &op : bottomOperations) {
        QString key = op.id;
        QVector<OperationData> topGroup;
        for (const auto &other : topOperations) {
            if (other.jobId == op.jobId && (other.startTime + other.duration) == (op.startTime + op.duration)) {
                topGroup.append(other);
            }
        }
        bottomOpIdToGroup[key] = topGroup;
    }

    qDebug() << "После loadFromJson:";
    qDebug() << "Top count:" << topOperations.size();
    qDebug() << "Bottom count:" << bottomOperations.size();
    qDebug() << "MaxFinishTop:" << maxFinishTop;
    qDebug() << "MaxFinishBottom:" << maxFinishBottom;
    qDebug() << "UniqueJobTop:" << uniqueJobCountTop;
    qDebug() << "UniqueJobBottom:" << uniqueJobCountBottom;
}


//void GanttDB::loadFromJson(const QString &filename) {
//    QFile file(filename);
//    if (!file.open(QIODevice::ReadOnly)) {
//        qWarning() << "Не удалось открыть JSON файл:" << filename;
//        return;
//    }
//    QByteArray data = file.readAll();
//    file.close();

//    QJsonDocument doc = QJsonDocument::fromJson(data);
//    QJsonObject root = doc.object();
//    QJsonArray topArray = root["top"].toArray();
//    QJsonArray bottomArray = root["bottom"].toArray();


//    auto parseArray = [](const QJsonArray &array, const QString &label) {
//        QVector<OperationData> result;
//        for (const auto &val : array) {
//            QJsonObject obj = val.toObject();
//            OperationData op;
//            op.id = obj["id"].toString();
//            op.machineId = obj["machineId"].toInt();
//            op.jobId = obj["jobId"].toInt();
//            op.startTime = obj["startTime"].toInt();
//            op.duration = obj["duration"].toInt();
//            op.setupTime = obj["setupTime"].toInt();
//            op.name = obj["name"].toString();
//            op.cost = obj["cost"].toInt();

//            QJsonArray predArray = obj["predecessors"].toArray();
//            for (const auto &p : predArray) {
//                op.predecessors.append(p.toString());
//            }

////            qDebug() << label << ":" << op.id << op.jobId << op.startTime << op.duration << (op.startTime + op.duration);

//            result.append(op);
//        }
//        return result;
//    };

//    topOperations = parseArray(topArray, "Top");
//    bottomOperations = parseArray(bottomArray, "Bottom");

//    // Вычисляем maxFinish и uniqueJobCount
//    maxFinishTop = 0;
//    maxFinishBottom = 0;
//    QSet<int> jobsTop, jobsBottom;

//    for (const auto &op : topOperations) {
//        maxFinishTop = std::max(maxFinishTop, op.startTime + op.duration);
//        jobsTop.insert(op.jobId);
//    }
//    for (const auto &op : bottomOperations) {
//        maxFinishBottom = std::max(maxFinishBottom, op.startTime + op.duration);
//        jobsBottom.insert(op.jobId);
//    }

//    uniqueJobCountTop = jobsTop.size();
//    uniqueJobCountBottom = jobsBottom.size();

//}



void GanttDB::writeToDatabase() {
    QSqlQuery query(db);

    // Очистка предыдущих записей
    query.exec("DROP TABLE IF EXISTS top_operations");
    query.exec("DROP TABLE IF EXISTS bottom_operations");

    // Создание таблиц
    query.exec(R"(
        CREATE TABLE top_operations (
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

    query.exec(R"(
        CREATE TABLE bottom_operations (
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

    auto insertOps = [&](const QVector<OperationData> &ops, const QString &table) {
        for (const auto &op : ops) {
            QString pred = op.predecessors.join(",");
            query.prepare(QString(R"(
                INSERT INTO %1 (id, machineId, jobId, startTime, duration, setupTime, name, cost, predecessors)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
            )").arg(table));

            query.addBindValue(op.id);
            query.addBindValue(op.machineId);
            query.addBindValue(op.jobId);
            query.addBindValue(op.startTime);
            query.addBindValue(op.duration);
            query.addBindValue(op.setupTime);
            query.addBindValue(op.name);
            query.addBindValue(op.cost);
            query.addBindValue(pred);

            if (!query.exec())
                qWarning() << "Insert into" << table << "failed:" << query.lastError().text();
        }
    };

    insertOps(topOperations, "top_operations");
    insertOps(bottomOperations, "bottom_operations");

    qDebug() << "Данные успешно записаны в базу.";
}




// GanttDB.cpp (добавить в реализацию)
void GanttDB::loadFromDatabase() {
    topOperations.clear();
    bottomOperations.clear();

    QSqlQuery query(db);

    // Загрузка верхнего графика
    query.exec("SELECT * FROM top_operations");
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
        op.predecessors = query.value("predecessors").toString().split(",", Qt::SkipEmptyParts);
        topOperations.append(op);
    }

    // Загрузка нижнего графика
    query.exec("SELECT * FROM bottom_operations");
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
        op.predecessors = query.value("predecessors").toString().split(",", Qt::SkipEmptyParts);
        bottomOperations.append(op);
    }

    // Подсчёт максимального времени и уникальных jobId
    maxFinishTop = 0;
    maxFinishBottom = 0;
    QSet<int> jobsTop, jobsBottom;

    for (const auto &op : topOperations) {
        maxFinishTop = std::max(maxFinishTop, op.startTime + op.duration);
        jobsTop.insert(op.jobId);
    }
    for (const auto &op : bottomOperations) {
        maxFinishBottom = std::max(maxFinishBottom, op.startTime + op.duration);
        jobsBottom.insert(op.jobId);
    }

    uniqueJobCountTop = jobsTop.size();
    uniqueJobCountBottom = jobsBottom.size();
}
