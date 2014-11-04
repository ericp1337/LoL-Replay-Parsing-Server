#include "handleget.h"

HandleGet::HandleGet(QObject *parent) :
    QObject(parent)
{
}

HandleGet::HandleGet(QHttpRequest *request, QHttpResponse *response) : m_request(request), m_response(response)
{
    connect(m_request, SIGNAL(data(QByteArray)), this, SLOT(dataReadyRead(QByteArray)));
    connect(m_request, SIGNAL(end()), this, SLOT(requestDone()));
    connect(m_response, SIGNAL(done()), this, SLOT(deleteLater()));
}

HandleGet::~HandleGet()
{
    delete m_request;
    m_request = 0;
}

void HandleGet::dataReadyRead(QByteArray data)
{
    this->m_buffer += QString(data);
    this->m_response->writeHead(100);
}

void HandleGet::requestDone()
{
    qDebug() << this->m_request->path();
    QJsonDocument json = QJsonDocument::fromJson("{\"title\": \"Home - Index\"}");

    QRegExp getReplay("^/replay/download/([0-9]+)");
    QRegExp getReplayInfo("^/replay/info/([0-9]+)");

    if(this->m_request->path().compare("/") == 0) {
        this->m_response->writeHead(200);
        this->m_response->write(json.toJson());
        this->m_response->end();

    } else if(getReplay.exactMatch(this->m_request->path())) {
        // this will send a replay file back to the client if it is found
        // otherwise we should send a 404 if we cannot find it.
        QFile replay("replays/" + getReplay.cap(1) + ".lrf");
        if(replay.exists()) {
            replay.open(QIODevice::ReadOnly);
            this->m_response->setHeader("Content-Type", "application/octet-stream; charset=utf-8");
            this->m_response->setHeader("Content-Disposition", "application/octet-stream; filename=\"" + getReplay.cap(1).toUtf8() + ".lrf\"");
            this->m_response->setHeader("Content-Length", QString::number(replay.size()));
            this->m_response->writeHead(200);

            QByteArray buffer;
            buffer.resize(1024);
            while(!replay.atEnd()) {
                replay.read(buffer.data(), buffer.size());
                this->m_response->write(buffer);
            }
            replay.close();

        } else {
            this->m_response->writeHead(404);
            this->m_response->end("replay does not exist.");
        }

    } else if(getReplayInfo.exactMatch(this->m_request->path())) {
        this->m_response->writeHead(200);
        this->m_response->end(json.toJson());

    }else {
        this->m_response->writeHead(404);
        this->m_response->end();
    }
}
