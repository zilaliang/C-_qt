//
// Created by Administrator on 24-8-3.
//

#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H
#include <QPoint>
#include <QGraphicsView>

class MyGraphicView : public QGraphicsView {
Q_OBJECT

public:
    explicit MyGraphicView(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


signals:
    void keypress(QKeyEvent*event);
    void mouseClicked(QPoint point);
    void mouseDoubleClick(QPoint point);
    void mouseMovePoint(QPoint point);
    void mousePress(QPoint point);
    void mouseRelease(QPoint point);
};


#endif //MYGRAPHICVIEW_H