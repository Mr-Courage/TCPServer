#include "widget.h"

#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("并发服务器");
    QIcon icon("D:/c++/QT_c++/Thread_server/logo.ico");
    w.setWindowIcon(icon);
    w.show();
    return a.exec();
}
