#ifndef DAHLIA_CLIENT_H
#define DAHLIA_CLIENT_H

//Qt Libs
#include <QtSql>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QDialog>
#include <QPixmap>
#include <QTcpSocket>
#include <QLineEdit>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPushButton>
#include <QGridLayout>
#include <QtSql/QtSql>
#include <QStandardItemModel>
#include <QCryptographicHash>
#include <QVBoxLayout>
#include <QPlainTextEdit>


//Botan Libs
#include <botan/botan.h>
#include <botan/rsa.h>
#include <botan/look_pk.h>

//Dahlia
#include "dahlia_client_networking_manager.h"

class dahlia_client : public QWidget
{
    Q_OBJECT
    
public:
    dahlia_client(QWidget *parent = 0);
    ~dahlia_client();

private:

    /** Vars **/
        //Currently loaded Identity (By Md5 handle)
        QString currently_loaded_identity;
        QString currently_loaded_identity_pubkey;

        //memory Trackers
                //Dahlia Jungle Screen
                int dahlia_jungle_already_init;

                    //Add new contact to recent chat area (dahlia jungle screen)
                    int dahlia_jungle_new_contact_dialog_already_init;

                //Boot Screen
                    //New Identity
                        //Generation success
                        int new_identity_generation_success_already_init;
                    //Load Identity
                        //Pick Identity from local keyring
                        int load_identity_pick_identity_already_init;

                    //Connection start options
                    int connection_start_options_already_init;

        //networking
            dahlia_client_networking_manager * network_manager;

        //Sqlite database(s)
            QSqlDatabase identity_db;
            QSqlDatabase addressbook_db;


        //Dahlia Jungle
            QWidget * dahlia_jungle_screen;
                QGridLayout * dahlia_jungle_layout;

                    //Row one
                        //Column one
                            //Chat history (of currently selected recip identity)
                            QStandardItemModel * dahlia_jungle_chat_history_tableview_itemmodel;
                            QTableView * dahlia_jungle_chat_history_tableview;

                        //Column two
                            //Dahlia logo
                            QImage dahlia_jungle_logo_image;
                            QLabel * dahlia_jungle_logo_image_holder;


                   //Row two
                       //Column two
                            //Recent Recip. chat list history;
                            QStandardItemModel * dahlia_jungle_recip_recent_history_tableview_itemmodel;
                            QTableView * dahlia_jungle_recip_recent_history_tableview;


                   //Row Three
                        //Column one
                            //VBox
                            QWidget * dahlia_jungle_send_msg_container;
                            QVBoxLayout * dahlia_jungle_send_msg_vboxlayout;
                                //Message input box
                                QPlainTextEdit * dahlia_jungle_send_msg_input;

                                //Send message
                                QPushButton * dahlia_jungle_send_msg_btn;

                        //Column two
                            //VBox
                            QWidget * dahlia_jungle_options_container;
                            QVBoxLayout * dahlia_jungle_options_vboxlayout;
                                //Open new chat
                                QPushButton * dahlia_jungle_options_new_chat_btn;

            //Add new contact to chat history
            QDialog * dahlia_jungle_add_new_contact_to_chat;
            QGridLayout * dahlia_jungle_add_new_contact_to_chat_layout;

                //Header
                QLabel * dahlia_jungle_add_new_contact_to_chat_header;

                //Displayname
                QLineEdit * dahlia_jungle_add_new_contact_to_chat_displayname_input;

                //Input (public key)
                QPlainTextEdit * dahlia_jungle_add_new_contact_to_chat_input;

                //Connect to contact
                QPushButton * dahlia_jungle_add_new_contact_to_chat_addbtn;

        //Boot Screen
            QWidget * boot_screen;
                QGridLayout * boot_screen_layout;

                /** ROW ONE OF BOOT SCREEN **/
                //Logo Image Header (Dahlia Logo)
                QImage logo_image;
                QLabel * logo_image_holder;


                /** ROW TWO OF BOOT SCREEN **/
                QLabel * currently_loaded_identity_label;

                /** ROW THREE OF BOOT SCREEN **/
                //New Identity
                QPushButton * new_identity_btn;
                    //Generation successfull screen
                        QDialog * new_identity_gen_success_dialog;
                            QGridLayout * new_identity_gen_success_layout;

                            //Generation successfull message header
                            QLabel * new_identity_gen_success_header;

                            //Md5 handle
                            QLabel * new_identity_gen_success_md5_handle;


                /** ROW FOUR OF BOOT SCREEN **/
                //Load Identity
                QPushButton * load_identity_btn;
                    //Pick an Identity to load(editing available too)
                        QDialog * load_identity_pick_identity_dialog;
                            QGridLayout * load_identity_pick_identity_layout;

                            //Header ("Edit or Load Identity")
                            QLabel * load_identity_pick_identity_header;

                            //Tableview (List of identites save in the local DB)
                            QStandardItemModel * load_identity_pick_identity_tableview_itemmodel;
                            QTableView * load_identity_pick_identity_tableview;

                            //Load Identity
                            QPushButton * load_identity_activate_load_btn;


                /** ROW FIVE OF BOOT SCREEN **/
                //Connect to a Dahlia Jungle
                QPushButton * connect_to_dahlia_jungle_btn;

        //Connection start settings
        QDialog * connection_start_options_dialog;
            QGridLayout * connection_start_options_gridlayout;

                //Header
                QLabel * connection_start_options_label;


    /** Functions **/
                void dahlia_jungle_show();

                //Add new contact to talk to.
                void start_new_chat_dialog();

                //Open new chat dialog box with this contact
                void open_chat_with_local_id(int);

private slots:

    /*** Background Networking Slots ***/
    void connected_to_a_dahlia_server_slot();

    /*** Dahlia Jungle Screen Slots ***/
    void open_start_new_chat_dialog();
    void attempt_to_connect_to_contact_slot();

    void show_chat_history_by_addressbook_db_id(QModelIndex);

    /*** Boot Screen ***/
    void boot_new_identity_btn_clicked();
    void boot_load_identity_btn_clicked();
        //Load Identity Screen slots
        void load_identity_dialog_load_ident_btn();

        //Connect to Dahila Jungle options
        void connect_to_dahlia_jungle();
};

#endif // DAHLIA_CLIENT_H
