#ifndef HANDLEPUT_H
#define HANDLEPUT_H

#include <QObject>
#include <qhttprequest.h>
#include <qhttpresponse.h>

class HandlePut : public QObject
{
    Q_OBJECT
public:
    explicit HandlePut(QObject *parent = 0);
    HandlePut(QHttpRequest *request, QHttpResponse *response);
    ~HandlePut();

signals:

public slots:

private slots:
    void requestDone();

private:
    QHttpRequest *m_request;
    QHttpResponse *m_response;

};

#endif // HANDLEPUT_H
