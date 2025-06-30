#pragma once
#include <QGraphicsRectItem>
#include <QPen>

class GanttBarItem : public QGraphicsRectItem {
public:
    GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
                               int timeUnit, int offsetX, int offsetY, int barHeight, QColor color, bool isHighlighted, int setupTime
                 ,int cost, const QString &innerLabelTex );


    void setHighlighted(bool on);
    QString getOpId() const;
    static void drawArrow(QGraphicsScene *scene, QPointF from, QPointF to) ;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;




protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void clearAllHighlightsExceptThis();



private:
    QPointF m_dragStart;
    QColor m_defaultColor;
    QColor m_assignedColor;  // сохранённый оригинальный цвет

    bool m_bisHighlighted = false;
    bool m_isManuallyHighlighted = false;

    QString m_opId;
    int m_jobId;
    int m_startTime;
    int m_isetupTime;
    int m_itimeUnit = 1;

    int m_duration;

    int m_imachineId;
    int m_setupTime;
    QString m_sname;
    int m_icost;

    QString m_innerLabel = nullptr;



};
