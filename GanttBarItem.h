//#pragma once
//#include <QGraphicsRectItem>
//#include <QPen>

//class GanttBarItem : public QGraphicsRectItem {
//public:
//    GanttBarItem(int machineId, int jobId, int startTime, int duration, int timeUnit, int offsetX, int offsetY);

//    void setHighlighted(bool on);

//protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

//private:
//    QPointF m_dragStart;
//    QColor m_defaultColor;
//};

//struct OperationData {
//    QString id;
//    int machineId;
//    int jobId;
//    int startTime;
//    int duration;
//    int setupTime;
//    QString name;
//    int cost;
//    QStringList predecessors;
//};

#pragma once
#include <QGraphicsRectItem>
#include <QPen>

class GanttBarItem : public QGraphicsRectItem {
public:
    GanttBarItem(int machineId, int jobId, int startTime, int duration, int timeUnit, int offsetX, int offsetY);
    GanttBarItem(int machineId, int jobId, int startTime, int duration, int timeUnit, int offsetX, int offsetY, int passedBarHeight);

    GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration, int timeUnit, int offsetX, int offsetY, int passedBarHeight);
    GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
                               int timeUnit, int offsetX, int offsetY, int barHeight, QColor color);


    void setHighlighted(bool on);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF m_dragStart;
    QColor m_defaultColor;
};
