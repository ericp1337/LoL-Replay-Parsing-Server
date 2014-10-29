#include <QCoreApplication>
#include "httpserver.h"

int main(int argc, char *argv[])
{
    int port = 8008;

    QCoreApplication a(argc, argv);
    HttpServer *server = new HttpServer(port);

    if(server->start()) {
        printf("Server is listening on port: %i\n", port);
        return a.exec();
    } else {
        printf("Error binding server to port: %i\n", port);
        return 1;
    }
}
