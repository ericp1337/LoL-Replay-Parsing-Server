#include "lol_api.h"
#include <QStandardPaths>

const int lol_api::PROFILE_ICON = 1;
const int lol_api::CHAMPION_SQUARE = 2;
const int lol_api::SUMMONER_SPELL = 3;
const int lol_api::ITEM = 4;
const QUrl lol_api::ddBaseCDN = QUrl("http://ddragon.leagueoflegends.com/cdn/");
QString lol_api::ddVersion;
QString lol_api::API_KEY;

lol_api::lol_api(QString apiKey, QObject *parent) : QObject(parent)
{
    lol_api::API_KEY = apiKey;

    eventLoop = new QEventLoop;
    this->networkManager = new QNetworkAccessManager;
    this->networkDiskCache = new QNetworkDiskCache;
    this->networkDiskCache->setCacheDirectory(QStandardPaths::standardLocations( QStandardPaths::TempLocation ).at(0));
    this->networkManager->setCache(this->networkDiskCache);
}

void lol_api::setClientVersion(QString version)
{
    QNetworkRequest request(QUrl("https://global.api.pvp.net/api/lol/static-data/na/v1.2/versions?api_key=" + lol_api::API_KEY));
    QNetworkAccessManager manager;
    QNetworkReply *r = manager.get(request);

    QEventLoop loop;
    connect(r, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QJsonArray versions = QJsonDocument::fromJson(r->readAll()).array();
    QString v;
    version = version.split(".").at(0) + "." + version.split(".").at(1);
    bool found = false;
    for(QJsonArray::const_iterator it = versions.constBegin(), ite = versions.constEnd(); it != ite; ++it) {
        v = (*it).toString();
        if(v.contains(version + ".") && !found) {
            version = v;
        }
    }

    //get latest game version if we cannot find it using the local version from the replay
    if(!found) {
        version = versions.first().toString();
    }

    lol_api::ddVersion = version;
    r->deleteLater();
}

void lol_api::start()
{
    // start to process the list of files to be downloaded
    while(!this->queue.isEmpty()) {
        QUrl item = QUrl::fromUserInput(queue.dequeue());
        this->tempFile = new QTemporaryFile(item.fileName().split(".").at(0));
        if(!this->tempFile->open()) {
            qWarning() << "Temp File Could not be opened.";
            return;
        }

        QNetworkRequest request;
        request.setUrl(item);
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
        this->networkReply = this->networkManager->get(request);

        connect(this->networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(networkError(QNetworkReply::NetworkError)));
        connect(this->networkReply, SIGNAL(readyRead()), this, SLOT(dlReadyRead()));
        connect(this->networkReply, SIGNAL(finished()), this, SLOT(downloadComplete()));
        connect(this->networkReply, SIGNAL(finished()), eventLoop, SLOT(quit()));
        eventLoop->exec();
    }

    // All files have been downloaded since the queue is empty, so now lets emit a signal to anyone who needs to know.
    emit dlCompleted();
}

lol_api::~lol_api()
{
    delete networkManager;
    delete eventLoop;
}

void lol_api::setDlList(const QStringList dl_list)
{
    // Should I require this to be the URL?
    // I think that might make it easier for using with multiple different types of request endpoints. Such as Getting champions & Items using this same class.
    foreach(QString item, dl_list) {
        qDebug() << "Adding Item: " << item;
        this->queue.enqueue(item);
    }
}

void lol_api::append(const int itemType, const QString item)
{
    if(lol_api::ddVersion.isEmpty()) {
        qWarning() << "lol_api::append ddVersion has not been set. You can set it using lol_api::setClientVersion(QString version).Item will not be added.";
        return;
    }

    QString endpoint;

    switch(itemType) {
    case 1: // PROFILE_ICON
        endpoint = "img/profileicon/";
        break;
    case 2: // CHAMPION_SQUARE
        endpoint = "img/champion/";
        break;
    case 3: // SUMMONER_SPELL
        endpoint = "img/spell/";
        break;
    case 4: // ITEM
        endpoint = "img/item/";
        break;
    default: // IF Type doesn't match any of above
        return;
        break;
    }

    QString url = lol_api::ddBaseCDN.toString() + lol_api::ddVersion + "/" + endpoint + item + ".png";

    this->queue.enqueue(url);
}

void lol_api::downloadComplete()
{
    if(this->networkReply->error() == QNetworkReply::NoError) {
        this->tempFile->copy(QStandardPaths::standardLocations( QStandardPaths::TempLocation ).at(0) + "/" + this->tempFile->fileTemplate() + ".png");
        this->tempFile->close();
    }
    this->tempFile->deleteLater();
    this->networkReply->deleteLater();
}

void lol_api::dlReadyRead()
{
    if(this->networkReply->error() == QNetworkReply::NoError)
        this->tempFile->write(this->networkReply->readAll());
}

void lol_api::networkError(QNetworkReply::NetworkError error)
{
    qWarning() << "lol_api::NetworkError: " << this->networkReply->errorString();
}
