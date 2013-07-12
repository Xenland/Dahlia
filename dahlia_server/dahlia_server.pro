#-------------------------------------------------
#
# Project created by QtCreator 2013-07-09T21:14:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

TARGET = dahlia_server
TEMPLATE = app

INCLUDEPATH+=/usr/include/

SOURCES += main.cpp\
        dahlia_server.cpp \
    dahlia_server_management.cpp \
    dahlia_server_connection_thread.cpp

HEADERS  += dahlia_server.h \
    dahlia_server_management.h \
    dahlia_server_connection_thread.h
