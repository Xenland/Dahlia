#include "dahlia_server.h"

dahlia_server::dahlia_server(QWidget *parent)
    : QWidget(parent)
{
    //Start TCP Server Management
    server_management = new dahlia_server_management(0);
    server_management->listen(QHostAddress("0.0.0.0"), 55460);

    //Report if running and the server details
    if(server_management->isListening()){
        qDebug() << "SERVER IS RUNNING";
    }else{
        qDebug() << "SERVER FAILED TO START for the following error: " << server_management->serverError();
    }


    qDebug() << "SERVER ADDRESS: " << server_management->serverAddress();
    qDebug() << "SERVER PORT: " << server_management->serverPort();
}

dahlia_server::~dahlia_server()
{
 //Stop TCP server management
 server_management->close();
}
