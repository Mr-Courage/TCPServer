#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer;
    server->listen(QHostAddress::AnyIPv4,8000);
    connect(server,SIGNAL(newConnection()),this,SLOT(myConnected()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::showRead(QByteArray buff)
{
    ui->textBrowser->append(buff);
}

void Widget::myConnected()
{
    socket = server->nextPendingConnection();
    Thread *t= new Thread(socket);
    t->start();

    //connect(t,SIGNAL(finished()),this,SLOT(deleteLater()));//这个代码会和线程里的exit冲突


    connect(t,SIGNAL(showRead(QByteArray)),this,SLOT(showRead(QByteArray)));
}

/*void Widget::incomingConnection(quintptr socketDescriptor)
{
    Thread *t = new Thread(socketDescriptor);
    connect(t,SIGNAL(finished()),this,SLOT(deleteLater()));
    t->start();

    connect(t,SIGNAL(showRead(QByteArray)),this,SLOT(showRead(QByteArray)));
}*/


