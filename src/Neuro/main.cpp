#include <QApplication>

#include "controller.h"

using namespace s21;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller w;
    w.setWindowTitle("MLP");
//    w.setWindowIcon(QIcon(":/icon"));
    w.show();
    return a.exec();
}
