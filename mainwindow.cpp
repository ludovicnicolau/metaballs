#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QList>
#include <QListView>
#include <QShortcut>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_metaballs_list_model(this)
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << 100 << 300);

    ui->listView_metaballs->setModel(&m_metaballs_list_model);
    ui->openGLWidget->setMetaballsListModel(&m_metaballs_list_model);

    connect(ui->checkBox_show_grid, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxShowGridStateChanged(int)));
    connect(ui->hSlider_threshold, SIGNAL(valueChanged(int)), this, SLOT(onSliderThresholdValueChanged(int)));
    connect(ui->hSlider_radius, SIGNAL(valueChanged(int)), this, SLOT(onSliderRadiusValueChanged(int)));
    connect(ui->comboBox_operations, SIGNAL(currentIndexChanged(int)), ui->openGLWidget, SLOT(setOperation(int)));
    connect(ui->listView_metaballs->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
            ui->openGLWidget, SLOT(setSelectedItemInListView(const QModelIndex&, const QModelIndex&)));


    new QShortcut(QKeySequence(QKeySequence::Delete), this, SLOT(deleteItem()));
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

void MainWindow::onSliderRadiusValueChanged(int value)
{
    ui->openGLWidget->setCurrentMetaballRadius(value);
}

void MainWindow::deleteItem()
{
    ui->listView_metaballs->model()->removeRow(ui->listView_metaballs->currentIndex().row());
    ui->listView_metaballs->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::SelectionFlag::Clear);
}

