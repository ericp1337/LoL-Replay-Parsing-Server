#include "handler.h"

Handler::Handler(QObject *parent) :
    QObject(parent)
{
}

Handler::Handler(QHttpRequest *request, QHttpResponse *response)
{
    this->m_request = request;
    this->m_response = response;

    //need to store body so we can get all received data when it is done transferring
    request->storeBody();
    connect(request, SIGNAL(end()), this, SLOT(getRequestBody()));
    //connect(req, SIGNAL(end()), resp, SLOT(end()));
    connect(response, SIGNAL(done()), this, SLOT(deleteLater()));

    qDebug() << "[" << QDateTime::currentDateTimeUtc().toString() << "] " << "client connected";

    if(request->method() != QHttpRequest::HTTP_GET || request->method() != QHttpRequest::HTTP_PUT) {
        response->writeHead(501);
        response->end("501 not implemented");
    }
}

Handler::~Handler()
{
    delete m_request;
    m_request = 0;
}

void Handler::getRequestBody()
{
    if(this->m_request->body().startsWith("RIOT")) {
        qDebug() << "LoL Replay File Detected!\n Now Parsing...";
        //parse replay since we know it is a League file from the header
        this->parseReplay(this->m_request->body());
    }

    this->m_response->writeHead(200);
    this->m_response->write(QDateTime::currentDateTimeUtc().toString().toUtf8() + "\n");
    this->m_response->end("Goodbye ;)");
}

void Handler::parseReplay(QByteArray data)
{
}
