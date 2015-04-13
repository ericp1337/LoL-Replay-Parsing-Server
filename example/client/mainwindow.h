#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressDialog>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QStyledItemDelegate>
#include <QMessageBox>
#include "settings.h"
#include <QTemporaryFile>
#include <QStandardPaths>
#include <QThread>

#include "settings.h"
#include "lol_api.h"

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
    void networkUploadProgress(qint64, qint64);
    void uploadComplete();
    void networkError(QNetworkReply::NetworkError reply);

    // slots for downloading images
    void imgDownloadDone();
    //

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    Settings *settings;
    QNetworkAccessManager *networkManager;
    QNetworkReply *networkReply;
    QProgressDialog *progressDialog;
    QStandardItemModel *table_model;
    QStyledItemDelegate *itemDelegate;
    QDir userDir;
    QString tempDir;
    QUrl dataDragon;

private:
    void setupTableModel();
    bool replayIsValid(const QJsonDocument replay);
};

#endif // MAINWINDOW_H
