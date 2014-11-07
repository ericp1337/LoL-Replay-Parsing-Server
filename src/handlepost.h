#ifndef HANDLEPOST_H
#define HANDLEPOST_H

#include <QObject>
#include <qhttprequest.h>
#include <qhttpresponse.h>

class HandlePost : public QObject
{
    Q_OBJECT
public:
    explicit HandlePost(QObject *parent = 0);
    HandlePost(QHttpRequest *request, QHttpResponse *response);

signals:

public slots:

};

#endif // HANDLEPOST_H
