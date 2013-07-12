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
#include <QTableView>
#include <QPushButton>
#include <QGridLayout>
#include <QtSql/QtSql>
#include <QStandardItemModel>
#include <QCryptographicHash>

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

        //memory Trackers
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

private slots:
    /*** Boot Screen ***/
    void boot_new_identity_btn_clicked();
    void boot_load_identity_btn_clicked();
        //Load Identity Screen slots
        void load_identity_dialog_load_ident_btn();

        //Connect to Dahila Jungle options
        void connect_to_dahlia_jungle();
};

#endif // DAHLIA_CLIENT_H
