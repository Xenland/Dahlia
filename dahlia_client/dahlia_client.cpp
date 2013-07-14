#include "dahlia_client.h"

using namespace Botan;

dahlia_client::dahlia_client(QWidget *parent)
    : QWidget(parent)
{
    //Init loaded identity handle string
        currently_loaded_identity = "";

    //Init memory trackers
        //Dahlia Jungle
        dahlia_jungle_already_init = 0;

            //Add new chat to recent.
            dahlia_jungle_new_contact_dialog_already_init = 0;

        //New Identity
        new_identity_generation_success_already_init = 0;

        //Load Identity
        load_identity_pick_identity_already_init = 0;

        //Connection start options
        connection_start_options_already_init = 0;

    //Init Net-Manager
        network_manager = new dahlia_client_networking_manager(0);
            //Make connections
            connect(network_manager, SIGNAL(connected_to_dahlia_server()), this, SLOT(connected_to_a_dahlia_server_slot()));

    //Init DB
    identity_db = QSqlDatabase::addDatabase("QSQLITE", "identityDb");
    identity_db.setDatabaseName("./db/keyring");
    qDebug() << identity_db.lastError();
    bool db_opened = identity_db.open();
    qDebug() << "DB OPENED " << db_opened;

    addressbook_db = QSqlDatabase::addDatabase("QSQLITE", "addressbookDb");
    addressbook_db.setDatabaseName("./db/address_book");
    qDebug() << addressbook_db.lastError();
    db_opened = addressbook_db.open();
    qDebug() << "DB OPENED ADDRESS BOOK" << db_opened;

    //Init
    boot_screen = new QWidget(0);
    boot_screen->setWindowTitle("Welcome to Dahlia");
    boot_screen_layout = new QGridLayout(boot_screen);

        //Dahlia Header Image (Logo place holder)
        logo_image = QImage(":/img/dahlia_logo");
        logo_image_holder = new QLabel(0);
        logo_image_holder->setPixmap(QPixmap::fromImage(logo_image));
            //Attach to grid/show
            boot_screen_layout->addWidget(logo_image_holder, 0,0, 1,1, Qt::AlignHCenter);

        //Currently loaded
        currently_loaded_identity_label = new QLabel("<h3>Current Identity</h3>\nNone Loaded");
        currently_loaded_identity_label->setAlignment(Qt::AlignCenter);
            //Attach to grid/show
            boot_screen_layout->addWidget(currently_loaded_identity_label, 1,0, 1,1, Qt::AlignHCenter);

        //New Identity
        new_identity_btn = new QPushButton("New Dahlia Identity", 0);
            //Configure style
            new_identity_btn->setStyleSheet("QPushButton{padding:1em;}");

            //Attach to grid/show
            boot_screen_layout->addWidget(new_identity_btn, 2,0, 1,1, Qt::AlignHCenter);

            //Connections
                //Click
                connect(new_identity_btn, SIGNAL(clicked()), this, SLOT(boot_new_identity_btn_clicked()));

        //Load Identity
        load_identity_btn = new QPushButton("Load Dahlia Identity", 0);
            //Configure style
            load_identity_btn->setStyleSheet("QPushButton{padding:1em;}");

            //Attach to grid/show
            boot_screen_layout->addWidget(load_identity_btn, 3,0, 1,1, Qt::AlignHCenter);

            //Connections
                //Click
                connect(load_identity_btn, SIGNAL(clicked()), this, SLOT(boot_load_identity_btn_clicked()));


        //Connect to Dahlia Network
        connect_to_dahlia_jungle_btn = new QPushButton("Connect to Dahlia Jungle");
        connect_to_dahlia_jungle_btn->setDisabled(1);
        connect_to_dahlia_jungle_btn->setStyleSheet("QPushButton{padding:1em;}");
        boot_screen_layout->addWidget(connect_to_dahlia_jungle_btn, 4,0, 1,1, Qt::AlignHCenter);

            //Connections
                //Click
                connect(connect_to_dahlia_jungle_btn, SIGNAL(clicked()), this, SLOT(connect_to_dahlia_jungle()));

    //Show boot screen.
    boot_screen->show();
    boot_screen->resize(300, 350);
}

dahlia_client::~dahlia_client()
{
    
}



void dahlia_client::dahlia_jungle_show(){
    if(dahlia_jungle_already_init == 0){
        dahlia_jungle_screen = new QWidget(0);
        dahlia_jungle_screen->setWindowTitle("Dahlia Jungle");
        dahlia_jungle_layout = new QGridLayout(dahlia_jungle_screen);

            /** FIRST ROW **/
                /*** COLUMN one (takes up all columns except the last column in the row(s))***/
                dahlia_jungle_chat_history_tableview = new QTableView(0);
                dahlia_jungle_chat_history_tableview_itemmodel = new QStandardItemModel(0,1,0);
                dahlia_jungle_chat_history_tableview_itemmodel->setHorizontalHeaderItem(0, new QStandardItem(QString("Chat History")));
                dahlia_jungle_chat_history_tableview->setModel(dahlia_jungle_chat_history_tableview_itemmodel);

                dahlia_jungle_layout->addWidget(dahlia_jungle_chat_history_tableview, 0,0, 2,1);

                /*** COLUMN two ***/
                dahlia_jungle_logo_image = QImage(":/img/dahlia_logo");
                dahlia_jungle_logo_image_holder = new QLabel(0);
                dahlia_jungle_logo_image_holder->setPixmap(QPixmap::fromImage(logo_image));

                dahlia_jungle_layout->addWidget(dahlia_jungle_logo_image_holder, 0,1, 1,1);

            /** SECOND ROW **/
                /*** COLUMN two (goes down two rows) ***/
                dahlia_jungle_recip_recent_history_tableview = new QTableView(0);
                    //Connections
                connect(dahlia_jungle_recip_recent_history_tableview, SIGNAL(activated(QModelIndex)), this, SLOT(show_chat_history_by_addressbook_db_id(QModelIndex)));
                dahlia_jungle_recip_recent_history_tableview->hideColumn(3);
                dahlia_jungle_recip_recent_history_tableview->setMaximumWidth(250);
                dahlia_jungle_recip_recent_history_tableview_itemmodel = new QStandardItemModel(0,3,0);
                dahlia_jungle_recip_recent_history_tableview_itemmodel->setHorizontalHeaderItem(0, new QStandardItem(QString("Status")));
                dahlia_jungle_recip_recent_history_tableview_itemmodel->setHorizontalHeaderItem(1, new QStandardItem(QString("Contact Name")));
                dahlia_jungle_recip_recent_history_tableview_itemmodel->setHorizontalHeaderItem(2, new QStandardItem(QString("addressbook_db_id")));

                dahlia_jungle_recip_recent_history_tableview->setModel(dahlia_jungle_recip_recent_history_tableview_itemmodel);

                dahlia_jungle_layout->addWidget(dahlia_jungle_recip_recent_history_tableview, 1,1, 1,1);

            /** THIRD ROW **/
                /*** COLUMN one ***/
                dahlia_jungle_send_msg_container = new QWidget(0);
                dahlia_jungle_send_msg_vboxlayout = new QVBoxLayout(0);
                dahlia_jungle_send_msg_container->setLayout(dahlia_jungle_send_msg_vboxlayout);

                    dahlia_jungle_send_msg_input = new QPlainTextEdit("", 0);
                    dahlia_jungle_send_msg_vboxlayout->addWidget(dahlia_jungle_send_msg_input);

                    dahlia_jungle_send_msg_btn = new QPushButton("Send Message", 0);
                    dahlia_jungle_send_msg_vboxlayout->addWidget(dahlia_jungle_send_msg_btn);

                dahlia_jungle_layout->addWidget(dahlia_jungle_send_msg_container);

                /*** COLUMN TWO ***/
                dahlia_jungle_options_container = new QWidget(0);
                dahlia_jungle_options_vboxlayout = new QVBoxLayout(0);
                dahlia_jungle_options_container->setLayout(dahlia_jungle_options_vboxlayout);

                    dahlia_jungle_options_new_chat_btn = new QPushButton("Connect to contact", 0);
                        //Connections
                        connect(dahlia_jungle_options_new_chat_btn, SIGNAL(clicked()), this, SLOT(open_start_new_chat_dialog()));

                    dahlia_jungle_options_vboxlayout->addWidget(dahlia_jungle_options_new_chat_btn);

                dahlia_jungle_layout->addWidget(dahlia_jungle_options_container);


            /** Flag already init **/
            dahlia_jungle_already_init = 1;

        //Show
        dahlia_jungle_screen->show();

    }else{
        //Show
        dahlia_jungle_screen->show();
    }

    //Hide boot screen
    boot_screen->hide();

    //resize
    dahlia_jungle_screen->resize(700,500);
}


void dahlia_client::start_new_chat_dialog(){
    //Check if new chat dialog already init.
    if(dahlia_jungle_new_contact_dialog_already_init == 0){
        //Init, then show.
        dahlia_jungle_add_new_contact_to_chat = new QDialog(0);
        dahlia_jungle_add_new_contact_to_chat_layout = new QGridLayout(0);
        dahlia_jungle_add_new_contact_to_chat->setLayout(dahlia_jungle_add_new_contact_to_chat_layout);

            //Header
            dahlia_jungle_add_new_contact_to_chat_header = new QLabel("<h3>Paste your contacts' public key or MD5 Handle</h3>", 0);
            dahlia_jungle_add_new_contact_to_chat_layout->addWidget(dahlia_jungle_add_new_contact_to_chat_header);

            //Display name
            dahlia_jungle_add_new_contact_to_chat_displayname_input = new QLineEdit(0);
            dahlia_jungle_add_new_contact_to_chat_layout->addWidget(dahlia_jungle_add_new_contact_to_chat_displayname_input);

            //textbox
            dahlia_jungle_add_new_contact_to_chat_input = new QPlainTextEdit(0);
            dahlia_jungle_add_new_contact_to_chat_layout->addWidget(dahlia_jungle_add_new_contact_to_chat_input);

            //Add to recent contacts (if md5, ask server if they can find the matching public key to this md5 handle)
            dahlia_jungle_add_new_contact_to_chat_addbtn = new QPushButton("Connect to contact", 0);
                //connections
            connect(dahlia_jungle_add_new_contact_to_chat_addbtn, SIGNAL(clicked()), this, SLOT(attempt_to_connect_to_contact_slot()));
            dahlia_jungle_add_new_contact_to_chat_layout->addWidget(dahlia_jungle_add_new_contact_to_chat_addbtn);

         /** Flag already init **/
         dahlia_jungle_new_contact_dialog_already_init = 1;

        //Show
        dahlia_jungle_add_new_contact_to_chat->show();

    }else{
        //Show
        dahlia_jungle_add_new_contact_to_chat->show();
    }
}


void dahlia_client::open_chat_with_local_id(int addressbook_id){
    //Get information on this addressbook_id so we can open/start the chat.
    QString display_name = QString("");
    QString md5_handle = QString("");
    QString pubkey = QString("");

    QSqlQuery contact_info(addressbook_db);
    contact_info.exec(QString("SELECT `display_name`, `md5_handle`, `pubkey` FROM `recip_clients` WHERE `id` = '%1' LIMIT 0,1").arg(addressbook_id));
    if(contact_info.first()){
        display_name = contact_info.value("display_name").toString();
        md5_handle = contact_info.value("md5_handle").toString();
        pubkey = contact_info.value("pubkey").toString();
    }

    QString display_contact_as = QString("");
    if(display_name != ""){
        display_contact_as = display_name;
    }else{
        display_contact_as = md5_handle;
    }

    //Place extracted data into the table view
    QStandardItem * col_one = new QStandardItem(QString("Offline"));
    QStandardItem * col_two = new QStandardItem(display_contact_as);
    QStandardItem * col_three = new QStandardItem(QString(addressbook_id));

    int row_to_insert_on = dahlia_jungle_recip_recent_history_tableview_itemmodel->rowCount();
    dahlia_jungle_recip_recent_history_tableview_itemmodel->setItem(row_to_insert_on, 0, col_one);
    dahlia_jungle_recip_recent_history_tableview_itemmodel->setItem(row_to_insert_on, 1, col_two);
    dahlia_jungle_recip_recent_history_tableview_itemmodel->setItem(row_to_insert_on, 2, col_three);
}


/********************* SLOTS *********************/
void dahlia_client::connected_to_a_dahlia_server_slot(){
    //Init, and show the "Dahlia Jungle" screen (if already init and showing, then refresh the connection details)
    dahlia_jungle_show();
}

void dahlia_client::open_start_new_chat_dialog(){
    start_new_chat_dialog();
}

void dahlia_client::attempt_to_connect_to_contact_slot(){
    //If this public key isn't found locally add it; open the chat, if this is a md5 handle, ask the server for the public key, save it, then open the chat.
            QString displayname_to_pubkey = dahlia_jungle_add_new_contact_to_chat_displayname_input->text();

            QString public_key_input_stripped_down = dahlia_jungle_add_new_contact_to_chat_input->toPlainText();
            public_key_input_stripped_down.replace(QString("\n"), QString(""));
            public_key_input_stripped_down.replace(QString("-----BEGIN PUBLIC KEY-----"), QString(""));
            public_key_input_stripped_down.replace(QString("-----END PUBLIC KEY-----"), QString(""));
            qDebug() << public_key_input_stripped_down;

            //MD5 handle
            QByteArray public_key_input_stripped_down_256_handle = QCryptographicHash::hash(public_key_input_stripped_down.toUtf8(), QCryptographicHash::Sha256);
            QString public_key_input_stripped_down_md5_handle = QString(QCryptographicHash::hash(public_key_input_stripped_down_256_handle, QCryptographicHash::Md5).toHex()).toUtf8();
            qDebug() << "MD5 --- " << public_key_input_stripped_down_md5_handle;


        //Public key in addressbook?
        QSqlQuery public_key_exists(addressbook_db);
        public_key_exists.exec(QString("SELECT `id` FROM `recip_clients` WHERE `pubkey` = '%1' LIMIT 0,1").arg(public_key_input_stripped_down));

        if(public_key_exists.next()){
            int public_key_local_id_int = public_key_exists.value(0).toInt();

            //Open chat with this local id int
            open_chat_with_local_id(public_key_local_id_int);
        }else{
            //Add this public key to the address book, then open chat with this local id int
                //Add pubkey to addressbook
            QSqlQuery add_public_key(addressbook_db);
            add_public_key.exec(QString("INSERT INTO `recip_clients` (`id`, `display_name`, `pubkey`, `md5_handle`) VALUES(NULL, '%1', '%2', '%3')").arg(displayname_to_pubkey).arg(public_key_input_stripped_down).arg(public_key_input_stripped_down_md5_handle));
            qDebug() << "INSERT ID ERROR: " << add_public_key.lastError();
            add_public_key.exec(QString("SELECT last_insert_rowid()"));
            qDebug() << "INSERT ID CONTACT ADDRRD BOOK " << add_public_key.lastError();
            add_public_key.first();
            int public_key_local_id_int = add_public_key.value(0).toInt();

            qDebug() << "OPENING CHAT WITH CONTACT ID: " << public_key_local_id_int;

            //Open chat with this local id int
            open_chat_with_local_id(public_key_local_id_int);
        }
}


void dahlia_client::show_chat_history_by_addressbook_db_id(QModelIndex cell){
    /** Attempt to get the addressbook_db_id from the selected
     ** cells' row.
     **/

    QModelIndex addressbook_db_id_cell = dahlia_jungle_recip_recent_history_tableview_itemmodel->index(cell.row(), 3, QModelIndex());
    int addressbook_db_id = addressbook_db_id_cell.data().toInt();

    //Set chat history browser to this addressbook_db_id
        //TODO

    //Load chat history to this reciepient.
        //TODO
}


void dahlia_client::boot_new_identity_btn_clicked(){
    int generation_successfull = 0;

    /** Generate Keys **/
    std::string pub = "";
    std::string priv = "";
    try{

        LibraryInitializer init;

        AutoSeeded_RNG rng;

        RSA_PrivateKey key(rng, 4096);

        pub = X509::PEM_encode(key);

        priv = PKCS8::PEM_encode(key);

        generation_successfull = 1;
    }catch(std::exception &e){
        qDebug() << e.what();
    }


    /** Open Dialog that will tell the user their new identity information (if any) **/
    if(generation_successfull == 1){
        //Save information to the database
            //Generate a Md5_handle
                //Strip the Begin/end tags from the public key
                QString stripped_pub_key  = QString::fromStdString(pub);
                stripped_pub_key.replace(QString("-----BEGIN PUBLIC KEY-----"), QString(""));
                stripped_pub_key.replace(QString("-----END PUBLIC KEY-----"), QString(""));
                stripped_pub_key.replace(QString("\n"), QString(""));

                //Sha256 stripped pub key
                QByteArray sha256_pub_key_handle = QCryptographicHash::hash(stripped_pub_key.toUtf8(), QCryptographicHash::Sha256);
                qDebug() << sha256_pub_key_handle.toHex();

                //Md5 the sha256 handle (Convience handle)
                QByteArray md5_pub_key_handle = QCryptographicHash::hash(sha256_pub_key_handle, QCryptographicHash::Md5);

                //Convert md5 hex to Sqlite format string
                QString md5_pub_key_handle_string = md5_pub_key_handle.toHex();

        QSqlQuery insert_blank_row(identity_db);
        insert_blank_row.exec(QString("INSERT INTO `identity` (`id`,`privatekey`,`publickey`,`md5_handle`) VALUES(NULL,'%1','%2','%3')").arg(QString::fromStdString(priv)).arg(QString::fromStdString(pub)).arg(md5_pub_key_handle_string));

        //Open dialog successfull message
        if(new_identity_generation_success_already_init == 0){
            new_identity_gen_success_dialog = new QDialog(0);
            new_identity_gen_success_dialog->setModal(1);
                //Configure dialog/window
                new_identity_gen_success_dialog->setWindowTitle("New Identity | Dahlia");
            new_identity_gen_success_layout = new QGridLayout(new_identity_gen_success_dialog);
                //Header (new identity)
                new_identity_gen_success_header = new QLabel("<h3>Here is your new Identity details!</h3>");
                new_identity_gen_success_layout->addWidget(new_identity_gen_success_header, 0,1, 1,1, Qt::AlignHCenter);

                //Md5 handle
                new_identity_gen_success_md5_handle = new QLabel(md5_pub_key_handle_string);
                new_identity_gen_success_layout->addWidget(new_identity_gen_success_md5_handle, 1,1, 1,1, Qt::AlignHCenter);

            //Show new identity generation success
                new_identity_gen_success_dialog->show();

            /** Flag already init **/
            new_identity_generation_success_already_init = 1;
        }else{
            //Set md5 handle
            new_identity_gen_success_md5_handle->setText(md5_pub_key_handle_string);

            //Show new identity generation success
            new_identity_gen_success_dialog->show();

        }
    }else{

    }
}


void dahlia_client::boot_load_identity_btn_clicked(){
    if(load_identity_pick_identity_already_init == 0){
        load_identity_pick_identity_dialog = new QDialog(0);
            //Configure window
            load_identity_pick_identity_dialog->setWindowTitle("Load Identity | Dahlia");

        load_identity_pick_identity_layout = new QGridLayout(load_identity_pick_identity_dialog);
            //Header
            load_identity_pick_identity_header = new QLabel("<h3>Load Identity</h3>");
            load_identity_pick_identity_layout->addWidget(load_identity_pick_identity_header, 0,0, 1,1, Qt::AlignRight);

            /** ROW TWO **/
            //TableView
            load_identity_pick_identity_tableview_itemmodel = new QStandardItemModel(0,2,0);
                 load_identity_pick_identity_tableview_itemmodel->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
                 load_identity_pick_identity_tableview_itemmodel->setHorizontalHeaderItem(1, new QStandardItem(QString("Global Identfier (MD5 Format)")));

            load_identity_pick_identity_tableview = new QTableView(0);
            load_identity_pick_identity_tableview->resizeColumnsToContents();
            load_identity_pick_identity_tableview->setModel(load_identity_pick_identity_tableview_itemmodel);
            load_identity_pick_identity_layout->addWidget(load_identity_pick_identity_tableview, 1,0, 1,1);

            /** ROW THREE **/
            //Load identity
            load_identity_activate_load_btn = new QPushButton("Load Selected Identity");
            load_identity_activate_load_btn->setStyleSheet("QPushButton{padding:1em;}");
            load_identity_pick_identity_layout->addWidget(load_identity_activate_load_btn, 2,0, 1,1, Qt::AlignHCenter);
                //Connect button to "click" event
                connect(load_identity_activate_load_btn, SIGNAL(clicked()), this, SLOT(load_identity_dialog_load_ident_btn()));


        //Show
        load_identity_pick_identity_dialog->show();

        /** Flag Already Init **/
        load_identity_pick_identity_already_init = 1;


    }else{
        //Show dialog
        load_identity_pick_identity_dialog->show();
    }

    //Resize
    load_identity_pick_identity_dialog->resize(500,400);


    /** Refresh Tableview/Model Data **/
        //Query the local identities list from the database
        QSqlQuery local_identities_list(identity_db);
        local_identities_list.exec(QString("SELECT `display_name`,`md5_handle` FROM `identity`"));
        qDebug() << "DB QUERY LAST ERROR: " << local_identities_list.lastError();
        //Add Rows of data to the backend of the table view (QStandardItemModel)
        int row_index = 0;
        while(local_identities_list.next()){
            QString display_name = local_identities_list.value(0).toString();
            QString local_identity_md5_id = local_identities_list.value(1).toString();

            //Place extracted data into the table view
            QStandardItem * col_one = new QStandardItem(display_name);
            QStandardItem * col_two = new QStandardItem(local_identity_md5_id);

            load_identity_pick_identity_tableview_itemmodel->setItem(row_index, 0, col_one);
            load_identity_pick_identity_tableview_itemmodel->setItem(row_index, 1, col_two);

            //Increment row_index
            row_index +=1;
        }

        //Resize columns by content
        load_identity_pick_identity_tableview->resizeColumnsToContents();
}


void dahlia_client::load_identity_dialog_load_ident_btn(){
    //Ask the tableview what is currently selected
    QModelIndexList selected_indexes = load_identity_pick_identity_tableview->selectionModel()->selectedIndexes();
    int total_selected = selected_indexes.size();
    if(total_selected > 0){
        //Load the first selected one.
        QModelIndex data_row_model = selected_indexes.at(0);
        QModelIndex identity_id_handle_model = load_identity_pick_identity_tableview_itemmodel->index(data_row_model.row(), 1, QModelIndex());
        QVariant identity_id_handle_variant = load_identity_pick_identity_tableview_itemmodel->data(identity_id_handle_model);

        currently_loaded_identity = identity_id_handle_variant.toString();

        //Update currently loaded identity visual
        currently_loaded_identity_label->setText("<h3>Current Identity</h3>\n"+currently_loaded_identity);
        //Close load identity
        load_identity_pick_identity_dialog->hide();

        //Enable "connect" button
        connect_to_dahlia_jungle_btn->setEnabled(1);
    }

}

/**
 * @brief dahlia_client::connect_to_dahlia_jungle
 * @info Connecting is actually just signalling the networking manager to start connecting to relay nodes,
 *          and to visually display chat rooms, contact lists, etc
 **/
void dahlia_client::connect_to_dahlia_jungle(){
    network_manager->restartWithNewClient("0.0.0.0", 55460);
}
