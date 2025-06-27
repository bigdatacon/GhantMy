#pragma once
#include <QGraphicsRectItem>
#include <QPen>

class GanttBarItem : public QGraphicsRectItem {
public:
    GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
                               int timeUnit, int offsetX, int offsetY, int barHeight, QColor color, bool isHighlighted);


    void setHighlighted(bool on);
    QString getOpId() const;




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
    int m_duration;

};
