#include "handleput.h"

HandlePut::HandlePut(QObject *parent) :
    QObject(parent)
{
}

HandlePut::HandlePut(QHttpRequest *request, QHttpResponse *response) : m_request(request), m_response(response)
{
    connect(m_request, SIGNAL(end()), this, SLOT(requestDone()));
    connect(m_response, SIGNAL(done()), this, SLOT(deleteLater()));
}

HandlePut::~HandlePut()
{
    delete m_request;
}

void HandlePut::requestDone()
{
    if(!this->m_request->body().startsWith("RIOT")) {
        this->m_response->writeHead(403);
        this->m_response->end("403 Forbidden, This is not a League of Legends replay file.");
    }

    this->m_response->writeHead(200);
    this->m_response->end();
}
