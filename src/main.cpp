/*
 * Author: Eric Pfeiffer (Computerfr33k)
 * Description: This is a RESTful server that accepts League Replays as POST data and then returns back a json parsed version of the replay info.
 * License: GPLv3
 */

#include <QCoreApplication>
#include <QDir>

#include <qhttpserver.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverconnection.hpp>

#include "handler.h"

using namespace qhttp::server;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QHostAddress address = QHostAddress::LocalHost;
    QString port = "8008";

    if(argc > 2) {
        address = QHostAddress(argv[1]);
        port = argv[2];
    } else if(argc > 1) {
        port = QString(argv[1]);
    }

    QDir replays = QDir("replays");
    if(replays.exists())
        qDebug() << replays.absolutePath();
    else
        qDebug() << replays.mkpath(".");

    // trivial connection counter for tracking clients
    quint64 clientConnectionId = 0;

    QHttpServer server(&a);
    server.listen(address, port.toInt(), [&](QHttpRequest* req, QHttpResponse* res){
        new Handler(clientConnectionId++, req, res);
        // this ClientHandler object will be deleted automatically when:
        // socket disconnects (automatically after data has been sent to the client)
        // -> QHttpConnection destroys
        // -> QHttpRequest destroys -> ClientHandler destroys
        // -> QHttpResponse destroys
        // all by parent-child model of QObject.
    });

    if(server.isListening()) {
        // cout << "Server is now listening on: " << address.toString().toStdString() << ":" << port.toStdString() << endl;
        std::printf("Server is now listening on: %s:%d\n", address.toString().toStdString().data(), port.toInt());
        return a.exec();
    } else {
        std::fprintf(stderr, "Server could not listen on: %s:%d\n", address.toString().toStdString().data(), port.toInt());
        return -1;
    }
}
