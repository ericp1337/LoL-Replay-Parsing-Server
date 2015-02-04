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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settings;
    delete model;
    delete networkManager;
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
    if(replayFile.open(QIODevice::ReadOnly))
        this->networkReply = this->networkManager->post(QNetworkRequest(QUrl(this->settings->getServerUrl().toString() + "/upload-lrf")), replayFile.readAll());
    replayFile.close();
}
