
#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>
#include "GanttDB.h"
#include <QMap>

class GanttView : public QGraphicsView {
    Q_OBJECT
public:
    GanttView(const QString &title = "", QWidget *parent = nullptr);
//    GanttView(const QString &title, const QVector<OperationData> &operations, GanttDB* db, QWidget *parent = nullptr);
    GanttView(const QString &title, const QVector<OperationData> &operations, GanttDB* db, QWidget *parent = nullptr);
    GanttView(const QString &title, const QVector<OperationData> &operations, GanttDB* db, int maxFinishTime, int uniqueJobCount);





protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene *m_scene;
    QString m_title;
    bool m_bInitialized = false;
    QVector<OperationData> m_operations;
    QMap<int, QColor> m_jobColorMap;

    int m_maxFinishTime = 0;
    int m_uniqueJobCount = 0;

    GanttDB* m_pDB = nullptr;

    void populateScene();  // Добавляет оси, бары, подписи
    void generateJobColorMap();
};


