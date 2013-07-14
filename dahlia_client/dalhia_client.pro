#-------------------------------------------------
#
# Project created by QtCreator 2013-07-10T14:30:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

TARGET = dalhia_client
TEMPLATE = app


SOURCES += main.cpp\
        dahlia_client.cpp \
    dahlia_client_networking_manager.cpp

HEADERS  += dahlia_client.h \
    dahlia_client_networking_manager.h

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    botan.pri


#Botan
INCLUDEPATH += "/usr/include/"
LIBS += -L"/usr/lib/" -lbotan -ldl
