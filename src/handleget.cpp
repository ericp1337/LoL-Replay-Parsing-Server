#include "handleget.h"

HandleGet::HandleGet(QHttpRequest *request, QHttpResponse *response) : QObject(request), m_request(request), m_response(response)
{
    QObject::connect(request, &QHttpRequest::end, this, &HandleGet::requestDone);
    QObject::connect(request, &QHttpRequest::data, this, &HandleGet::dataReadyRead);
}

void HandleGet::dataReadyRead(QByteArray data)
{
}

void HandleGet::requestDone()
{
    qDebug() << this->m_request->url();
    QJsonDocument json = QJsonDocument::fromJson("{\"endpoints\":{\"Download Replay Info\":{\"url\":\"/replay/info/$match_id\",\"type\":\"GET\"},\"Download Replay File\":{\"url\":\"/replay/download/$match_id\",\"type\":\"GET\"},\"List Replays\":{\"url\":\"/list-replays\",\"type\":\"GET\"},\"Upload Lrf\":{\"url\":\"/upload-lrf\",\"type\":\"POST\"}},\"title\":\"Home - Available Endpoints\"}");

    QRegExp getReplay("^/replay/download/([0-9]+)");
    QRegExp getReplayInfo("^/replay/info/([0-9]+)");

    if(this->m_request->url().toString().compare("/") == 0) {
        this->m_response->setStatusCode(qhttp::ESTATUS_OK);
        this->m_response->addHeader("Content-Type", "application/json");
        this->m_response->write(json.toJson());
        this->m_response->end();

    } else if(getReplay.exactMatch(this->m_request->url().toString())) { // This will return the replay file (lrf)
        // this will send a replay file back to the client if it is found
        // otherwise we should send a 404 if we cannot find it.
        QFile replay("replays/" + getReplay.cap(1) + ".lrf");
        if(replay.exists()) {
            replay.open(QIODevice::ReadOnly);
            this->m_response->addHeader("Content-Type", "application/octet-stream; charset=utf-8");
            this->m_response->addHeader("Content-Disposition", "application/octet-stream; filename=\"" + getReplay.cap(1).toUtf8() + ".lrf\"");
            this->m_response->addHeader("Content-Length", QString::number(replay.size()).toUtf8());
            this->m_response->setStatusCode(qhttp::ESTATUS_OK);

            QByteArray buffer;
            buffer.resize(1024);
            while(!replay.atEnd()) {
                replay.read(buffer.data(), buffer.size());
                this->m_response->write(buffer);
            }
            replay.close();
            this->m_response->end();

        } else {
            this->m_response->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
            this->m_response->addHeader("Content-Type", "application/json");
            this->m_response->end(QJsonDocument::fromJson(QString("{\"errorString\":\"404 error, replay file not found!\"}").toUtf8()).toJson());
        }

    } else if(getReplayInfo.exactMatch(this->m_request->url().toString())) { // This will return the replay json
        // this will send the json of the replay to the client.
        QFile metaData("replays/" + getReplayInfo.cap(1) + ".json");
        if(metaData.exists()) {
            metaData.open(QIODevice::ReadOnly);
            json = QJsonDocument::fromJson(metaData.readAll());
            this->m_response->setStatusCode(qhttp::ESTATUS_OK);
            this->m_response->end(json.toJson());
        } else {
            this->m_response->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
            this->m_response->addHeader("Content-Type", "application/json");
            QString error = "{\"errorString\":\"Could not find replay info.\"}";
            this->m_response->end(QJsonDocument::fromJson(error.toUtf8()).toJson());
        }

    } else if(this->m_request->url().toString() == "/list-replays") { //this will return all of the available replays that are uploaded to the server
        QDir replays("replays");
        QStringList replayList = replays.entryList(QStringList("*.lrf"));

        QJsonDocument replaysAvailable = QJsonDocument();
        QJsonObject jsonObject;
        jsonObject["numResults"] = replayList.length();
        jsonObject["replays"] = QJsonArray::fromStringList(replayList);

        replaysAvailable.setObject(jsonObject);

        this->m_response->setStatusCode(qhttp::ESTATUS_OK);
        this->m_response->addHeader("Content-Type", "application/json");
        this->m_response->end(replaysAvailable.toJson());

    } else {
        this->m_response->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
        this->m_response->addHeader("Content-Type", "application/json");
        this->m_response->end(QJsonDocument::fromJson(QString("{\"errorString\":\"404 error, page not found!\"}").toUtf8()).toJson());
    }
}
