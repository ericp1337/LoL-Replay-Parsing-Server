/*
 * Use this class for interacting with Riot's API.
 * TODO: Move this stuff into a downloader class, and use this for api calls.
 */

#ifndef LOL_API_H
#define LOL_API_H

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

class lol_api : public QObject
{
    Q_OBJECT
public:
    explicit lol_api(QString apiKey, QObject *parent = 0);
    ~lol_api();

    void start();
    void setDlList(QStringList dl_list);
    void append(const int itemType, const QString item);
    void setClientVersion(QString version);

    // static
    static const int PROFILE_ICON;
    static const int CHAMPION_SQUARE;
    static const int SUMMONER_SPELL;
    static const int ITEM;
    static QString API_KEY;

signals:

public slots:

signals:
    void dlCompleted();

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

    static const QUrl ddBaseCDN;
    static QString ddVersion;
};

#endif // LOL_API_H
