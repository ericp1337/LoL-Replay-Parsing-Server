#ifndef HANDLEPUT_H
#define HANDLEPUT_H

#include <QObject>
#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <stdio.h>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <cmath>

using namespace std;

class HandlePut : public QObject
{
    Q_OBJECT
public:
    explicit HandlePut(QObject *parent = 0);
    HandlePut(QHttpRequest *request, QHttpResponse *response);
    ~HandlePut();

signals:

public slots:

private slots:
    void requestDone();
    void dataReadyRead(QByteArray data);

private:
    QHttpRequest *m_request;
    QHttpResponse *m_response;
    QFile *m_file;
    QString m_filename;
    int m_totalDataSize;

    // This is to parse the replay that is created using the 3rd party replay client "LOL Recorder"
    QJsonDocument parseLrf(QByteArray data);
    // This is to parse the official replays from League of Legends. (currently unsupported as they have been removed from PBE)
    QJsonDocument parseReplay();

};

#endif // HANDLEPUT_H
