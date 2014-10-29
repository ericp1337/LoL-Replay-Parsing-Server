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
    this->clientId = 1;

    if(req->method() == QHttpRequest::HTTP_PUT) {
        connect(req, SIGNAL(end()), this, SLOT(parseReplay()));
        connect(req, SIGNAL(data(QByteArray)), this, SLOT(dataReadyRead(QByteArray)));

        req->storeBody();
        file = new QFile("/tmp/upload-" + QString::number(this->clientId) + ".bin");
        file->open(QFile::ReadWrite);
        this->m_request = req;
        this->m_respnse = resp;

    } else if(req->method() == QHttpRequest::HTTP_GET) {
        resp->writeHead(200);
        resp->setHeader("Content-Type", "text/html");
        resp->end("<form name=\"myWebForm\" action=\"http://127.0.0.1:8008/upload\" method=\"post\">"
                  "<input type=\"checkbox\" /> Checkbox 1<br />"
                  "<input type=\"text\" /> Text Field 1<br />"
                  "<input type=\"submit\" value=\"SUBMIT\" />"
               "</form>");
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
