#include "httpserver.h"

using namespace std;

HttpServer::HttpServer(QObject *parent) : QObject(parent), m_port(8000)
{
}

HttpServer::HttpServer(int port, QObject *parent) : m_port(port), QObject(parent)
{
    sqlDatabase = new QSqlDatabase();
    sqlDatabase->addDatabase("QSQLITE");
    sqlDatabase->setDatabaseName("db.sqlite");
    qDebug() << sqlDatabase->open();
}

HttpServer::~HttpServer()
{
    sqlDatabase->close();
    delete sqlDatabase;
}

bool HttpServer::start()
{
    QHttpServer *httpServer = new QHttpServer(this);
    connect(httpServer, SIGNAL(newRequest(QHttpRequest*,QHttpResponse*)), this, SLOT(newRequest(QHttpRequest*,QHttpResponse*)));

    return httpServer->listen(this->m_port);
}

void HttpServer::newRequest(QHttpRequest *req, QHttpResponse *resp)
{
    if(req->path() == "/upload" && req->method() == QHttpRequest::HTTP_PUT) {
        new HandlePut(req, resp);
    } else if(req->method() == QHttpRequest::HTTP_GET) {
        new HandleGet(req, resp);
    } else {
        resp->writeHead(501);
        resp->end("This request is not supported/implemented.");
    }
}
