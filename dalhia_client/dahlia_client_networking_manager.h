#ifndef DAHLIA_CLIENT_NETWORKING_MANAGER_H
#define DAHLIA_CLIENT_NETWORKING_MANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTcpSocket>
#include <QMutex>
#include <QMutexLocker>

#include <stdio.h>
#include <string.h>

class dahlia_client_networking_manager : public QObject
{
    Q_OBJECT
public:
    explicit dahlia_client_networking_manager(QObject *parent = 0);

    /** Functions **/
    void restartWithNewClient(QString, int);

private:
    /** Vars **/
        //Mutex
        QMutex incoming_proccess_mutex;

        //Networking
        QTcpSocket * client_socket;

            //Incoming command stack (saves queue of incoming commands from server)
            QVariantList incoming_cmds_to_proccess;

    /** Functions **/

signals:
        void new_incoming_command_to_proccess();
    
public slots:
    
private slots:
    void incomingDataReady();
    void proccess_incoming_commands();

};

#endif // DAHLIA_CLIENT_NETWORKING_MANAGER_H
