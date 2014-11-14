#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
//
#include <qhttpserver.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverconnection.hpp>
//
#include "handleget.h"
#include "handlepost.h"

using namespace qhttp::server;

class Handler : public QObject
{
    Q_OBJECT
public:
    explicit Handler(quint64 clientConnectId, QHttpRequest *request, QHttpResponse *response);

private:
    int m_clientConnectionId;
};

#endif // HANDLER_H
