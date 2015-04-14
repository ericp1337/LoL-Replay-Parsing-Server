#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include <QUrl>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    QString getReplayDir();
    QUrl getServerUrl();
    QString getAPIKey();

private slots:
    void on_browseButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::Settings *ui;
    QString replayDir;
    QUrl serverUrl;
    QString APIKey;
    QSettings *settings;
};

#endif // SETTINGS_H
