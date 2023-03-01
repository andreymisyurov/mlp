#include <QApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller w;
    w.setWindowTitle("MLP");
//    w.setWindowIcon(QIcon(":/icon"));
    w.show();
    return a.exec();
}
