#include "Header/mainwindow.h"
#include <qbrush.h>
#include<QTime>
#include<QGraphicsRectItem>
#include "Form/ui_MainWindow.h"
#include<Qpen>
#include<QInputDialog>
#include<QColorDialog>
#include<QKeyEvent>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <QFileDialog>
#include<qDebug>
#include<QPixmap>
#include<QSpinBox>
#include<QSqlDatabase>
#include<QGraphicsRectItem>
#include<QSqlQuery>
#include<QColor>
#include <QSqlError>
#include<QTextCodec>
#include<QBuffer>
#include<QDataStream>
#include<Qlist>
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :

    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QToolBar*toolbar=findChild<QToolBar*>("toolBar_6");
    if(toolbar!=NULL) {
        toolbar->setVisible(false);
    }

    setCentralWidget(ui->gView);
    labItemInfo=new QLabel("ItemInfo:");
    labItemInfo->setMinimumWidth(150);
    //新创建的一个scene对象
    scene=new QGraphicsScene(-300,-100,600,200);
    ui->gView->setScene(scene);
    ui->gView->setCursor(Qt::CrossCursor);
    ui->gView->setMouseTracking(true);
    ui->gView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->statusbar->addWidget(labItemInfo);

    labViewCord=new QLabel("View坐标:");
    labViewCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labViewCord);

    labSceneCord=new QLabel("Scene坐标:");
    labSceneCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labSceneCord);

    labItemCord=new QLabel("Item坐标:");
    labItemCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labItemCord);

    connect(ui->gView,SIGNAL(mouseMovePoint(QPoint)),
        SLOT(on_mouseMovePoint(QPoint)));
    connect(ui->gView,SIGNAL(keypress(QKeyEvent*)),
        this,SLOT(on_keypress(QKeyEvent*)));
    connect(ui->gView,SIGNAL(mouseClicked(QPoint)),
        this,SLOT(on_mouseClicked(QPoint)));
    connect(ui->gView,SIGNAL(mouseDoubleClick(QPoint)),
        this,SLOT(on_mouseDoubleClick(QPoint)));
    connect(ui->gView,SIGNAL(mousePress(QPoint)),
        this,SLOT(on_mousePress(QPoint)));
    connect(ui->gView,SIGNAL(mouseRelease(QPoint)),
        this,SLOT(on_mouseRelease(QPoint)));

    qsrand(QTime::currentTime().second());



}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::backPen() {
    QToolBar*toolbar=findChild<QToolBar*>("toolBar_6");
    if(toolbar!=NULL) {
        toolbar->setVisible(false);
    }
    int cnt =scene->items().count();
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->items().at(i);
        item->setFlag(QGraphicsItem::ItemIsMovable,true);
    }
    penOn=false;
}

void MainWindow::updateCursorShape(const QPointF &pointScene) {
    if (!rectItem) {
        setCursor(Qt::ArrowCursor);  // 如果没有选中矩形，恢复为默认光标
        return;
    }

    QPointF pointItem = rectItem->mapFromScene(pointScene);
    QRectF rect = rectItem->boundingRect();
    const int margin = 5;  // 鼠标接近边缘或角落的判断范围

    // 判断鼠标位置来设置光标形状
    if (abs(pointItem.x() - rect.left()) < margin && abs(pointItem.y() - rect.top()) < margin) {
        // 左上角 - 对角调整
        ui->gView->setCursor(Qt::SizeFDiagCursor);
    } else if (abs(pointItem.x() - rect.right()) < margin && abs(pointItem.y() - rect.top()) < margin) {
        // 右上角 - 反对角调整
        ui->gView->setCursor(Qt::SizeBDiagCursor);
    } else if (abs(pointItem.x() - rect.left()) < margin && abs(pointItem.y() - rect.bottom()) < margin) {
        // 左下角 - 反对角调整
        ui->gView->setCursor(Qt::SizeBDiagCursor);
    } else if (abs(pointItem.x() - rect.right()) < margin && abs(pointItem.y() - rect.bottom()) < margin) {
        // 右下角 - 对角调整
        ui->gView->setCursor(Qt::SizeFDiagCursor);
    } else if (abs(pointItem.x() - rect.left()) < margin) {
        // 左边 - 水平调整
        ui->gView->setCursor(Qt::SizeHorCursor);
    } else if (abs(pointItem.x() - rect.right()) < margin) {
        // 右边 - 水平调整
        ui->gView->setCursor(Qt::SizeHorCursor);
    } else if (abs(pointItem.y() - rect.top()) < margin) {
        // 上边 - 垂直调整
        ui->gView->setCursor(Qt::SizeVerCursor);
    } else if (abs(pointItem.y() - rect.bottom()) < margin) {
        // 下边 - 垂直调整
        ui->gView->setCursor(Qt::SizeVerCursor);
    } else {
        setCursor(Qt::ArrowCursor);      // 如果不在调整区域，恢复为默认光标
    }
}

void MainWindow::resizeRect(const QPointF &pointScene) {
    QRectF rect = initialRect;  // 获取矩形的初始尺寸
    QPointF delta = pointScene - lastPoint;  // 计算鼠标移动的差值
    int width;
    int hight;
    // 根据调整方向更新矩形的尺寸和位置

    switch (resizeDirection) {
        case TopLeft:
            rect.setX(rect.x()+delta.x());
            rect.setY(delta.y()+rect.y());
        break;
        case TopRight://修改y和宽度
            rect.setY(delta.y()+rect.y());
            width = initialRect.width() + (pointScene.x() - lastPoint.x());
            rect.setWidth(width);
        break;
        case BottomLeft:
            rect.setX(delta.x()+rect.x());
            hight=initialRect.height() + delta.y();
            rect.setHeight(hight);
        break;
        case BottomRight:
            width = initialRect.width() + (pointScene.x() - lastPoint.x());
            rect.setWidth(width);
            hight=initialRect.height() + delta.y();
            rect.setHeight(hight);
        break;
        case Left:
            rect.setX(delta.x()+rect.x());
        break;
        case Right:
            width = initialRect.width() + (pointScene.x() - lastPoint.x());
            rect.setWidth(width);
        break;
        case Top:
            rect.setY(delta.y()+rect.y());
        break;
        case Bottom:
            hight=initialRect.height() + delta.y();
            rect.setHeight(hight);
        break;
        default:
            break;
    }

    // 更新矩形的大小
    rectItem->setRect(rect);
}

void MainWindow::saveItemToDatabase(QGraphicsRectItem *rectItem) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("myDatabase.db");
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    if (!db.open()) {
        qDebug() << "打开数据库失败";
        return;
    }
    db.open();
    QSqlQuery query;
    //如果表rect_items不存在

    bool sueccse=query.exec("CREATE TABLE rect_items (id INTEGER PRIMARY KEY AUTOINCREMENT, position_x REAL, position_y REAL, rect_x REAL ,rect_y REAL,width REAL, height REAL, color TEXT)");
    if(sueccse)
        qDebug()<<"成功创建表";
    else
        qDebug()<<"创表失败";
    QPointF scenePos = rectItem->scenePos();

    query.prepare("INSERT INTO rect_items(position_x, position_y, rect_x,rect_y,width, height, color) VALUES(:posx, :posy,:rect_x,:rect_y ,:width, :height, :color)");

    query.bindValue(":posx", scenePos.x());
    query.bindValue(":posy", scenePos.y());
    query.bindValue(":rect_x", rectItem->rect().x());
    query.bindValue(":rect_y", rectItem->rect().y());
    query.bindValue(":width", rectItem->rect().width());
    query.bindValue(":height", rectItem->rect().height());
    query.bindValue(":color", rectItem->brush().color());
    //清空数据表
    //query.exec("DELETE FROM rect_items");
    if (!query.exec()) {
        qDebug() << "执行查询失败:" << query.lastError().text();
    } else {
        qDebug() << "数据插入成功";
    }
    db.close();
}

void MainWindow::savePixmapItemToDatabase(QGraphicsPixmapItem *pixmapItem) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("myDatabase.db");
    if (!db.open()) {
        qDebug() << "打开数据库失败";
        return;
    }
    QSqlQuery query(db);
    // 如果表 pixmap_items 不存在，则创建它
    bool success = query.exec("CREATE TABLE IF NOT EXISTS pixmap_items (id INTEGER PRIMARY KEY AUTOINCREMENT, image_data BLOB)");
    if (success)
        qDebug() << "创表成功";
    else
        qDebug() << "创表失败";

    // 将 QPixmap 转换为 QByteArray
    QPixmap pixmap = pixmapItem->pixmap();
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPEG");

    query.prepare("INSERT INTO pixmap_items (image_data) VALUES (:imageData)");
    query.bindValue(":imageData", imageData);

    if (!query.exec()) {
        qDebug() << "执行查询失败:" << query.lastError().text();
    } else {
        qDebug() << "插入成功";
    }

    db.close();
}
void MainWindow::readItemFromDatabase() {
    //将所有数据从数据库中读取出来赋值给ReadRectItem
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("myDatabase.db");
    if (!db.open()) {
        qDebug() << "打开数据库失败";
        return;
    }

    QSqlQuery query;
    query.exec("SELECT * FROM rect_items");
    while(query.next()) {
        QPointF position(query.value(1).toFloat(),query.value(2).toFloat());
        QRectF rect(
        query.value(3).toFloat(), // x 坐标
        query.value(4).toFloat(), // y 坐标
        query.value(5).toFloat(), // 宽度
        query.value(6).toFloat()  // 高度
        );
        QColor color(query.value(7).toString());
        QGraphicsRectItem *ReadRectItem=new QGraphicsRectItem(rect);
        qDebug()<<position;
        ReadRectItem->setPos(position);
        ReadRectItem->setBrush(QBrush(color));
        ReadRectItem->setFlags(QGraphicsRectItem::ItemIsFocusable|
        QGraphicsRectItem::ItemIsSelectable|
        QGraphicsRectItem::ItemIsMovable);
        scene->addItem(ReadRectItem);
        ui->gView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
    db.close();
}
void MainWindow::addImagesFromDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("myDatabase.db");
    if (!db.open()) {
        qDebug() << "打开数据库失败";
        return;
    }
    QSqlQuery query(db);
    if (!query.exec("SELECT image_data FROM pixmap_items")) {
        qDebug() << "执行查询失败:" << query.lastError().text();
        db.close();
        return;
    }
    while (query.next()) {
        QByteArray imageData = query.value(0).toByteArray();
        QPixmap pixmap;
        pixmap.loadFromData(imageData);
        if (!pixmap.isNull()) {
            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
            pixmapItem->setFlags(QGraphicsItem::ItemIsFocusable|
                QGraphicsItem::ItemIsSelectable|
                QGraphicsItem::ItemIsMovable);
            pixmapItem->setPos(-560,-380);
            pixmapItem->setScale(0.3);
            //表面卡片的ID和描述
            pixmapItem->setData(ItemId,++seqNum);
            pixmapItem->setData(ItemDescription,QVariant("图片"));
            scene->addItem(pixmapItem);
        }
    }

    db.close();
}

void MainWindow::serializeObject(const QList<QGraphicsItem*>&items, const QString& filename) {
    QString filePath = "D:/ALLCODE/QTC++/text/untitled9/source/" + filename;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        stream<<items.size();
        for(QGraphicsItem*item:items) {
            stream<<item->type();
            stream<<item->pos();
            if(auto*rectItem=qgraphicsitem_cast<QGraphicsRectItem*>(item)) {
                stream<<rectItem->rect();
                stream<<rectItem->brush();
            }
            else if (auto* pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item)) {
                 // 保存图片路径
                stream<<pixmapItem->data(Qt::UserRole).toString();
                // 保存其他图片特有的数据
                QPixmap pixmap = pixmapItem->pixmap();
                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QIODevice::WriteOnly);
                pixmap.save(&buffer, "JPEG");
                stream << byteArray;
            }
        }
        file.close();
    }
}


void MainWindow::readItemFromDatFile(const QString& filename) {
    QString filePath = "D:/ALLCODE/QTC++/text/untitled9/source\\" + filename;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        int numItems;
        stream >> numItems;
        qDebug()<<"存储的数据的个数为"<<numItems;
        for(int i=0;i<numItems;i++) {
            int itemType;
            QPointF position;
            QRectF rect;
            QBrush brush;
            stream >> itemType;
            stream >> position;
            if (itemType == QGraphicsPixmapItem::Type) {
                QString imagePath;
                stream >> imagePath;
                // 处理其他类型的图形项
                QByteArray imageData;
                stream >> imageData;
                QPixmap pixmap;
                pixmap.loadFromData(imageData, "JPEG");

                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
                pixmapItem->setData(Qt::UserRole, imagePath);
                pixmapItem->setPos(position);
                pixmapItem->setFlags(QGraphicsItem::ItemIsFocusable |
                                      QGraphicsItem::ItemIsSelectable |
                                      QGraphicsItem::ItemIsMovable);
                pixmapItem->setScale(0.3);
                qDebug() << "读取的是图片";
                //表面卡片的ID和描述
                pixmapItem->setData(ItemId,++seqNum);
                pixmapItem->setData(ItemDescription,QVariant("图片"));
                scene->addItem(pixmapItem);
                // 设置QGraphicsPixmapItem自适应QGraphicsView的大小
                ui->gView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
            }
            else if (itemType == QGraphicsRectItem::Type) {
                stream >> rect;
                stream >> brush;
                QGraphicsRectItem *ReadRectItem = new QGraphicsRectItem(rect);
                ReadRectItem->setPos(position);
                ReadRectItem->setBrush(brush);
                ReadRectItem->setFlags(QGraphicsItem::ItemIsFocusable |
                                       QGraphicsItem::ItemIsSelectable |
                                       QGraphicsItem::ItemIsMovable);
                scene->addItem(ReadRectItem);
            }
        }
        file.close();
    } else {
        qDebug() << "无法打开文件：" << file.errorString();
    }
}



void MainWindow::on_keypress(QKeyEvent *event) {
    int cnt=scene->selectedItems().count();
    if(cnt==0)return;
    switch(event->key()) {
        case Qt::Key_Delete: {
            for(int i=0;i<cnt;i++) {
                QGraphicsItem *item=scene->selectedItems().at(0);
                scene->removeItem(item);
            }
            break;
        }
        case Qt::Key_Up: {
            for(int i=0;i<cnt;i++) {
                QGraphicsItem*item=scene->selectedItems().at(i);
                item->setY(item->y()-1);
            }
            break;
        }
        case Qt::Key_Down: {
            for(int i=0;i<cnt;i++) {
                QGraphicsItem*item=scene->selectedItems().at(i);
                item->setY(item->y()+1);
            }
            break;
        }
        case Qt::Key_Left: {
            for(int i=0;i<cnt;i++) {
                QGraphicsItem*item=scene->selectedItems().at(i);
                item->setX(item->x()-1);
            }
            break;
        }
        case Qt::Key_Right: {
            for(int i=0;i<cnt;i++) {
                QGraphicsItem*item=scene->selectedItems().at(i);
                item->setX(item->x()+1);
            }
            break;
        }
        //back键删除
        case Qt::Key_Backspace: {
            for(int i=0;i<cnt;i++) {
                QGraphicsItem*item=scene->selectedItems().at(i);
                scene->removeItem(item);
            }
            break;
        }
    }

}

void MainWindow::on_mouseClicked(QPoint point) {
    // 将点击的点从视图坐标转换为场景坐标
    QPointF pointScene = ui->gView->mapToScene(point);

    // 声明一个指向QGraphicsItem的指针
    QGraphicsItem* item = NULL;
    // 获取点击位置的QGraphicsItem对象
    item = scene->itemAt(pointScene, ui->gView->transform());
    // 如果获取到了QGraphicsItem对象
    if(item != NULL) {
        // 设置labItemInfo标签的文本，显示QGraphicsItem的ItemId和ItemDescription数据
        labItemInfo->setText("ItemInfo:" + item->data(ItemId).toString() +
            "," + item->data(ItemDescription).toString());

        QPointF pointItem=item->mapFromScene(pointScene);
        labItemCord->setText(QString::asprintf("Item坐标:%.0f,%.0f",pointItem.x(),pointItem.y()));
    }
}

void MainWindow::on_mouseDoubleClick(QPoint point) {
    backPen();//取消画笔

    QPointF pointScene = ui->gView->mapToScene(point);
    QGraphicsItem* item = NULL;
    item = scene->itemAt(pointScene, ui->gView->transform());

    if(item == NULL) return;
    switch(item->type()) {
        case QGraphicsRectItem::Type: {
            QGraphicsRectItem*theItem;
            theItem=qgraphicsitem_cast<QGraphicsRectItem*>(item);
            QColor color=theItem->brush().color();
            color=QColorDialog::getColor(color,NULL,"选择颜色");
            theItem->setBrush(QBrush(color));
            break;
        }
        case QGraphicsEllipseItem::Type: {
            QGraphicsEllipseItem*theItem;
            theItem=qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
            QColor color=theItem->brush().color();
            color=QColorDialog::getColor(color,NULL,"选择颜色");
            theItem->setBrush(QBrush(color));
            break;
        }
        case QGraphicsPolygonItem::Type: {
            QGraphicsPolygonItem*theItem;
            theItem=qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
            QColor color=theItem->brush().color();
            color=QColorDialog::getColor(color,NULL,"选择颜色");
            theItem->setBrush(QBrush(color));
            break;
        }
        case QGraphicsLineItem::Type: {
            QGraphicsLineItem*theItem;
            theItem=qgraphicsitem_cast<QGraphicsLineItem*>(item);
            QColor color=theItem->pen().color();
            color=QColorDialog::getColor(color,NULL,"选择颜色");
            theItem->setPen(QPen(color));
            break;
        }
        case QGraphicsTextItem::Type: {
            QGraphicsTextItem*theItem;
            theItem=qgraphicsitem_cast<QGraphicsTextItem*>(item);
            QColor color=theItem->defaultTextColor();
            color=QColorDialog::getColor(color,NULL,"选择颜色");
            theItem->setDefaultTextColor(color);
            break;
        }
    }
}

void MainWindow::on_mouseMovePoint(QPoint point) {
    labViewCord->setText(QString::asprintf("View坐标:%d,%d",point.x(),point.y()));
    //转化为scene坐标
    QPointF pointScene = ui->gView->mapToScene(point);
    labSceneCord->setText(QString::asprintf("Scene坐标:%f,%f",pointScene.x(),pointScene.y()));
    // 获取鼠标所在位置的QGraphicsItem对象
    QGraphicsItem* item = scene->itemAt(pointScene, ui->gView->transform());

    if(IsPen==true&&penOn==true) {
        //在每个位置留下点
        if(isPenOriginal) {
            pen1.setColor(Qt::black);
            pen1.setWidth(3);
        }
        QGraphicsLineItem*line=new QGraphicsLineItem(lastPoint.x(),lastPoint.y(),pointScene.x(),pointScene.y());
        lastPoint=pointScene;
        line->setPen(pen1);
        scene->addItem(line);
    }
    //新方法修改矩形大小
    // 更新鼠标形状
    updateCursorShape(pointScene);
    if (resizing && rectItem) {
        resizeRect(pointScene);  // 调整矩形大小
    }
    // if(isMove==true&&originalItem!=nullptr) {
    //     //得到鼠标移动的变化值
    //     QPointF delta=pointScene-lastPoint;
    //     QRectF newRect=originalItem->boundingRect();
    //     //根据鼠标移动delta的值来改变图形的高度和宽度
    //     if(resizeDirection==Qt::SizeFDiagCursor) {
    //         newRect.setWidth(newRect.width()+delta.x());
    //         newRect.setHeight(newRect.height()+delta.y());
    //     }
    //     originalItem->setTransform(QTransform::fromScale(newRect.width()/originalItem->boundingRect().height(),
    //         newRect.height()/originalItem->boundingRect().width()));
    // }
    if(item == NULL) return;
}

void MainWindow::on_mousePress(QPoint point) {
    lastPoint=ui->gView->mapToScene(point);//设置起始点
    if(penOn==true) {
        IsPen=true;
    }
    QPointF pointScene = ui->gView->mapToScene(point);
    //如果点击的是一个QGraphicsRectItem对象
    QGraphicsItem* item = scene->itemAt(pointScene, ui->gView->transform());
    if(item == NULL) return;
    if(IsPen==true&&penOn==true) {
        int cnt =scene->selectedItems().count();
        for(int i=0;i<cnt;i++) {
            QGraphicsItem*item=scene->selectedItems().at(i);
            item->setFlag(QGraphicsItem::ItemIsMovable,false);
        }
    }
    // if(!scene->selectedItems().isEmpty()) {
    //     rectItem=scene->selectedItems().at(0);
    //     int height=originalItem->boundingRect().height();
    //     int width=originalItem->boundingRect().width();
    //     QPointF pointItem=originalItem->mapFromScene(pointScene);
    //     //根据点击的位置来修改鼠标样式
    //     if(pointItem.x()<=0.5*width&&pointItem.x()>0.5*width-10&&pointItem.y()<0.5*height&&pointItem.y()>0.5*height-10) {
    //         ui->gView->setCursor(Qt::SizeFDiagCursor);
    //         //并且将item设置为不可移动
    //         originalItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    //         resizeDirection=Qt::SizeFDiagCursor;
    //         qDebug()<<"SizeFDiagCursor";
    //     }
    //     qDebug()<<pointItem<<width<<height;
    //     isMove=true;
    // }
    //如果点击的是一个QGraphicsRectItem对象
    if(item->type()==QGraphicsRectItem::Type) {
        rectItem=qgraphicsitem_cast<QGraphicsRectItem*>(item);
    }

    if(rectItem!=nullptr) {
        initialRect = rectItem->rect();
        //qDebug()<<rectItem->rect().height()<<rectItem->rect().width();
        QRectF rect = rectItem->rect();
        // 判断点击位置来确定调整方向
        QPointF pointItem = rectItem->mapFromScene(pointScene);
        const int margin = 10;  // 鼠标接近边缘的判断范围
        if (abs(pointItem.x() - rect.left()) < margin && abs(pointItem.y() - rect.top()) < margin) {
            resizeDirection = TopLeft;
        } else if (abs(pointItem.x() - rect.right()) < margin && abs(pointItem.y() - rect.top()) < margin) {
            resizeDirection = TopRight;
        } else if (abs(pointItem.x() - rect.left()) < margin && abs(pointItem.y() - rect.bottom()) < margin) {
            resizeDirection = BottomLeft;
        } else if (abs(pointItem.x() - rect.right()) < margin && abs(pointItem.y() - rect.bottom()) < margin) {
            resizeDirection = BottomRight;
        } else if (abs(pointItem.x() - rect.left()) < margin) {
            resizeDirection = Left;
        } else if (abs(pointItem.x() - rect.right()) < margin) {
            resizeDirection = Right;
        } else if (abs(pointItem.y() - rect.top()) < margin) {
            resizeDirection = Top;
        } else if (abs(pointItem.y() - rect.bottom()) < margin) {
            resizeDirection = Bottom;
        } else {
            resizeDirection = None;
        }
        if (resizeDirection != None) {
            resizing = true;
            isMove=true;
            rectItem->setFlag(QGraphicsItem::ItemIsMovable,false);
        }
    }

}

void MainWindow::on_mouseRelease(QPoint point) {
    // if(rectItem!=nullptr) {
    //     rectItem->setFlag(QGraphicsItem::ItemIsMovable,true);
    //     ui->gView->setCursor(Qt::CrossCursor);
    // }
    IsPen=false;
    resizing = false;
    ui->gView->setCursor(Qt::ArrowCursor);
    if(isMove==true) {
        rectItem->setFlag(QGraphicsItem::ItemIsMovable,true);
        isMove=false;
    }
    // isMove=false;
    // resizeDirection = Qt::ArrowCursor; // 重置方向
}

void MainWindow::on_actItem_Rect_triggered() {
    auto*item=new QGraphicsRectItem(-50,-30,100,60);
    item->setFlags(QGraphicsRectItem::ItemIsFocusable|
        QGraphicsRectItem::ItemIsSelectable|
        QGraphicsRectItem::ItemIsMovable);
    //item->setBrush(QBrush(Qt::yellow));
    item->setPos(-100+qrand()%200,-60+qrand()%120);
//表面卡片的ID和描述
    item->setData(ItemId,++seqNum);
    item->setData(ItemDescription,"矩形");
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);

    // //测试是否能够将矩形添加到数据库
    // rectItem = new QGraphicsRectItem(0, 0, 100, 100); // x, y, width, height
    // rectItem->setBrush(QColor("blue")); // 设置矩形的填充颜色
    // // 将矩形对象添加到场景中
    // scene->addItem(rectItem);
}

void MainWindow::on_actItem_Ellipse_triggered() {
    auto*item=new QGraphicsEllipseItem(-50,-30,100,60);
    item->setFlags(QGraphicsItem::ItemIsFocusable|
        QGraphicsItem::ItemIsSelectable|
        QGraphicsItem::ItemIsMovable);
    item->setBrush(QBrush(Qt::gray));
    item->setPos(-100+qrand()%200,-60+qrand()%120);
    //表面卡片的ID和描述
    item->setData(ItemId,++seqNum);
    item->setData(ItemDescription,QVariant("椭圆形"));
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void MainWindow::on_actItem_Circle_triggered() {
    auto*item=new QGraphicsEllipseItem(-50,-30,100,100);

    item->setFlags(QGraphicsItem::ItemIsFocusable|
        QGraphicsItem::ItemIsSelectable|
        QGraphicsItem::ItemIsMovable);
    item->setBrush(QBrush(Qt::blue));
    item->setPos(-100+qrand()%200,-60+qrand()%120);
    //表面卡片的ID和描述
    item->setData(ItemId,++seqNum);
    item->setData(ItemDescription,QVariant("圆形"));
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void MainWindow::on_actItem_Line_triggered() {
    auto*item=new QGraphicsLineItem(-100,0,100,0);
    item->setFlags(QGraphicsItem::ItemIsFocusable|
        QGraphicsItem::ItemIsSelectable|
        QGraphicsItem::ItemIsMovable);
    QPen pen(Qt::black);
    pen.setWidth(3);
    item->setPen(pen);
    item->setPos(-100+qrand()%200,-60+qrand()%120);
    //表面卡片的ID和描述
    item->setData(ItemId,++seqNum);
    item->setData(ItemDescription,QVariant("直线"));
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void MainWindow::on_actItem_Polygon_triggered() {
    auto*item=new QGraphicsPolygonItem;
    QPolygonF points;
    points.append(QPointF(-40,-40));
    points.append(QPointF(40,-40));
    points.append(QPointF(60,40));
    points.append(QPointF(-60,40));

    item->setPolygon(points);
    item->setFlags(QGraphicsItem::ItemIsFocusable|
        QGraphicsItem::ItemIsSelectable|
        QGraphicsItem::ItemIsMovable);
    item->setBrush(QBrush(Qt::blue));
    item->setPos(-100+qrand()%200,-60+qrand()%120);
    //表面卡片的ID和描述
    item->setData(ItemId,++seqNum);
    item->setData(ItemDescription,QVariant("梯形"));
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void MainWindow::on_actItem_Triangle_triggered() {
    auto*item=new QGraphicsPolygonItem;
    QPolygonF points;
    points.append(QPointF(0,-40));
    points.append(QPointF(60,40));
    points.append(QPointF(-60,40));
    item->setPolygon(points);
    item->setFlags(QGraphicsItem::ItemIsFocusable|
        QGraphicsItem::ItemIsSelectable|
        QGraphicsItem::ItemIsMovable);
    item->setBrush(QBrush(Qt::red));
    item->setPos(-100+qrand()%200,-60+qrand()%120);
    //表面卡片的ID和描述
    item->setData(ItemId,++seqNum);
    item->setData(ItemDescription,QVariant("三角形"));
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void MainWindow::on_actItem_Text_triggered() {
    QString str = QInputDialog::getText(this, "输入对话框", "请输入:");
    auto*item=new QGraphicsTextItem(str);
    item->setFlags(QGraphicsItem::ItemIsFocusable|
        QGraphicsItem::ItemIsSelectable|
        QGraphicsItem::ItemIsMovable);
    QFont font;
    font.setPointSize(20);
    item->setDefaultTextColor(Qt::green);

    item->setFont(font);
    item->setPos(-100+qrand()%200,-60+qrand()%120);
    //表面卡片的ID和描述
    item->setData(ItemId,++seqNum);
    item->setData(ItemDescription,QVariant("文字"));
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void MainWindow::on_act_Paint_triggered() {
    penOn=true;
    isPenOriginal=true;
    int cnt =scene->items().count();
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->items().at(i);
        item->setFlag(QGraphicsItem::ItemIsMovable,false);
    }
    QToolBar*toolbar=findChild<QToolBar*>("toolBar_6");
    if(toolbar!=NULL) {
        toolbar->setVisible(true);
    }
}

void MainWindow::on_actZoomIn_triggered() {
    int cnt =scene->selectedItems().count();
    if(cnt==0) {
        ui->gView->scale(1.1,1.1);
        return;
    }
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(i);
        item->setScale(0.1+item->scale());
    }
}

void MainWindow::on_actZoomOut_triggered() {
    int cnt =scene->selectedItems().count();
    if(cnt==0) {
        ui->gView->scale(0.9,0.9);
        return;
    }
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(i);
        item->setScale(-0.1+item->scale());
    }
}

void MainWindow::on_actRestore_triggered() {
    int cnt =scene->selectedItems().count();
    if(cnt==0) {
        ui->gView->resetTransform();
        return;
    }
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(i);
        item->setScale(1);
    }
}

void MainWindow::on_actRotateLeft_triggered() {
    int cnt =scene->selectedItems().count();
    if(cnt==0) {
        ui->gView->rotate(-45);
        return;
    }
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(i);
        item->setRotation(item->rotation()-45);
    }
}

void MainWindow::on_actRotateRight_triggered() {
    int cnt =scene->selectedItems().count();
    if(cnt==0) {
        ui->gView->rotate(+45);
        return;
    }
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(i);
        item->setRotation(item->rotation()+45);
    }
}

void MainWindow::on_actEdit_Front_triggered() {
    int cnt =scene->selectedItems().count();
    if(cnt==0) {
        return;
    }
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(i);
        item->setZValue(item->zValue()+1);
    }
}

void MainWindow::on_actEdit_Back_triggered() {
    int cnt =scene->selectedItems().count();
    if(cnt==0) {
        return;
    }
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(i);
        item->setZValue(item->zValue()-1);
    }
}

void MainWindow::on_actGroup_triggered() {
    int cnt =scene->selectedItems().count();
    if(!(cnt>1)) {
        return;
    }
    QGraphicsItemGroup *group=new QGraphicsItemGroup;
    scene->addItem(group);
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(0);
        item->setSelected(false);
        group->addToGroup(item);
    }
    group->setFlags(QGraphicsItem::ItemIsFocusable|
                QGraphicsItem::ItemIsSelectable|
                QGraphicsItem::ItemIsMovable);
    //scene->clearSelection();
    group->setSelected(true);
}

void MainWindow::on_actGroupBreak_triggered() {
    int cnt =scene->selectedItems().count();
    if(cnt==0) {
        return;
    }
    QList<QGraphicsItemGroup*>list;
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(i);
        if(item->type()==QGraphicsItemGroup::Type) {
            QGraphicsItemGroup*group=qgraphicsitem_cast<QGraphicsItemGroup*>(item);
            list.append(group);
        }
    }
    foreach(auto gItem,list) {
        scene->destroyItemGroup(gItem);
    }
}

void MainWindow::on_actEdit_Delete_triggered() {
    int cnt=scene->selectedItems().count();
    if(cnt==0)
        return;
    for(int i=0;i<cnt;i++) {
        QGraphicsItem*item=scene->selectedItems().at(0);
        scene->removeItem(item);
    }
}

void MainWindow::on_actOpen_Photo_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "打开图片", ".", "Image files (*.jpeg)");

    if (!filename.isEmpty()) {
        // //QString转化为String
        srcImage=imread(filename.toStdString());
        // //转化为QImage 这个是第二章方法一样的也能转化为Item对象来进行处理
        // cvtColor(srcImage,srcImage,COLOR_BGR2RGB);
        // QImage displayImage=QImage(srcImage.data,srcImage.cols,srcImage.rows,srcImage.cols*srcImage.channels(),QImage::Format_RGB888);
        // QImage image=imageCenter(displayImage,ui->gView);
        // //
        // ui->setPixmap(QPixmap::fromImage(image));
        //初始化QPixmap对象

        QPixmap pixmap(filename);
        QGraphicsPixmapItem*item=new QGraphicsPixmapItem(pixmap);
        item->setFlags(QGraphicsItem::ItemIsFocusable|
             QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsMovable);

        item->setPos(-560,-380);
        item->setScale(0.3);
        //表面卡片的ID和描述
        item->setData(ItemId,++seqNum);
        item->setData(ItemDescription,QVariant("图片"));
        scene->addItem(item);
        // 设置QGraphicsPixmapItem自适应QGraphicsView的大小
        ui->gView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::on_act_Edge_triggered() {
    if(srcImage.empty())
    return;
    Mat edgeImage,grayImage;
    //灰度图片
    cvtColor(srcImage,grayImage,COLOR_BGR2GRAY);
    //调用边缘检测函数
    Canny(grayImage,edgeImage,200,1);
    //转化为RGB类型的图像
    cvtColor(edgeImage,edgeImage,COLOR_GRAY2RGB);

    //Mat类型转化为QImage
    QImage image=QImage(edgeImage.data,edgeImage.cols,edgeImage.rows,edgeImage.cols*edgeImage.channels(),QImage::Format_RGB888);
    QGraphicsPixmapItem*item=new QGraphicsPixmapItem(QPixmap::fromImage(image));
    item->setFlags(QGraphicsItem::ItemIsFocusable|
             QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsMovable);
    item->setPos(-560,-380);
    item->setScale(0.3);
    scene->addItem(item);
}

void MainWindow::on_act_Filter_triggered() {
    Mat blurImage;
    blur(srcImage,blurImage,Size(3,3));
    //Mat类型转化为QImage
    QImage image=QImage(blurImage.data,blurImage.cols,blurImage.rows,blurImage.cols*blurImage.channels(),QImage::Format_RGB888);
    QGraphicsPixmapItem*item=new QGraphicsPixmapItem(QPixmap::fromImage(image));
    item->setFlags(QGraphicsItem::ItemIsFocusable|
             QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsMovable);
    item->setPos(-560,-380);
    item->setScale(0.3);
    scene->addItem(item);
}

void MainWindow::on_act_Gray_triggered() {
    Mat resultImage;
    cvtColor(srcImage,resultImage,COLOR_BGR2GRAY);
    //Mat类型转化为QImage
    QImage image=QImage(resultImage.data,resultImage.cols,resultImage.rows,resultImage.cols*resultImage.channels(),QImage::Format_Grayscale8);
    QGraphicsPixmapItem*item=new QGraphicsPixmapItem(QPixmap::fromImage(image));
    item->setFlags(QGraphicsItem::ItemIsFocusable|
             QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsMovable);
    item->setPos(-560,-380);
    item->setScale(0.3);
    scene->addItem(item);
}

void MainWindow::on_act_pallete_triggered() {
    isPenOriginal=false;
    QColor color;
    color=QColorDialog::getColor(color,NULL,"选择颜色");
    pen1.setColor(color);
}

void MainWindow::on_act_Penwidth_triggered() {
    pen1.setWidth(QInputDialog::getInt(this,"输入对话框","请输入宽度:"));
    isPenOriginal=false;
}

void MainWindow::on_act_Eraser_triggered() {
    //修改鼠标样式为一个圆
    isPenOriginal=false;
    pen1.setWidth(64);
    pen1.setColor(Qt::white);
}

void MainWindow::on_act_Storage_triggered() {
    //计算场景中item的数量1
    int cnt =scene->items().count();
    qDebug()<<cnt;
    if (cnt!= 0){
        QList<QGraphicsItem *> items = scene->items();
        for (int i = 0; i<cnt; ++i) {
            textItem = dynamic_cast<QGraphicsRectItem*>(items.at(i));
            if(textItem!= nullptr){
                qDebug()<<"exist";
                saveItemToDatabase(textItem);
            }
        }
    }
    //存储图片的数据
    QList<QGraphicsItem *> items = scene->items();
    for (int i = 0; i < cnt; ++i) {
        QGraphicsPixmapItem *pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(items.at(i));
        if (pixmapItem != nullptr) {
            qDebug() << "在场景中找到一个QGraphicsPixmapItem";
            savePixmapItemToDatabase(pixmapItem);
        }
    }
}

void MainWindow::on_act_Read_triggered() {
    addImagesFromDatabase();
    readItemFromDatabase();
}

void MainWindow::on_act_Serialization_triggered() {
    QList<QGraphicsItem *> items = scene->items();
    for (QGraphicsItem *item : items) {
        if (auto *rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item)) {
            rectitems.append(rectItem);
        } else if (auto *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item)) {
            pixmapItems.append(pixmapItem);
        }
    }
    // 进行序列化存储
    serializeObject(rectitems, "rect_item.dat");
    serializeObject(pixmapItems, "pixmap_item.dat");
}

void MainWindow::on_act_Deserialization_triggered() {
    readItemFromDatFile("pixmap_item.dat");
    readItemFromDatFile("rect_item.dat");
}
