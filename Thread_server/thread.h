#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QTcpSocket *s,QObject *parent = nullptr);
public slots:
    void run();
signals:
    void showRead(QByteArray);
    void deletPort();


private:
    quintptr socketDescriptor;
    QTcpSocket *socket;

private slots:
    void read();
    void disconnect();

};

#endif // THREAD_H
