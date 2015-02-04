#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionSettings_triggered();
    void treeView_current_selection_changed(QModelIndex current, QModelIndex previous);

    void on_uploadReplayButton_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    Settings *settings;
    QNetworkAccessManager *networkManager;
    QNetworkReply *networkReply;
};

#endif // MAINWINDOW_H
