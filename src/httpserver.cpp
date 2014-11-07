#include "httpserver.h"

using namespace std;

HttpServer::HttpServer(QObject *parent) : QObject(parent), m_port(8000)
{
}

HttpServer::HttpServer(int port, QObject *parent) : m_port(port), QObject(parent)
{
}

HttpServer::~HttpServer()
{
}

bool HttpServer::start()
{
    this->m_threadPool = QThreadPool::globalInstance();
    this->m_threadPool->setMaxThreadCount(25);

    QHttpServer *httpServer = new QHttpServer(this);
    connect(httpServer, SIGNAL(newRequest(QHttpRequest*,QHttpResponse*)), this, SLOT(newRequest(QHttpRequest*,QHttpResponse*)));

    return httpServer->listen(this->m_port);
}

void HttpServer::newRequest(QHttpRequest *req, QHttpResponse *resp)
{
    if(req->path() == "/upload" && req->method() == QHttpRequest::HTTP_PUT) {
        HandlePut *putRequest = new HandlePut(req, resp);

        qDebug() << this->m_threadPool->activeThreadCount();
        // Qt will manage this thread for us.
        QThreadPool::globalInstance()->start(putRequest);
        // new HandlePut(req, resp);
        qDebug() << this->m_threadPool->activeThreadCount();


    } else if(req->method() == QHttpRequest::HTTP_GET) {
        new HandleGet(req, resp);

    } else if(req->method() == QHttpRequest::HTTP_POST) {
        new HandlePost(req, resp);

    } else {
        resp->writeHead(501);
        resp->end("This request is not supported/implemented.");
    }
}
