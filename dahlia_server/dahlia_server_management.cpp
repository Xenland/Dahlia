#include "dahlia_server_management.h"

dahlia_server_management::dahlia_server_management(QObject *parent) :
    QTcpServer(parent)
{
}

void dahlia_server_management::incomingConnection(qintptr socketDescriptor){
    //Create a thread for this connection.
    dahlia_server_connection_thread * thread = new dahlia_server_connection_thread();
        //Configure this thread/connection/client
        thread->setSocketDescriptor(socketDescriptor);

    //Begin proccessing networking commands with this client
    thread->start();
}
