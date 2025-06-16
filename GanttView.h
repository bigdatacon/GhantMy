#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>

class GanttView : public QGraphicsView {
    Q_OBJECT
public:
    GanttView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene *m_scene;
};
