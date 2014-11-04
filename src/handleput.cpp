#include "handleput.h"

int nint(double x) // provided by koahnig at qt-project.org
{// avoids binary rounding error in (int) conversion
    return x < 0 ? (int)ceil(x) : (int)floor(x);
}


HandlePut::HandlePut(QObject *parent) :
    QObject(parent)
{
}

HandlePut::HandlePut(QHttpRequest *request, QHttpResponse *response) : m_request(request), m_response(response), m_totalDataSize(0)
{
    //m_request->storeBody();
    connect(m_request, SIGNAL(end()), this, SLOT(requestDone()));
    connect(m_request, SIGNAL(data(QByteArray)), this, SLOT(dataReadyRead(QByteArray)));
    connect(m_response, SIGNAL(done()), this, SLOT(deleteLater()));


    qDebug() << "Client connected...processing replay file.";

    // create tmp file until we parse the file and now the match id,
    // so we can then store the file using its game id.

    this->m_filename = QString::number(this->m_request->remotePort());

    if(QDir("replays").exists()) {
        this->m_file = new QFile("replays/" + this->m_filename);
        this->m_file->open(QIODevice::ReadWrite);
    } else
        QDir().mkdir("replays");

    //tell client to continue with its request
    //this->m_response->writeHead(100);
}

HandlePut::~HandlePut()
{
    delete m_request;
    delete m_file;
}

void HandlePut::requestDone()
{
    //acknowledge that we received all of the data from the client.
    this->m_response->writeHead(200);
    this->m_response->end("replay can be found at: " + this->m_request->header("host").toUtf8());

    this->parseLrf();
}

void HandlePut::dataReadyRead(QByteArray data)
{
    this->m_file->write(data);
}

// Parses the LRF file used by LoL Recorder (http://leaguereplays.com)
QJsonDocument HandlePut::parseLrf()
{
    this->m_file->seek(0);
    int version, metadataLength;

    QDataStream streamReader(this->m_file);
    //Qt defaults to BigEndian, so we need to change to little endian to get the correct byte order.
    streamReader.setByteOrder(QDataStream::LittleEndian);

    streamReader >> version;
    streamReader >> metadataLength;

    //move because the header is exactly 8 bytes, and the json array always will start at 8.
    this->m_file->seek(8);

    QJsonDocument metaData = QJsonDocument::fromJson(this->m_file->read(metadataLength));

    //matchID is a double, convert to int to make it actually useful.
    int matchID =  nint(metaData.object().value("matchID").toDouble());

    //copy file to permanant storage so we can send it to a client if they request it.
    if(!QFile("replays/" + QString::number(matchID) + ".lrf").exists())
        this->m_file->copy("replays/" + QString::number(matchID) + ".lrf");

    this->m_file->close();
    this->m_file->remove();

    // Save json to seperate file so we can use it later if needed instead of having to waste time parsing again.
    QFile replay_json("replays/" + QString::number(matchID) + ".json");
    replay_json.open(QIODevice::WriteOnly);
    replay_json.write(metaData.toJson());
    replay_json.close();

    return metaData;
}
