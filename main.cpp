#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    setbuf(stdout,NULL);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QMessageBox tip;
    tip.setStyleSheet("color:blue");
    tip.setIcon(QMessageBox::Information);
    tip.setWindowTitle("Information");
    tip.setText("Only ASCII words suit!");
    tip.exec();

    return a.exec();
}
