#include <QApplication>

#include <iostream>

#include "MainWindow.h"

int main(int argc, char* argv[]) {
    std::cout << "test" << std::endl;
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();
    return a.exec();
}
