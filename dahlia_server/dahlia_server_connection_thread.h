#ifndef DAHLIA_SERVER_CONNECTION_THREAD_H
#define DAHLIA_SERVER_CONNECTION_THREAD_H

#include <QThread>
#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>
#include <QVariantList>
#include <QtSql/QtSql>
#include <QtSql>


class dahlia_server_connection_thread : public QThread
{
    Q_OBJECT
public:
    explicit dahlia_server_connection_thread(QObject *parent = 0);

    void run();

    //Config
    void setSocketDescriptor(qintptr);
    
private:
    /** Vars **/
        //Client capabilities
            int capabilities_text;

        //Networking
            qintptr clientSocketDescriptor;

        //Sqlite database(s)
            QSqlDatabase global_identities_db;

    /** Funcs **/
    QString respondToClientsCommand(QByteArray*);

        /*** CLIENT API ***/
        QString clientapi_req_handshake();
        QString clientapi_req_client_details(QJsonObject);
        QString clientapi_req_relay_text(QJsonObject);

signals:
    
public slots:
    
};

#endif // DAHLIA_SERVER_CONNECTION_THREAD_H
