#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <qhttpserver.h>
#include <stdio.h>
#include <string>
#include <QJsonDocument>
#include <QString>
#include <QByteArray>
#include <QFile>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = 0);
    HttpServer(int port, QObject *parent = 0);
    bool start();
    QString getErrorMsg();

signals:

public slots:

private slots:
    void newRequest(QHttpRequest *req,QHttpResponse *resp);
    QString parseReplay();
    void dataReadyRead(QByteArray);

private:
    int m_port;
    QString m_errorMsg;
    QHttpRequest *m_request;
    QHttpResponse *m_respnse;
    QByteArray buffer;

    QFile *file;
    int clientId;

};

#endif // HTTPSERVER_H
