#ifndef HANDLEPOST_H
#define HANDLEPOST_H

// Qt Libs
#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
///////////////////
// QHttpServer Libs
#include <qhttpserver.hpp>
#include <qhttpserverconnection.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>
///////////////////

using namespace qhttp::server;

class HandlePost : public QObject
{
    Q_OBJECT
public:
    explicit HandlePost(quint64 clientId, QHttpRequest *request, QHttpResponse *response);

signals:

public slots:

private slots:
    void requestDone();
    void dataReadyRead(QByteArray data);

private:
    QHttpRequest    *m_request;
    QHttpResponse   *m_response;
    QFile           *m_upload;
    quint64         m_clientConnectionId;
    bool            m_error;

};

#endif // HANDLEPOST_H
