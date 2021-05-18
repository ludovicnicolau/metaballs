#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "metaballslistmodel.h"

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
    void onCheckBoxShowGridStateChanged(int state);
    void onSliderThresholdValueChanged(int value);
    void onSliderRadiusValueChanged(int value);
    void deleteItem();

private:
    Ui::MainWindow *ui;

    MetaballsListModel m_metaballs_list_model;
};
#endif // MAINWINDOW_H
