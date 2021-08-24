#include "mainwindow.hpp"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setOrganizationName("L3harris");
    a.setApplicationName("RDT");
    MainWindow w;
    w.show();
    return a.exec();
}
