#pragma once
#include <QGraphicsRectItem>
#include <QPen>

class GanttBarItem : public QGraphicsRectItem {
public:

    GanttBarItem(QString id, int machineId, int jobId, int startTime, int duration,
                               int timeUnit, int offsetX, int offsetY, int barHeight, QColor color);


    void setHighlighted(bool highlight);

    QString getOpId() const ;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool isHighlighted() const ;



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF m_dragStart;
    QColor m_defaultColor;
    QColor m_assignedColor;  // сохранённый оригинальный цвет

    bool m_highlighted = false;


    QString m_opId;

};
