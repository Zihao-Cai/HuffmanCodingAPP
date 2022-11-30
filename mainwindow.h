#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <qstring.h>
#include "htree.h"
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void codingbtn_clicked();
    void decodingbtn_clicked();
    void exitbtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
