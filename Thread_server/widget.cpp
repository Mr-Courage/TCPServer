#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer;

    connect(server,SIGNAL(newConnection()),this,SLOT(myConnected()));
    ui->startSerButton->setEnabled(true);
    ui->closeSerButton->setEnabled(false);
    ui->QradioButton->setChecked(true);//默认为群聊


}

Widget::~Widget()
{
    delete ui;
}

void Widget::showRead(QByteArray buff)
{
    int i = 0;
    ui->textBrowser->append(buff);

    while(i<ThreadList.length()){
        ThreadList[i]->write(buff);
        i++;
    }

}

void Widget::myConnected()
{
    socket = server->nextPendingConnection();
    Thread *t= new Thread(socket);
    t->start();

    ui->ThreadList->addItem(QString::number(socket->peerPort()));
    ThreadList.append(socket);//将新连接的客户端加入

    //connect(t,SIGNAL(finished()),this,SLOT(deleteLater()));//这个代码会和线程里的exit冲突

    connect(t,SIGNAL(deletPort()),this,SLOT(deletePort()),Qt::UniqueConnection);
    connect(t,SIGNAL(showRead(QByteArray)),this,SLOT(showRead(QByteArray)),Qt::UniqueConnection);
    ui->clientLabel->setText("当前用户："+QString::number(ThreadList.length())+"人");//显示人数
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
            ThreadList.removeAt(i);
            ui->clientLabel->setText("当前用户："+QString::number(ThreadList.length())+"人");//显示人数
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


void Widget::on_disc_all_Button_clicked()
{
    int i=0;
    while(i<ThreadList.length()){
        ThreadList[i]->disconnectFromHost();
    }

}


void Widget::on_closeSerButton_clicked()
{
    server->close();
    ui->textBrowser->append("服务器关闭");
    ui->startSerButton->setEnabled(true);
    ui->closeSerButton->setEnabled(false);
}


void Widget::on_startSerButton_clicked()
{
    server->listen(QHostAddress::AnyIPv4,8000);
    ui->textBrowser->append("服务器开启");
    ui->startSerButton->setEnabled(false);
    ui->closeSerButton->setEnabled(true);
}


void Widget::on_pushButton_clicked()
{
    int i = 0;
    QString str = ui->sendEdit->text();
    if(ui->SradioButton->isChecked()){
        while(i<ThreadList.length()){
            if(QString::number(ThreadList[i]->peerPort()) == ui->ThreadList->currentText()){
                ThreadList[i]->write("(私聊)管理员："+str.toUtf8());
                ui->textBrowser->append(str.toUtf8());
                break;
            }
            i++;
        }
    }
    if(ui->QradioButton->isChecked()){
        while(i<ThreadList.length()){
            ThreadList[i]->write("(群聊)管理员："+str.toUtf8());
            i++;
        }
        ui->textBrowser->append(str.toUtf8());
    }
}

