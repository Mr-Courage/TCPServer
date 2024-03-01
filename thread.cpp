#include "thread.h"
#include <QDebug>
Thread::Thread(QTcpSocket *s, QObject *parent)
    : QThread{parent}
{
    socket = s;

}

void Thread::run()
{
    connect(socket,SIGNAL(readyRead()),this,SLOT(read()),Qt::DirectConnection);

    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnect()),Qt::DirectConnection);

    this->exec();
}

void Thread::read()
{
    QByteArray buff = socket->readAll();

    qDebug()<<buff;
    emit showRead(buff);
}

void Thread::disconnect()
{
    this->socket->disconnectFromHost();
    this->socket->close();
    this->socket->deleteLater();
    this->exit();

}

