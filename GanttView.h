
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
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene *m_scene;
    QString m_title;
    bool m_bInitialized = false;

    void populateScene();  // Добавляет оси, бары, подписи
};



