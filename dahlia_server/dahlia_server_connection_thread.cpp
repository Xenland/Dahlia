#include "dahlia_server_connection_thread.h"

dahlia_server_connection_thread::dahlia_server_connection_thread(QObject *parent) :
    QThread(parent)
{

    /** Database stuff **/
        //Global Identities
        global_identities_db = QSqlDatabase::addDatabase("QMYSQL", "global_identities");
        global_identities_db.setHostName("localhost");
        global_identities_db.setDatabaseName("dahlia_server");
        global_identities_db.setUserName("root"); //Don't use ROOT!
        global_identities_db.setPassword("fuckthat");
        qDebug() << global_identities_db.lastError();
        bool db_opened = global_identities_db.open();
        qDebug() << "DB OPENED " << db_opened;
}

void dahlia_server_connection_thread::setSocketDescriptor(qintptr descriptor){
    //set descriptor
    clientSocketDescriptor = descriptor;
}


void dahlia_server_connection_thread::run(){
    qDebug() << "running.....thread...";

    //Misc. vars
    QByteArray slash_n = QString("\n").toUtf8(); //Slash n (\n) in qbytearray format.

    //Open TCP socket handle to work with.
    QTcpSocket tcpSocket;

    if (!tcpSocket.setSocketDescriptor(clientSocketDescriptor)) {
        qDebug() << tcpSocket.errorString();
        return;
    }

    int hold_on_to_client = 1;
    while(hold_on_to_client == 1){
        qDebug() << "HOLDING CLIENT";
        //Wait for a command from the client for 5 seconds before disconnecting
        bool ready_to_read = tcpSocket.waitForReadyRead(5000);
        if(ready_to_read == true){
            //Wait for client request client request
            int waiting_4_response = 1;
            QByteArray * client_response = new QByteArray();
            while(waiting_4_response == 1){
                qDebug() << "waiting";
                if(tcpSocket.peek(1).size() > 0){
                    //Append to client response incomming message.
                    client_response->append(tcpSocket.read(1));

                    //Have we recieved the full command?
                    QByteArray last_two_char_bytearray = client_response->mid((client_response->size()-1),-1);
                    if(slash_n == last_two_char_bytearray){
                        waiting_4_response = 0;
                        qDebug() << "finished proccesing the networking part of this clients incomming request";
                    }
                }else{
                    waiting_4_response = 0;
                }
            }

            //Respond to client
            QString response_to_client = respondToClientsCommand(client_response);
            tcpSocket.write(response_to_client.toUtf8());
            qDebug() << response_to_client;

        }else{
            //Drop client (5 seconds is too long)
            hold_on_to_client = 0;
        }
    }
    qDebug() << "DISCONNECTING";
    //Close connection
    tcpSocket.disconnectFromHost();

}

QString dahlia_server_connection_thread::respondToClientsCommand(QByteArray * client_json_command){
    //Convert client command from a QByteArray to QString datatype.
    QString client_json_command_string = QString(client_json_command->data());
    qDebug() << "RECEIVED CLIENT COMMAND " << client_json_command_string;

    QJsonDocument client_json_command_doc = QJsonDocument::fromJson(client_json_command_string.toUtf8());
    QJsonObject client_json_cmd = client_json_command_doc.object();

    //What method/command/request is being called from this client?
    QJsonValue method = client_json_cmd.value("method");
    if(method.toString() == "handshake"){
        /** Client Requesting HANDSHAKE **
         ** 1] Ask for the client details
         **/
        return QString("{\"method\":\"requesting_client_details\"}");
    }else if(method.toString() == "client_details"){
        /** Client is Sending Client Details **
         ** 1]Check if we have this public key added,
         **     if no, add it to the database, ask client what they want to do now?
         **     if yes, ask client what they want to do now?
         **/


    }

    return QString("");
}
