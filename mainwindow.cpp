#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << 100 << 300);
    connect(ui->checkBox_show_grid, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxShowGridStateChanged(int)));
    connect(ui->hSlider_threshold, SIGNAL(valueChanged(int)), this, SLOT(onSliderThresholdValueChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCheckBoxShowGridStateChanged(int state)
{
    if (state == Qt::CheckState::Checked)
    {
        ui->openGLWidget->setGridVisible(true);
    }
    else
    {
        ui->openGLWidget->setGridVisible(false);
    }
}

void MainWindow::onSliderThresholdValueChanged(int value)
{
    ui->openGLWidget->setThreshold(value / 10.0f);
}

