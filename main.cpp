#include <QApplication>
#include <QPushButton>

#include "Header/mainwindow.h"
#include <iostream>
#include<QGraphicsRectItem>
////数据库操作函数
// #include "sqlite3.h"
// static int callback(void *data, int argc, char **argv, char **azColName){
//     int i;
//     fprintf(stderr, "%s: ", (const char*)data);
//     for(i=0; i<argc; i++){
//         printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//     }
//     printf("\n");
//     return 0;
// }


int main(int argc, char* argv[])
{
    ////数据库测试
    // sqlite3 *db;
    // char *zErrMsg = 0;
    // int rc;
    // char *sql;
    // const char* data = "Callback function called";
    //
    // /* Open database */
    // rc = sqlite3_open("D:/ALLCODE/QTC++/Sources/Database/demodb.db", &db);
    // if( rc ){
    //     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    //     exit(0);
    // }else{
    //     fprintf(stderr, "Opened database successfully\n");
    // }
    //
    // /* Create SQL statement */
    // sql = "SELECT * from stuinfo";
    //
    // /* Execute SQL statement */
    // rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    // if( rc != SQLITE_OK ){
    //     fprintf(stderr, "SQL error: %s\n", zErrMsg);
    //     sqlite3_free(zErrMsg);
    // }else{
    //     fprintf(stdout, "Operation done successfully\n");
    // }
    // sqlite3_close(db);

    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return QApplication::exec();
}
// #include <QApplication>
// #include <QGraphicsRectItem>
// #include <QColor>
// #include <QBrush>
// #include <QSqlDatabase>
// #include <QSqlQuery>
// #include <QDebug>
// #include <QSqlError>
// #include <QSqlRecord>
// #include<QSqlDriver>
// #include<QTextCodec>
//
// void saveRectItemDataToDatabase(const QPointF& position, const QRectF& rect, const QColor& color);
//
// int main(int argc, char* argv[]) {
//     QApplication a(argc, argv);
//
//     // 创建一个矩形项
//     QGraphicsRectItem rectItem(0, 0, 100, 100); // x, y, width, height
//     rectItem.setBrush(QBrush(QColor("blue"))); // 设置矩形的填充颜色
//
//     // 保存矩形项的数据到数据库
//     saveRectItemDataToDatabase(rectItem.pos(), rectItem.rect(), rectItem.brush().color());
//
//     return a.exec();
// }
//
// void saveRectItemDataToDatabase(const QPointF& position, const QRectF& rect, const QColor& color) {
//     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//     db.setDatabaseName("myDatabase.db");
//     QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
//     if (!db.open()) {
//         qDebug() << "Failed to open database";
//         return;
//     }
//     db.open();
//     QSqlQuery query;
//
//     bool sueccse=query.exec("CREATE TABLE rect_items (id INTEGER PRIMARY KEY AUTOINCREMENT, position_x REAL, position_y REAL, width REAL, height REAL, color TEXT)");
//     if(sueccse)
//         qDebug()<<"create table success";
//     else
//         qDebug()<<"create table failed";
//
//     query.prepare("INSERT INTO rect_items(position_x, position_y, width, height, color) VALUES(:posx, :posy, :width, :height, :color)");
//
//     query.bindValue(":posx", position.x()+11);
//     query.bindValue(":posy", position.y());
//     query.bindValue(":width", rect.width());
//     query.bindValue(":height", rect.height());
//     query.bindValue(":color", color.name());
//     //清空数据表
//     //query.exec("DELETE FROM rect_items");
//     if (!query.exec()) {
//         qDebug() << "Error: Failed to execute query:" << query.lastError().text();
//     } else {
//         qDebug() << "Data inserted successfully";
//     }
//
//     db.close();
// }
