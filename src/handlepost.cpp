#include "handlepost.h"

// This is used to convert from double to int
int nint(double x) // provided by koahnig at qt-project.org
{// avoids binary rounding error in (int) conversion
    return x < 0 ? (int)ceil(x) : (int)floor(x);
}

HandlePost::HandlePost(quint64 clientId, QHttpRequest *request, QHttpResponse *response) :
    QObject(request),
    m_request(request),
    m_response(response),
    m_clientConnectionId(clientId),
    m_error(false)
{
    QObject::connect(request, &QHttpRequest::end, this, &HandlePost::requestDone);
    QObject::connect(request, &QHttpRequest::data, this, &HandlePost::dataReadyRead);

    this->m_upload = new QFile("replays/" + QString::number(clientId) + ".lrf");
    if(!this->m_upload->open(QIODevice::ReadWrite)) {
        m_error = true;
        qWarning() << "HandlePost: could not open temp file for upload.";
        response->setStatusCode(qhttp::ESTATUS_SERVICE_UNAVAILABLE);
        response->end(QJsonDocument::fromJson(QByteArray("{\"errorString\":\"temporary file could not be opened for the upload on server, please try again later.\"}")).toJson());
    }
}

void HandlePost::requestDone()
{
    // only proceed after the entire request has been successfully received.
    if(!this->m_request->isSuccessful()) {
        return;
    }

    if(this->m_request->url().toString() != "/upload-lrf" || this->m_request->url().toString() != "/upload-rofl") {
        this->m_response->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
        this->m_response->end("{\"errorString\":\"The url you are uploading to is not valid.Please use: /upload-lrf or /upload-rofl.\"}");
        return;

    } else if(m_error) { // if there was an error creating the temp file for upload, then we will not be able to read from it, so we should exit.
        this->m_response->setStatusCode(qhttp::ESTATUS_INTERNAL_SERVER_ERROR);
        this->m_response->end("{\"errorString\":\"Internal server error. Please try again later.\"}");
        return;
    }

    if(this->m_request->url().toString() == "/upload-lrf") {
        parseLrf();

    } else if(this->m_request->url().toString() == "/upload-rofl") {
        //parseRofl();

    }
}

void HandlePost::dataReadyRead(QByteArray data)
{
    this->m_upload->write(data);
}

void HandlePost::parseLrf() {
    int version, metaDataLength;

    this->m_upload->seek(0);
    QDataStream streamReader(this->m_upload);
    streamReader.setByteOrder(QDataStream::LittleEndian);

    streamReader >> version;
    streamReader >> metaDataLength;

    // the beginning binary header will always be at the 8th byte.
    this->m_upload->seek(8);
    QJsonDocument json = QJsonDocument::fromJson(this->m_upload->read(metaDataLength));
    QFile replayInfo("replays/" + QString::number(nint(json.object()["matchID"].toDouble())) + ".json");
    if(replayInfo.open(QIODevice::WriteOnly)) {
        replayInfo.write(json.toJson());
        replayInfo.close();
    }

    json.object().insert("messageString", "replay was uploaded successfully.");

    this->m_upload->close();
    this->m_upload->copy("./replays/" + QString::number(nint(json.object()["matchID"].toDouble())) + ".lrf");
    this->m_upload->remove();
    this->m_upload->deleteLater();
    this->m_response->setStatusCode(qhttp::ESTATUS_OK);
    this->m_response->addHeader("Content-Length", QString::number(json.toJson().size()).toUtf8());
    this->m_response->end(json.toJson());
}

void HandlePost::parseRofl()
{
}
