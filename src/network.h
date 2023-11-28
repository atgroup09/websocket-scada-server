/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  This file is part of the HMI server component.
*
*  The code in this page is free software: you can
*  redistribute it and/or modify it under the terms of the GNU
*  General Public License (GNU GPL) as published by the Free Software
*  Foundation, either version 3 of the License, or (at your option)
*  any later version.  The code is distributed WITHOUT ANY WARRANTY;
*  without even the implied warranty of MERCHANTABILITY or FITNESS
*  FOR A PARTICULAR PURPOSE.  See the GNU GPL for more details.
*
*  As additional permission under GNU GPL version 3 section 7, you
*  may distribute non-source (e.g., minimized or compacted) forms of
*  that code without the copy of the GNU GPL normally required by
*  section 4, provided you include this license notice and a URL
*  through which recipients can access the Corresponding Source.
*/

#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

#include "log.h"
#include "json.h"
#include "device.h"
#include "device-modbus-rtu.h"
#include "device-modbus-tcp.h"
#include "device-dcon7000.h"


/**
@brief Configuration of network.
*/
class Network : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit Network(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Network();


    /**
    Public constants
    */

    /**
    @brief Fields
    */
    static const QString FIELD__ID;
    static const QString FIELD__PROTO_COMM;
    static const QString FIELD__PROTO_DATA;
    static const QString FIELD__PORT_PREF;
    static const QString FIELD__PORT;
    static const QString FIELD__PORT_DEV;
    static const QString FIELD__SPD;
    static const QString FIELD__PRTY;
    static const QString FIELD__DATA_BITS;
    static const QString FIELD__STOP_BITS;
    static const QString FIELD__MODE;
    static const QString FIELD__ALLOW;
    static const QString FIELD__DEVICES;

    /**
    @brief Communication protocols
    */
    static const QString PROTO_COMM__SERIAL;
    static const QString PROTO_COMM__ETH;
    static const QString PROTO_COMM__DUMMY;

    /**
    @brief Data transfer protocol
    */
    static const QString PROTO_DATA__MODBUS_RTU;
    static const QString PROTO_DATA__MODBUS_TCP;
    static const QString PROTO_DATA__DCON;

    /**
    @brief Serial port :: Baud rates
    */
    static const quint32 SERIAL_SPD__2400   = 2400;
    static const quint32 SERIAL_SPD__4800   = 4800;
    static const quint32 SERIAL_SPD__9600   = 9600;
    static const quint32 SERIAL_SPD__14400  = 14400;
    static const quint32 SERIAL_SPD__19200  = 19200;
    static const quint32 SERIAL_SPD__28800  = 28800;
    static const quint32 SERIAL_SPD__38800  = 38800;
    static const quint32 SERIAL_SPD__57600  = 57600;
    static const quint32 SERIAL_SPD__115200 = 115200;

    /**
    @brief Serial port :: The parities
    */
    static const QString SERIAL_PRTY__NONE;
    static const QString SERIAL_PRTY__EVEN;
    static const QString SERIAL_PRTY__ODD;

    /**
    @brief Serial port :: Data bits
    */
    static const quint8 SERIAL_DATA_BITS__7 = 7;
    static const quint8 SERIAL_DATA_BITS__8 = 8;

    /**
    @brief Serial port :: Stop bits
    */
    static const quint8 SERIAL_STOP_BITS__1 = 1;
    static const quint8 SERIAL_STOP_BITS__2 = 2;

    /**
    @brief Serial port :: The modes
    */
    static const QString SERIAL_MODE__RS232;
    static const QString SERIAL_MODE__RS485;

    /**
    @brief Permission codes
    */
    static const quint8 DENY  = 0;
    static const quint8 ALLOW = 1;


    /**
    Public options
    */

    /**
    @brief ID.
    */
    quint16 mID;

    /**
    @brief Communication protocol.
    */
    QString mProtoComm;

    /**
    @brief Data transfer protocol.
    */
    QString mProtoData;

    /**
    @brief Serial port :: Number.
    @details from 0 ...
    */
    quint16 mSerialPort;

    /**
    @brief Serial port :: Prefix of name (full path).
    @details For Windows it's "COM" ... ("\\\\.\\COM" if mSerialPort > 9)
             For Linux it's "/dev/ttyUSB" or "/dev/ttyS"
    */
    QString mSerialPortPref;

    /**
    @brief Serial port :: Name (full path).
    @details For Windows it's "COM1" ("\\\\.\\COM10" if mSerialPort > 9)
             For Linux it's "/dev/ttyUSB0" or "/dev/ttyS0"
             If the options is set, then mSerialPort and mSerialPortPref will not be used!
    */
    QString mSerialPortDev;

    /**
    @brief Serial port :: Baudrate.
    */
    quint32 mSerialSpd;

    /**
    @brief Serial port :: Parity.
    */
    QString mSerialPrty;

    /**
    @brief Serial port :: Number of data bits.
    */
    quint8 mSerialDataBits;

    /**
    @brief Serial port :: Number of stop bits.
    */
    quint8 mSerialStopBits;

    /**
    @brief Serial port :: Mode.
    */
    QString mSerialMode;

    /**
    @brief Permission to use the device.
    */
    bool mAllow;

    /**
    @brief Path to a file that contains list of devices (JSON).
    */
    QString mFileDevices;

    /**
    @brief Use log.
    */
    bool mUseLog;

    /**
    @brief Path to a file when will be write service data.
    */
    QString mFileLog;


    /**
    Public methods
    */

    /**
    @brief  Set values by default.
    @param  None.
    @return None.
    */
    void byDefault();

    /**
    @brief  Load values from JsonObject.
    @param  DataIn - data.
    @return True if value of options are correct, otherwise - False.
    */
    bool fromJson(const QJsonObject &DataIn);

    /**
    @brief  Pack values to String buffer.
    @param  StringIn - link to string buffer.
    @return None.
    */
    void toString(QString &StringIn);

    /**
    @brief  Pack data to JSON buffer.
    @param  ObjIn - link to JsonObject.
    @return None.
    @detailed { "ID":mID, "Devices": [ {...}, ... ] }
    */
    void toJson(QJsonObject &ObjIn);

    /**
    @brief  Pack values to JSON string.
    @param  StringIn - link to string buffer
    @return None.
    */
    void toJsonString(QString &StringIn);

    /**
    @brief  Check option "ProtoComm".
    @param  None.
    @return True if value of option "ProtoComm" is correct, otherwise - False.
    */
    bool isCorrectProtoComm();

    /**
    @brief  Check option "ProtoData".
    @param  None.
    @return True if value of option "ProtoData" is correct, otherwise - False.
    */
    bool isCorrectProtoData();

    /**
    @brief  Check option "SerialPort".
    @param  None.
    @return True if value of option "SerialPort" is correct, otherwise - False.
    */
    bool isCorrectSerialPort();

    /**
    @brief  Check option "SerialSpd".
    @param  None.
    @return True if value of option "SerialSpd" is correct, otherwise - False.
    */
    bool isCorrectSerialSpd();

    /**
    @brief  Check option "SerialPrty".
    @param  None.
    @return True if value of option "SerialPrty" is correct, otherwise - False.
    */
    bool isCorrectSerialPrty();

    /**
    @brief  Check option "SerialDataBits".
    @param  None.
    @return True if value of option "SerialDataBits" is correct, otherwise - False.
    */
    bool isCorrectSerialDataBits();

    /**
    @brief  Check option "SerialStopBits".
    @param  None.
    @return True if value of option "SerialStopBits" is correct, otherwise - False.
    */
    bool isCorrectSerialStopBits();

    /**
    @brief  Check option "SerialMode".
    @param  None.
    @return True if value of option "SerialMode" is correct, otherwise - False.
    */
    bool isCorrectSerialMode();

    /**
    @brief  Check options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool isCorrect();

    /**
    @brief  Normilize options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool normilize();

    /**
    @brief  Read list of devices from a file.
    @param  FileIn - path to a file.
    @return The number of devices have read.
    */
    quint16 readFileDevices(const QString &FileIn);

    /**
    @brief  Clear list of devices.
    @param  None.
    @return None.
    */
    void clearListDevices();

    /**
    @brief  Get size of list of devices.
    @param  None.
    @return Size of list of devices.
    */
    quint16 sizeListDevices();

    /**
    @brief  Check size of list of devices.
    @param  None.
    @return True if list of devices is empty, otherwise - False.
    */
    bool isListDevicesEmpty();

    /**
    @brief  Get value of option `ArhFile` of a device.
    @param  IdxIn - index of list of devices (0...ListDevices.size()-1).
    @return Value of the option.
    */
    QString getDeviceArhFile(const int IdxIn);

    /**
    @brief  Get value of `ArhTable` of a device.
    @param  IdxIn - index of list of devices (0...ListDevices.size()-1).
    @return Value of the option.
    */
    QString getDeviceArhTable(const int IdxIn);

    /**
    @brief  Get SQL-package of a Device data.
    @param  ProfileIn - name of profile;
    @param  EventsIn - true for event values, false for current values;
    @param  IdxIn - index of list of devices (0...ListDevices.size()-1);
    @param  ListStringsIn - link to list of string buffers.
    @return The number of packed values.
    */
    int getDeviceSql(const QString &ProfileIn, bool EventsIn, const int IdxIn, QList<QString> &ListStringsIn);

    /**
    @brief  Start randomized surey.
    @param  None.
    @return True if the survey has completed, otherwise - False.
    */
    bool randomize();

    /**
    @brief  Start surey.
    @param  None.
    @return True if the survey has completed, otherwise - False.
    */
    bool survey();

    /**
    @brief  Write data.
    @param  DevID - Device ID.
    @param  ObjIn - link to JsonObject-data.
    @return True if the data has written, otherwise - False.
    @detailed ObjIn = { RegVar:RegValue, ... }
    */
    bool write(quint16 DevID, QJsonObject &ObjIn);


private:

    /**
    Private options
    */

    /**
    @brief List of devices.
    */
    QList<Device *> mListDevices;


    /**
    Private methods
    */

    /**
    @brief  Parse data of list devices.
    @param  DocIn - link to JsonDocument.
    @return The number of devices have parsed.
    */
    quint16 parseDataDevices(const QJsonDocument &DocIn);

    /**
    @brief  Start survey.
    @param  RandomIn - true if the survey is randomized.
    @return True if the survey has completed, otherwise - False.
    */
    bool survey(const bool RandomIn);
};

#endif // NETWORK_H
