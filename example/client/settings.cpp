#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    /*
     * Setup previous settings if they exist
     */
    this->settings = new QSettings("settings.ini", QSettings::IniFormat, this);
    this->replayDir = settings->value("replay_dir", "").toString();
    this->ui->replayDir->setText(this->replayDir);
    this->serverUrl = QUrl::fromUserInput(this->settings->value("server_url", "").toString());
    this->ui->serverUrl->setText(this->serverUrl.toString());
}

Settings::~Settings()
{
    if(settings) {
        settings->deleteLater();
    }

    delete ui;
    delete settings;
}

void Settings::on_browseButton_clicked()
{
    // currently only support one directory for now.
    // could add them as a list if we wanted to.
    QFileDialog folder;
    folder.setFileMode(QFileDialog::Directory);
    QString tmp = folder.getExistingDirectory(this, "Select Replay Folder", "");

    if(!tmp.isEmpty()) {
        this->replayDir = tmp;
        this->ui->replayDir->setText(this->replayDir);
    }
}

QString Settings::getReplayDir()
{
    return this->replayDir;
}

QUrl Settings::getServerUrl()
{
    return this->serverUrl;
}

void Settings::on_buttonBox_accepted()
{
    this->serverUrl = QUrl::fromUserInput(this->ui->serverUrl->text());

    this->settings->setValue("replay_dir", this->replayDir);
    this->settings->setValue("server_url", this->serverUrl.toString());
    this->settings->sync();
}
