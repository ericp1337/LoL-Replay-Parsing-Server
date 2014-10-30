#include "httpserver.h"

using namespace std;

HttpServer::HttpServer(QObject *parent) : QObject(parent), m_port(8000)
{
}

HttpServer::HttpServer(int port, QObject *parent) : m_port(port), QObject(parent)
{
}

bool HttpServer::start()
{
    QHttpServer *httpServer = new QHttpServer(this);

    connect(httpServer, SIGNAL(newRequest(QHttpRequest*,QHttpResponse*)), this, SLOT(newRequest(QHttpRequest*,QHttpResponse*)));


    return httpServer->listen(this->m_port);
}

void HttpServer::newRequest(QHttpRequest *req, QHttpResponse *resp)
{
    if(req->path() == "/upload")
        new Handler(req, resp);
    else {
        resp->writeHead(501);
        resp->end();
    }
}

QString HttpServer::parseReplay()
{
    char *ch;
    //this->file->close();
    //qDebug() << this->m_request->body();

    //QJsonDocument json = QJsonDocument::fromJson(data);
    this->m_respnse->writeHead(200);
    this->m_respnse->end("upload done.");

    int start = this->m_request->body().indexOf("{\"gameId\"");
    int end = this->m_request->body().lastIndexOf("gameLength");
    end = this->m_request->body().indexOf("}", end);

    QString buffer;
    for(int i=start; i <= end; i++) {
        buffer += QString(this->m_request->body()[i]);
    }
    qDebug() << QJsonDocument::fromJson(buffer.toUtf8());

    this->file->close();

    return "";
}

void HttpServer::dataReadyRead(QByteArray data)
{
    this->file->write(data);
}
