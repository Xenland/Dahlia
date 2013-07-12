#ifndef DAHLIA_SERVER_H
#define DAHLIA_SERVER_H

#include <QWidget>
#include <QTcpServer>

#include "dahlia_server_management.h"

class dahlia_server : public QWidget
{
    Q_OBJECT
    
public:
    dahlia_server(QWidget *parent = 0);
    ~dahlia_server();

    dahlia_server_management * server_management;
};

#endif // DAHLIA_SERVER_H
