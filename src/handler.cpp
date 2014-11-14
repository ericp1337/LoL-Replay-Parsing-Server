#include "handler.h"

Handler::Handler(quint64 clientConnectId,QHttpRequest *request, QHttpResponse *response) : QObject(request), m_clientConnectionId(clientConnectId)
{
    // This is an unauthenticated REST server, anyone can upload files to this server.
    // Adding api keys would be relatively easy.
    // If you are concerned with security would be to check the file that has been upload to make to sure it is actually a replay file,
    // right now we just assume which is bad security, but this is for a class (limited time).

    qDebug() << "[" << QDateTime::currentDateTimeUtc().toString() << "] " << "client connected";

    if(request->method() == qhttp::EHTTP_GET) {
        new HandleGet(request, response);

    } else if(request->method() == qhttp::EHTTP_POST) {
        new HandlePost(clientConnectId, request, response);

    } else {
        response->setStatusCode(qhttp::ESTATUS_NOT_IMPLEMENTED);
    }
}
