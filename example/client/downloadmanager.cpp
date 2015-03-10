#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
    eventLoop = new QEventLoop;
    this->networkManager = new QNetworkAccessManager;
    this->networkDiskCache = new QNetworkDiskCache;
    this->networkDiskCache->setCacheDirectory("/tmp");
    this->networkManager->setCache(this->networkDiskCache);
}

void DownloadManager::start()
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

DownloadManager::~DownloadManager()
{
    delete networkManager;
    delete eventLoop;
}

void DownloadManager::setDlList(const QStringList dl_list)
{
    // Should I require this to be the URL?
    // I think that might make it easier for using with multiple different types of request endpoints. Such as Getting champions & Items using this same class.
    foreach(QString item, dl_list) {
        qDebug() << "Adding Item: " << item;
        this->queue.enqueue(item);
    }
}

void DownloadManager::append(const QString item)
{
    this->queue.enqueue(item);
}

void DownloadManager::downloadComplete()
{
    if(this->networkReply->error() == QNetworkReply::NoError) {
        this->tempFile->copy("/tmp/" + this->tempFile->fileTemplate() + ".png");
        this->tempFile->close();
    }
    this->tempFile->deleteLater();
    this->networkReply->deleteLater();
}

void DownloadManager::dlReadyRead()
{
    if(this->networkReply->error() == QNetworkReply::NoError)
        this->tempFile->write(this->networkReply->readAll());
}

void DownloadManager::networkError(QNetworkReply::NetworkError error)
{
    qWarning() << "lol_api::NetworkError: " << this->networkReply->errorString();
}