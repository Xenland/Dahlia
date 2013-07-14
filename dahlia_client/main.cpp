#include "dahlia_client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Start client.
    dahlia_client client;
    
    return a.exec();
}
