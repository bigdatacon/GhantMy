//#pragma once
//#include <QGraphicsView>
//#include <QGraphicsScene>

//class GanttView : public QGraphicsView {
//    Q_OBJECT
//public:
//    GanttView(QWidget *parent = nullptr);

//protected:
//    void wheelEvent(QWheelEvent *event) override;

//private:
//    QGraphicsScene *m_scene;
//};


#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>

class GanttView : public QGraphicsView {
    Q_OBJECT
public:
    GanttView(const QString &title = "", QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene *m_scene;
    QString m_title;
    void populateScene();  // Добавляет оси, бары, подписи
};
