//
// Created by Administrator on 24-8-2.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QPainter>
#include <QMainWindow>
#include<QLabel>
#include<QGraphicsScene>
#include<opencv2/opencv.hpp>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace cv;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
private:
    QLabel*labItemInfo;
    QLabel*labViewCord;
    QLabel*labSceneCord;
    QLabel*labItemCord;
    QGraphicsScene*scene;
    static const int ItemId=1;
    static const int ItemDescription=2;
    int seqNum=0;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    Ui::MainWindow *ui;
    Mat srcImage;//原始图像
    void backPen();
    bool IsPen;
    QPen pen1;//默认为黑色可以选择颜色
    bool isMove;
    //笔是否按下
    bool penOn;
    bool isPenOriginal;
    //创建一个原始的Item
    QGraphicsRectItem*rectItem=nullptr;//对应鼠标事件
    QPointF lastPoint;
    //修改矩形大小
    bool resizing = false;        // 标志是否正在调整矩形
    enum ResizeDirection { None, Top, Bottom, Left, Right, TopLeft, TopRight, BottomLeft, BottomRight } resizeDirection;
    QRectF initialRect;           // 记录初始矩形
    //容器
    QList<QGraphicsItem*>rectitems;//存储矩形
    QList<QGraphicsItem*>pixmapItems;//存储图片

    void updateCursorShape(const QPointF &pointScene);  // 更新鼠标指针形状
    void resizeRect(const QPointF &pointScene);         // 调整矩形尺寸
    //数据库存储与保存
    QGraphicsRectItem *textItem=nullptr;//对应存储功能

    void saveItemToDatabase(QGraphicsRectItem* rectItem);
    void savePixmapItemToDatabase(QGraphicsPixmapItem *pixmapItem);

    void readItemFromDatabase();
    void addImagesFromDatabase();
    //序列化存储与保存
    void serializeObject(const QList<QGraphicsItem*>& items, const QString& filename);

    void readItemFromDatFile(const QString& filename);
private slots:
    void on_keypress(QKeyEvent*event);
    void on_mouseClicked(QPoint point);
    void on_mouseDoubleClick(QPoint point);
    void on_mouseMovePoint(QPoint point);
    void on_mousePress(QPoint point);
    void on_mouseRelease(QPoint point);

    void on_actItem_Rect_triggered();
    void on_actItem_Ellipse_triggered();
    void on_actItem_Circle_triggered();
    void on_actItem_Line_triggered();
    void on_actItem_Polygon_triggered();
    void on_actItem_Triangle_triggered();
    void on_actItem_Text_triggered();
    void on_act_Paint_triggered();

    void on_actZoomIn_triggered();
    void on_actZoomOut_triggered();
    void on_actRestore_triggered();
    void on_actRotateLeft_triggered();
    void on_actRotateRight_triggered();
    void on_actEdit_Front_triggered();
    void on_actEdit_Back_triggered();
    void on_actGroup_triggered();
    void on_actGroupBreak_triggered();
    void on_actEdit_Delete_triggered();
    void on_actOpen_Photo_triggered();

    //边缘检测、中值滤波、灰度化处理 ！
    void on_act_Edge_triggered();
    void on_act_Filter_triggered();
    void on_act_Gray_triggered();

    //选择画笔
    void on_act_pallete_triggered();
    void on_act_Penwidth_triggered();
    void on_act_Eraser_triggered();

    //数据库存储
    void on_act_Storage_triggered();
    void on_act_Read_triggered();

    //序列化存储
    void on_act_Serialization_triggered();
    void on_act_Deserialization_triggered();
};

#endif //MAINWINDOW_H