#include "dahlia_server_connection_thread.h"

dahlia_server_connection_thread::dahlia_server_connection_thread(QObject *parent) :
    QThread(parent)
{
    /** Capabilities init **/
    capabilities_text = 0; // 0 = Unknown; 1 = Available; 2 = Not available.

    /** Database stuff **/
    qDebug() << QSqlDatabase::drivers();
        //Global Identities
        global_identities_db = QSqlDatabase::addDatabase("QPSQL", "dahlia_server");
        global_identities_db.setHostName("localhost");
        global_identities_db.setDatabaseName("dahlia_server");
        global_identities_db.setUserName("postgres"); //Don't use ROOT!
        global_identities_db.setPassword("happy");
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
        //Wait for a command from the client for 120 seconds before disconnecting
        bool ready_to_read = tcpSocket.waitForReadyRead(120000);
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

    //Tell client that we are disconnecting.
    tcpSocket.write(QString("{\"method\":\"disconnecting\"}").toUtf8());

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
        return clientapi_req_handshake();
    }else if(method.toString() == "client_details"){
        return clientapi_req_client_details(client_json_cmd);
    }else if(method.toString() == "ping"){
        return QString("{\"method\":\"idel\"}");
    }else if(method.toString() == "relay_text"){
        return clientapi_req_relay_text(client_json_cmd);
    }

    return QString("{\"method\":\"idle\", \"request_error\":\"command not found\"}");
}



/** ********** ********** ********** **
 ** ********** CLIENT API ********** **/

QString dahlia_server_connection_thread::clientapi_req_handshake(){
    /** Client Requesting HANDSHAKE **
     ** 1] Ask for the client details
     **/
    return QString("{\"method\":\"requesting_client_details\"}");
}


QString dahlia_server_connection_thread::clientapi_req_client_details(QJsonObject client_json_cmd){
    /** Client is Sending Client Details **
     ** 1]Check if we have this public key added,
     **     if no, add it to the database, ask client what they want to do now?
     **     if yes, ask client what they want to do now?
     **/
        //Get public key from teh clients connection
        QJsonValue public_key = client_json_cmd.value("pubkey");

    QSqlQuery client_has_account_query(global_identities_db);
    client_has_account_query.exec(QString("SELECT id FROM client.account WHERE public_key = '%1' LIMIT 1 OFFSET 1").arg(public_key.toString()));

    //If this client exists, ask the client what now?, if not exists, add it then ask what to do now?
    if(client_has_account_query.size() == 0){
        QSqlQuery add_account_query(global_identities_db);
        add_account_query.exec(QString("INSERT INTO client.account (public_key, md5_handle) VALUES ('%1', '%2')").arg(public_key.toString()).arg(QString(" ")));
        qDebug() << add_account_query.lastError();
        qDebug() << add_account_query.lastInsertId();
    }

    //Save in memory the capabilities of this client
        //Extract capabilities.
        QJsonValue capabilities_reported = client_json_cmd.value("capabilities");
        QJsonArray capabilities_reported_array = capabilities_reported.toArray();
        int total_capabilities = capabilities_reported_array.size();
        for(int a = 0; a < total_capabilities; a++){
            QJsonValue capability_extracted = capabilities_reported_array.at(a);
            //Text?
            if(capability_extracted.toString() == "text"){
                capabilities_text = 1;
            }

        }

    //Ask the client what do you want to do next? (Server is idle -- in the perspective of the client)
    return QString("{\"method\":\"idle\"}");
}

QString dahlia_server_connection_thread::clientapi_req_relay_text(QJsonObject client_json_cmd){
    //TODO.
}
