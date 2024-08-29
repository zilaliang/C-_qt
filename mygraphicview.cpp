//
// Created by Administrator on 24-8-3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyGraphicView.h" resolved

#include "Header/mygraphicview.h"
#include <QMouseEvent>
#include<QtDebug>
#include<QGraphicsItem>
#include"Header/mainwindow.h"
void MyGraphicView::keyPressEvent(QKeyEvent *event) {
    emit keypress(event);
    //控制权还给基类
    QGraphicsView::keyPressEvent(event);
}

void MyGraphicView::mouseDoubleClickEvent(QMouseEvent *event) {
    if(event->button()==Qt::LeftButton) {
        QPoint point=event->pos();
        emit mouseDoubleClick(point);
    }
    //执行基类
    QGraphicsView::mouseDoubleClickEvent(event);
}

void MyGraphicView::mousePressEvent(QMouseEvent *event) {
    if(event->button()==Qt::LeftButton) {
        QPoint point=event->pos();
        emit mouseClicked(point);
        emit mousePress(point);
    }

    QGraphicsView::mousePressEvent(event);
}

void MyGraphicView::mouseMoveEvent(QMouseEvent *event) {
    QPoint point=event->pos();
    emit mouseMovePoint(point);
    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicView::mouseReleaseEvent(QMouseEvent *event) {
    QPoint point=event->pos();
    emit mouseRelease(point);
    QGraphicsView::mouseReleaseEvent(event);
}


MyGraphicView::MyGraphicView(QWidget *parent):QGraphicsView(parent){

}