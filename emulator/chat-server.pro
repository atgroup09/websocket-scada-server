#-------------------------------------------------
#
# Project created by QtCreator 2018-10-12T11:44:15
#
#-------------------------------------------------

QT+= core websockets
QT-= gui

TARGET = chat-server
CONFIG+= console
CONFIG+= c++11
CONFIG-= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    log.cpp \
    args.cpp \
    config.cpp \
    client.cpp \
    server.cpp

HEADERS += \
    log.h \
    args.h \
    global.h \
    config.h \
    client.h \
    server.h
