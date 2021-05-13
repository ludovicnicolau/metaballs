#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << 100 << 300);
}

MainWindow::~MainWindow()
{
    delete ui;
}

