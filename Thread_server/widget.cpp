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

    ui->ThreadList->addItem(QString::number(socket->peerPort()));
    ThreadList.append(socket);//将新连接的客户端加入

    //connect(t,SIGNAL(finished()),this,SLOT(deleteLater()));//这个代码会和线程里的exit冲突

    connect(t,SIGNAL(deletPort()),this,SLOT(deletePort()));
    connect(t,SIGNAL(showRead(QByteArray)),this,SLOT(showRead(QByteArray)));
}

/*void Widget::incomingConnection(quintptr socketDescriptor)
{
    Thread *t = new Thread(socketDescriptor);
    connect(t,SIGNAL(finished()),this,SLOT(deleteLater()));
    t->start();

    connect(t,SIGNAL(showRead(QByteArray)),this,SLOT(showRead(QByteArray)));
}*/

void Widget::deletePort(){
    int i = 0;
    while(!ThreadList.empty()){

        if(ThreadList[i]->state() == QAbstractSocket::UnconnectedState){
            int index = ui->ThreadList->findText(QString::number(ThreadList[i]->peerPort()));
            ui->ThreadList->removeItem(index);
            return;
        i++;

        }
    }
}

void Widget::on_disc_one_Button_clicked()
{
    int i = 0;
    QString curr_content = ui->ThreadList->currentText();
    while(!ThreadList.empty()){
        if(QString::number(ThreadList[i]->peerPort()) == curr_content){
            ThreadList[i]->disconnectFromHost();
            return;
        }
    }
}

