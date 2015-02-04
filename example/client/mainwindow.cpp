#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->uploadReplayButton->setEnabled(false);

    this->settings = new Settings;
    this->ui->replayFolderLabel->setText(this->settings->getReplayDir());

    this->model = new QFileSystemModel;
    //these are the filters we use to determine what the model is to show.
    this->model->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    this->model->setRootPath(settings->getReplayDir());

    this->ui->treeView->setModel(model);
    this->ui->treeView->setRootIndex(model->index(settings->getReplayDir()));
    this->ui->treeView->setColumnWidth(0, 300);

    // Setup Selection Model so we no when items are selected and deselected for buttons (enable/disable)
    connect(this->ui->treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(treeView_current_selection_changed(QModelIndex,QModelIndex)));

    // Initialize Network Manager
    this->networkManager = new QNetworkAccessManager;
    this->progressDialog = new QProgressDialog("Uploading Replay...", "Cancel", 0, 100, this);
    this->progressDialog->setWindowModality(Qt::WindowModal);
    this->progressDialog->setAutoClose(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settings;
    delete model;
    delete networkManager;
    delete progressDialog;
}

void MainWindow::on_actionQuit_triggered()
{
    //sends a signal to Qt to quit.
    qApp->quit();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    // show a dialog box with Qt Information
    qApp->aboutQt();
}

void MainWindow::on_actionSettings_triggered()
{
    this->settings->exec();
}

void MainWindow::treeView_current_selection_changed(QModelIndex current, QModelIndex previous)
{
    if(current.isValid()) {
        if(!this->model->isDir(current)) {
            this->ui->uploadReplayButton->setEnabled(true);
            return;
        }
    }
    this->ui->uploadReplayButton->setDisabled(true);
}

void MainWindow::on_uploadReplayButton_clicked()
{
    QString filePath = this->model->filePath(this->ui->treeView->currentIndex());

    QFile replayFile(filePath);
    if(replayFile.open(QIODevice::ReadOnly)) {
        this->networkReply = this->networkManager->post(QNetworkRequest(QUrl(this->settings->getServerUrl().toString() + "/upload-lrf")), replayFile.readAll());
        //this->progressDialog->setMaximum(this->networkReply);
        // Send current status of upload to the progress dialog
        connect(this->networkReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(networkUploadProgress(qint64,qint64)));
        // Make sure to delete the reply after we have finished uploading so we are not leaking memory
        connect(this->progressDialog, SIGNAL(finished(int)), this->networkReply, SLOT(deleteLater()));
        // show the progress dialog now that we have started uploading a replay, but don't allow any more uploads since it only does one at a time currently.
        // You could make a QueueList for handling a list of files to be uploaded.
        this->progressDialog->exec();
    }
    replayFile.close();
}

void MainWindow::networkUploadProgress(qint64 current, qint64 total)
{
    this->progressDialog->setMaximum(total);
    this->progressDialog->setValue(current);
}
