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
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "handler.h"
#include "handleput.h"
#include "handleget.h"

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = 0);
    HttpServer(int port, QObject *parent = 0);
    ~HttpServer();
    bool start();
    QString getErrorMsg();

signals:

public slots:

private slots:
    void newRequest(QHttpRequest *req,QHttpResponse *resp);

private:
    QString m_errorMsg;
    QHttpRequest *m_request;
    QHttpResponse *m_respnse;

    int clientId;
    int m_port;

    QSqlDatabase *sqlDatabase;
};

#endif // HTTPSERVER_H
