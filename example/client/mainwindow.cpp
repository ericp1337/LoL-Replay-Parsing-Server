#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->settings = new Settings;
    this->model = new QFileSystemModel;
    model->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    model->setRootPath(settings->getReplayDir());

    this->ui->treeView->setModel(model);
    this->ui->treeView->setRootIndex(model->index(settings->getReplayDir()));
    qDebug() << this->ui->treeView->columnWidth(0);
    this->ui->treeView->setColumnWidth(0, 300);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settings;
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

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    this->model->filePath(index);
}
