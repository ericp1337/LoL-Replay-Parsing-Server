#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QTemporaryFile>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFileInfo>
#include <QEventLoop>
#include <QTime>
#include <QNetworkDiskCache>
#include <QQueue>

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);
    DownloadManager();
    void start();
    ~DownloadManager();
    void setDlList(QStringList dl_list);
    void append(const QString item);

signals:
    void dlCompleted();

public slots:

private slots:
    void downloadComplete();
    void dlReadyRead();
    void networkError(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *networkManager;
    QNetworkReply *networkReply;
    QTemporaryFile *tempFile;
    QNetworkDiskCache *networkDiskCache;
    QEventLoop *eventLoop;
    QQueue<QString> queue;
};

#endif // DOWNLOADMANAGER_H
