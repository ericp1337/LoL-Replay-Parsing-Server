#include "lol_api.h"

// CONST VARIABLES
QMap<QString, qint32> lol_api::mmQueues;
// END CONST VARIABLES

lol_api::lol_api(QObject *parent)
{
    // If this is empty, please fill it so we can use it.
    // need to make this into a fetch from API, since it could be updated as they add more modes.
    if(lol_api::mmQueues.isEmpty()) {
        QFile file(":/content/mmQueues.json");
        if(file.open(QIODevice::ReadOnly)) {
        }
    }
}

lol_api::~lol_api()
{
}

QByteArray lol_api::getStaticData()
{
}
