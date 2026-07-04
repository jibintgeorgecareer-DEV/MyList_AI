#include "mainwindow.h"

#include <QApplication>
#include <QIcon>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Jibin_Softwares");
    QCoreApplication::setApplicationName("MyList");

    a.setWindowIcon(QIcon(":/icons/logo.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}
