/*
 * Use this class for interacting with Riot's API.
 * TODO: Move this stuff into a downloader class, and use this for api calls.
 */

#ifndef LOL_API_H
#define LOL_API_H

#include <QObject>
#include <QUrl>
#include <QDebug>
#include <QMap>
#include "downloadmanager.h"

class lol_api : public QObject
{
    Q_OBJECT
public:
    explicit lol_api(QObject *parent = 0);
    ~lol_api();

    // API CALL METHODS
    QByteArray getStaticData();

signals:

public slots:

private:
    static QMap<QString, qint32> mmQueues;
};

#endif // LOL_API_H
