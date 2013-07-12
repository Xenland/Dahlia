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
        //Networking
            qintptr clientSocketDescriptor;

        //Sqlite database(s)
            QSqlDatabase global_identities_db;

    /** Funcs **/
    QString respondToClientsCommand(QByteArray * client_response);

signals:
    
public slots:
    
};

#endif // DAHLIA_SERVER_CONNECTION_THREAD_H
