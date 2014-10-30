/*
 * This class connects to the server using signals and slots to allow multiple clients to connect simultaneously.
 */

#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <QThread>
#include <QDebug>
#include <QDateTime>

class Handler : public QObject
{
    Q_OBJECT
public:
    /*
     * Constructor/Deconstructor
     */
    explicit Handler(QObject *parent = 0);
    Handler(QHttpRequest *request, QHttpResponse *response);
    ~Handler();

signals:

public slots:

private slots:
    void getRequestBody();

private:
    QHttpRequest *m_request;
    QHttpResponse *m_response;

    void parseReplay(QByteArray data);
};

#endif // HANDLER_H
