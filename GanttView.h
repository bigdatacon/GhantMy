
#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>
#include "GanttDB.h"
#include <QMap>

class GanttView : public QGraphicsView {
    Q_OBJECT
public:
    GanttView(const QString &title, const QVector<OperationData> &operations, GanttDB* db, int maxFinishTime, int uniqueJobCount);
    void printLinkedOperations(const QString &opId, int jobId, int m_startTime, int m_duration);
     void  clearHighlights();


     void startCostPulse();
     void stopCostPulse();

     void updateOperations(const QVector<OperationData> &ops) ;

     void setEditMode(bool on);
     void snapBarsToAxis() ;

     bool isEditMode() const;
     void showAddBarDialog();
     QString getTitle() const ;



protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;


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

    bool m_beditMode = false;
    QMap<int, qreal> m_axisCenters;



};


