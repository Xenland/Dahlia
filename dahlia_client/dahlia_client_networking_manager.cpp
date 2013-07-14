#include "dahlia_client_networking_manager.h"

dahlia_client_networking_manager::dahlia_client_networking_manager(QObject *parent) :
    QObject(parent)
{
    //Init network state
    client_network_state = 0; // 0 = Not connected; 1 = server says we are connected

    //Init Hide in plain sight switch
    hide_in_plain_sight_mode = 0;//0 = Not in hiding but not exposing(probubly more efficient but messages should be considered traceable); 1 = Hiding in plain sight as much as possible, sometimes exposed to attacks.

    //Init incoming command queue
    incoming_cmds_to_proccess = QVariantList();

    client_socket = new QTcpSocket(0);
    connect(client_socket, SIGNAL(readyRead()), this, SLOT(incomingDataReady()));


    //Class connections with it self
    connect(this, SIGNAL(new_incoming_command_to_proccess()), this, SLOT(proccess_incoming_commands()));

}


void dahlia_client_networking_manager::restartWithNewClient(QString address, int port){
    client_socket->connectToHost(address, port);

    //Wait for connected
    if(client_socket->waitForConnected()){
        qDebug() << "CONNECTED, SEND CLIENT INFORMATION";
        //Send client information in JSON format.
            //Generate JSON file

            //Start handshake.
            QString client_information_string = "{\"method\":\"handshake\"}";
            client_socket->write(client_information_string.toUtf8());

    }
}


void dahlia_client_networking_manager::incomingDataReady(){
    qDebug() << "APPENDING INCOMMING DATA";
    QByteArray server_response = client_socket->readAll();

    QJsonParseError json_response_doc_error = QJsonParseError();
    QJsonDocument json_response_doc = QJsonDocument::fromJson(server_response, &json_response_doc_error);
    qDebug() << "SERVER RESPONSE " << server_response;
    if(json_response_doc_error.error == 0){
        //Successfull JSON response from server, add to the proccessing queue.
        QVariantMap json_response = json_response_doc.object().toVariantMap();

        incoming_cmds_to_proccess.append(json_response);

        //Start the proccessor (if not already started or if idle continue)
        emit new_incoming_command_to_proccess();
    }else{
        //JSON parse error
        qDebug() << "JSON ERROR: "<< json_response_doc_error.errorString() << " ___ " << json_response_doc_error.error;
    }
}


void dahlia_client_networking_manager::proccess_incoming_commands(){
    QMutexLocker locker(&incoming_proccess_mutex);

    while(incoming_cmds_to_proccess.size() > 0){
        //Always proccess the first in command...
        QVariant json_response_asvariant = incoming_cmds_to_proccess.at(0);
        QVariantMap json_response = json_response_asvariant.toMap();

        //Proccessing (Simulated)
        qDebug() << json_response;
        if(json_response.value("method") == "requesting_client_details"){
            /**     REQUESTING CLIENT DETAILS   **
             **     -------------------------   **
             ** Submit the currently logged in
             ** identitys' public key.          **/
            QVariantMap client_details_response = QVariantMap();
            client_details_response.insert("method", QString("client_details"));

                //Send capabilities
                QStringList capabilities = QStringList();
                capabilities.append(QString("text"));
                client_details_response.insert("capabilities", capabilities);

                //Send pubkey (IF, not in "Hide in plainsight" pseudoanonymous mode)
                if(hide_in_plain_sight_mode == 0){
                    client_details_response.insert("pubkey", QString("pubkeyhere"));
                }

            //Convert to a write able format
            QJsonObject client_details_response_json = QJsonObject::fromVariantMap(client_details_response);

            QJsonDocument client_details_response_json_doc = QJsonDocument(client_details_response_json);

            QByteArray client_details_response_bytearray = client_details_response_json_doc.toJson();
                //Strip \n (that were created from the toJSON());
                client_details_response_bytearray.replace(QString("\n"), QString("").toUtf8());

            //client_details_response_bytearray.append(QString("\n"));
            qDebug() << "WRITING " << client_details_response_bytearray;
            //Write/send to server our client details.
            client_socket->write(client_details_response_bytearray);
        }else if(json_response.value("method") == "idle"){
            qDebug() << "IDLE";
            //If server says its idle -- we are connected, server is ready for commands.
            if(client_network_state == 0){
                client_network_state = 1;

                //Emit signal so the caller client can know when to display the "chat", "contacts" and effectivly know when to query for such things.
                emit connected_to_dahlia_server();
            }

        }else if(json_response.value("method") == "disconnecting"){
            qDebug() << "SERVER IS DISCONNECTING";
            client_network_state = 0; //Disconnected.
        }

        //remove the proccessed command from the list
        incoming_cmds_to_proccess.removeAt(0);
    }
}




void dahlia_client_networking_manager::setHideInPlainSight_mode(int mode){
    if(mode == 0){
        hide_in_plain_sight_mode = 0;
    }else{
        hide_in_plain_sight_mode = 1;
    }
}
