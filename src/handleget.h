#ifndef HANDLEGET_H
#define HANDLEGET_H

#include <QObject>
#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <QJsonDocument>
#include <QDebug>
#include <QRegExp>
#include <QFile>

class HandleGet : public QObject
{
    Q_OBJECT
public:
    explicit HandleGet(QObject *parent = 0);
    HandleGet(QHttpRequest *request, QHttpResponse *response);
    ~HandleGet();

signals:

public slots:

private slots:
    void dataReadyRead(QByteArray data);
    void requestDone();

private:
    QHttpRequest *m_request;
    QHttpResponse *m_response;

    QString m_buffer;

};

#endif // HANDLEGET_H
