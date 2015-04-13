#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->uploadReplayButton->setEnabled(false);

    this->itemDelegate = new QStyledItemDelegate;

    this->settings = new Settings;
    this->ui->replayFolderLabel->setText(this->settings->getReplayDir());

    this->model = new QFileSystemModel;
    //these are the filters we use to determine what the model is to show.
    this->model->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    this->model->setRootPath(settings->getReplayDir());
    this->ui->tableView->setItemDelegate(this->itemDelegate);

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

    // Setup TableView w/ base model
    this->table_model = new QStandardItemModel;

    // Get User's dir for app data.
    this->userDir = QDir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first());

    this->tempDir = QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0) + "/";

    // Game Constants
    this->dataDragon = "http://ddragon.leagueoflegends.com/cdn/5.7.2/img/";
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

// This will enable/disable the upload button based upon what is selected and if it is valid or not.
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
    if(this->settings->getServerUrl().isEmpty()) {
        QMessageBox::warning(this, "Uh Oh!", "you need to set a server url in the settings before trying to upload a replay", "close");
        return;
    }

    QString filePath = this->model->filePath(this->ui->treeView->currentIndex());

    QFile replayFile(filePath);
    if(replayFile.open(QIODevice::ReadOnly)) {
        QNetworkRequest req(QUrl(this->settings->getServerUrl().toString() + "/upload-lrf"));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        this->networkReply = this->networkManager->post(req, replayFile.readAll());

        // Send current status of upload to the progress dialog
        connect(this->networkReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(networkUploadProgress(qint64,qint64)));

        // Retrieve Info from uploaded file.
        connect(this->networkReply, SIGNAL(finished()), this, SLOT(uploadComplete()));

        // Cancel Upload if user cancel's using progress Dialog
        connect(this->progressDialog, SIGNAL(canceled()), this->networkReply, SLOT(abort()));

        connect(this->networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(networkError(QNetworkReply::NetworkError)));

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

bool MainWindow::replayIsValid(const QJsonDocument replay)
{
    QString matchID = QString::number(replay.object().value("matchID").toDouble(), 'f', 0);
    qDebug() << matchID;
    if(matchID == "0" || matchID == "-1") {
        return false;
    }
    return true;
}

void MainWindow::uploadComplete()
{
    // I need to try and seperate some of this functionality into seperate funstions so its not so big.
    setupTableModel();
    QJsonDocument replay = QJsonDocument::fromJson(this->networkReply->readAll());

    if(!this->replayIsValid(replay)) {
        qWarning() << "replay is not valid";
        this->progressDialog->cancel();
        this->networkReply->deleteLater();
        QMessageBox::warning(this, "Replay Error", "There was a problem parsing the replay. It could be corrupt or not contain valid match info.", "Close");
        return;
    }

    QString matchID = QString::number(replay.object().value("matchID").toDouble(), 'f', 0);
    qDebug() << replay;
    this->ui->matchID->setText(matchID);
    this->ui->gameMode->setText(replay.object().value("gameMode").toString());
    this->ui->region->setText(replay.object().value("region").toString().toUpper());
    this->ui->matchLength->setText(QString::number(replay.object().value("matchLength").toDouble(), 'f', 0));

    // Used to determine where in the table to insert each player on the team, since it could be out of order in the Array.
    // We Start at 1 because the header for each team starts on the first row where we insert the team.
    int blueCounter = 1;
    int purpleCounter = 7;

    lol_api lol_downloader;

    foreach(QJsonValue obj, replay.object().value("players").toArray()) {
        if(obj.toObject().value("team").toInt() == 1) {
            // Download Images
            lol_downloader.append(this->dataDragon.toString() + "item/" + QString::number(obj.toObject().value("item1").toDouble(), 'f', 0) + ".png");
            lol_downloader.append(this->dataDragon.toString() + "champion/" + obj.toObject().value("champion").toString() + ".png");
            lol_downloader.append(this->dataDragon.toString() + "profileicon/" + obj.toObject().value());
            //

            // Blue Team
            this->table_model->setItem(blueCounter, 0, new QStandardItem(obj.toObject().value("summoner").toString()));
            this->table_model->setItem(blueCounter, 1, new QStandardItem(QString::number(obj.toObject().value("level").toInt())));

            this->table_model->setItem(blueCounter, 2, new QStandardItem(QIcon(this->tempDir + obj.toObject().value("champion").toString()), obj.toObject().value("champion").toString()));

            this->table_model->setItem(blueCounter, 3, new QStandardItem(QString::number(obj.toObject().value("kills").toInt())));
            this->table_model->setItem(blueCounter, 4, new QStandardItem(QString::number(obj.toObject().value("deaths").toInt())));
            this->table_model->setItem(blueCounter, 5, new QStandardItem(QString::number(obj.toObject().value("assists").toInt())));

            // Compile Items Section
            QStandardItem *items = new QStandardItem;
            // Set Item image name in DecorationRole so we can retrieve that data in our extended QStandardItem class and use it to display image
            items->setData(QImage(this->tempDir + QString::number(obj.toObject().value("item1").toDouble(), 'f', 0) + ".png").scaled(30,30,Qt::KeepAspectRatio), Qt::DecorationRole);
            this->table_model->setItem(blueCounter, 6, items);
            //

            this->table_model->setItem(blueCounter, 7, new QStandardItem(QString::number(obj.toObject().value("gold").toInt())));
            this->table_model->setItem(blueCounter, 8, new QStandardItem(QString::number(obj.toObject().value("minions").toInt())));

            blueCounter++;
        } else {
            lol_downloader.append(this->dataDragon.toString() + "champion/" + obj.toObject().value("champion").toString() + ".png");

            // Purple Team
            this->table_model->setItem(purpleCounter, 0, new QStandardItem(obj.toObject().value("summoner").toString()));
            this->table_model->setItem(purpleCounter, 1, new QStandardItem(QString::number(obj.toObject().value("level").toInt())));
            this->table_model->setItem(purpleCounter, 2, new QStandardItem(QIcon(this->tempDir + obj.toObject().value("champion").toString()), obj.toObject().value("champion").toString()));
            this->table_model->setItem(purpleCounter, 3, new QStandardItem(QString::number(obj.toObject().value("kills").toInt())));
            this->table_model->setItem(purpleCounter, 4, new QStandardItem(QString::number(obj.toObject().value("deaths").toInt())));
            this->table_model->setItem(purpleCounter, 5, new QStandardItem(QString::number(obj.toObject().value("assists").toInt())));
            // Compile Items Section
            //
            this->table_model->setItem(purpleCounter, 7, new QStandardItem(QString::number(obj.toObject().value("gold").toInt())));
            this->table_model->setItem(purpleCounter, 8, new QStandardItem(QString::number(obj.toObject().value("minions").toInt())));
            purpleCounter++;
        }
    }

    lol_downloader.start();
    this->ui->tableView->resizeColumnsToContents();
    this->ui->tableView->resizeRowsToContents();

    //this->networkReply->deleteLater();
}

void MainWindow::networkError(QNetworkReply::NetworkError reply)
{
    qWarning() << "Network Error: " << this->networkReply->errorString();
    this->progressDialog->cancel();
    QMessageBox::information(this, "Network Error", "There was a problem with the network connection", "Close");
    //this->progressDialog->close();
    //this->networkReply->abort();
    //this->networkReply->deleteLater();
}

void MainWindow::imgDownloadDone()
{
}

void MainWindow::setupTableModel()
{
    //Make sure we clear it first to ensure the correct base.
    this->table_model->clear();

    table_model->setHorizontalHeaderLabels(QStringList() << "Player" << "Level" << "Champion" << "K" << "D" << "A" << "Items" << "Gold" << "LH");
    this->ui->tableView->setModel(table_model);
    //table_model->insertRows(0, 12);

    // Insert Blue Team Section into table
    QStandardItem *blueTeamLabel = new QStandardItem("Blue Team");
    blueTeamLabel->setBackground(Qt::blue);
    blueTeamLabel->setForeground(Qt::white);
    table_model->setItem(0, blueTeamLabel);

    // Insert Purple Team Section into table
    QStandardItem *purpleTeamLabel = new QStandardItem("Purple Team");
    purpleTeamLabel->setBackground(Qt::magenta);
    purpleTeamLabel->setForeground(Qt::white);
    table_model->setItem(6, purpleTeamLabel);

    this->table_model->setHeaderData(6, Qt::Horizontal, "Items", Qt::DecorationRole);
}
