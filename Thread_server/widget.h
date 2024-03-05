#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include "thread.h"
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void showRead(QByteArray buff);
    void myConnected();
    void deletePort();


    void on_disc_one_Button_clicked();

    void on_disc_all_Button_clicked();

    void on_closeSerButton_clicked();

    void on_startSerButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QList<QTcpSocket*> ThreadList;


//protected:
    //void incomingConnection(quintptr socketDescriptor);
};
#endif // WIDGET_H
