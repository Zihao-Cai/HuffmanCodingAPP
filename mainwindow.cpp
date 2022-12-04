#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->codingbtn,SIGNAL(clicked()),this,SLOT(codingbtn_clicked()));
    connect(ui->decodingbtn,SIGNAL(clicked()),this,SLOT(decodingbtn_clicked()));
    connect(ui->exitbtn,SIGNAL(clicked()),this,SLOT(exitbtn_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::codingbtn_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this);//从文件对话框选择目标文件
    ui->statusBar->showMessage(filepath);
    QFile* file = new QFile(filepath);
    Htree HT;
    QMessageBox tip;
    tip.setStyleSheet("color:blue");
    tip.setIcon(QMessageBox::Information);
    tip.setWindowTitle("Information");
    QString codpath;
    if(HT.create(file,codpath)){
        tip.setText("Coding successfully!\n" + codpath);
        tip.exec();
    }
    else{
        tip.setIcon(QMessageBox::Warning);
        tip.setText("Coding failed!");
        tip.exec();
    }
}

void MainWindow::decodingbtn_clicked(){
    QString filepath = QFileDialog::getOpenFileName(this);//从文件对话框选择目标文件
    ui->statusBar->showMessage(filepath);
    QFile* file = new QFile(filepath);
    Htree ht;
    QMessageBox tip;
    tip.setStyleSheet("color:blue");
    tip.setIcon(QMessageBox::Information);
    tip.setWindowTitle("Information");
    QString decpath;
    if(ht.decode(file,decpath)){
        tip.setText("Decoding successfully!\n" + decpath);
        tip.exec();
    }
    else{
        tip.setIcon(QMessageBox::Warning);
        tip.setText("Decoding failed!");
        tip.exec();
    }

}

void MainWindow::exitbtn_clicked(){
    this->close();
}


