#ifndef DAHLIA_SERVER_MANAGEMENT_H
#define DAHLIA_SERVER_MANAGEMENT_H

#include <QTcpServer>

#include "dahlia_server_connection_thread.h"

class dahlia_server_management : public QTcpServer
{
    Q_OBJECT
public:
    explicit dahlia_server_management(QObject *parent = 0);
    
signals:
    
public slots:

protected:
   void incomingConnection(qintptr socketDescriptor);
    
};

#endif // DAHLIA_SERVER_MANAGEMENT_H
