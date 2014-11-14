#ifndef HANDLEGET_H
#define HANDLEGET_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QRegExp>
#include <QFile>
#include <QDir>
//
#include <qhttpserver.hpp>
#include <qhttpserverconnection.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

using namespace qhttp::server;

class HandleGet : public QObject
{
    Q_OBJECT
public:
    explicit HandleGet(QHttpRequest *request, QHttpResponse *response);

signals:

public slots:

private slots:
    void dataReadyRead(QByteArray data);
    void requestDone();

private:
    QHttpRequest *m_request;
    QHttpResponse *m_response;
    QByteArray m_buffer;

};

#endif // HANDLEGET_H
