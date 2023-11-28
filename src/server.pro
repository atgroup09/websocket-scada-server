#-------------------------------------------------
#
# Project created by QtCreator 2018-10-16T11:44:15
#
#-------------------------------------------------

# Copyright (C) 2019-2022 ATgroup09
#  Contact: atgroup09@gmail.com
#
#  This file is part of the HMI server component.
#
#  The code in this page is free software: you can
#  redistribute it and/or modify it under the terms of the GNU
#  General Public License (GNU GPL) as published by the Free Software
#  Foundation, either version 3 of the License, or (at your option)
#  any later version.  The code is distributed WITHOUT ANY WARRANTY;
#  without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU GPL for more details.
#
#  As additional permission under GNU GPL version 3 section 7, you
#  may distribute non-source (e.g., minimized or compacted) forms of
#  that code without the copy of the GNU GPL normally required by
#  section 4, provided you include this license notice and a URL
#  through which recipients can access the Corresponding Source.
#

QT+= core websockets serialport
QT-= gui

TARGET = server
CONFIG+= console
CONFIG+= c++11
CONFIG-= app_bundle

TEMPLATE = app


SOURCES+= \
           main.cpp \
           log.cpp \
           args.cpp \
           bit.cpp \
           json.cpp \
           serialport.cpp \
           modbus-cli.cpp \
           modbus-rtu-cli.cpp \
           modbus-tcp-cli.cpp \
           dcon7000.cpp \
           mysql-cli.cpp \
           config.cpp \
           network.cpp \
           device.cpp \
           device-modbus-rtu.cpp \
           device-modbus-tcp.cpp \
           device-dcon7000.cpp \
           register.cpp \
           registers-group.cpp \
           event.cpp \
           client.cpp \
           server.cpp \
           service.cpp \
           arh.cpp

HEADERS+= \
           log.h \
           args.h \
           bit.h \
           json.h \
           serialport.h \
           modbus-cli.h \
           modbus-rtu-cli.h \
           modbus-tcp-cli.h \
           dcon7000.h \
           mysql-cli.h \
           global.h \
           config.h \
           network.h \
           device.h \
           device-modbus-rtu.h \
           device-modbus-tcp.h \
           device-dcon7000.h \
           register.h \
           registers-group.h \
           event.h \
           client.h \
           server.h \
           service.h \
           arh.h

# ModBus
# include files
INCLUDEPATH += lib/modbus/include
# shared libraries for platforms
CONFIG(debug, debug|release) {
# debug
    win32: LIBS += -L$$PWD/lib/modbus -llibmodbus-5
     unix: LIBS += -L$$PWD/lib/modbus -lmodbus
} else {
# release
    win32: LIBS += -L$$PWD/lib/modbus -llibmodbus-5
     unix: LIBS += -L$$PWD/lib/modbus -lmodbus
}

# MySQL
# include files
INCLUDEPATH += lib/mysql/include
# shared libraries for platforms
CONFIG(debug, debug|release) {
# debug
    win32: LIBS += -L$$PWD/lib/mysql -llibmysql
     unix: LIBS += -L$$PWD/lib/mysql -lmysqlclient
} else {
# release
    win32: LIBS += -L$$PWD/lib/mysql -llibmysql
     unix: LIBS += -L$$PWD/lib/mysql -lmysqlclient
}

# QtService
# include files
INCLUDEPATH += lib/qtservice/src
# shared libraries for platforms
CONFIG(debug, debug|release) {
# debug
    win32: LIBS += -L$$PWD/lib/qtservice -lQtSolutions_Service-headd
#   unix: LIBS += -L$$PWD/lib/qtservice -lQtSolutions_Service-head
} else {
# release
    win32: LIBS += -L$$PWD/lib/qtservice -lQtSolutions_Service-head
#   unix: LIBS += -L$$PWD/lib/qtservice -lQtSolutions_Service-head
}
