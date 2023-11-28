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

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QList>
#include <QMap>
#include <QMutex>

#include "log.h"
#include "json.h"
#include "global.h"
#include "registers-group.h"


/**
@brief Device.
*/
class Device : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit Device(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Device();


    /**
    Public constants
    */

    /**
    @brief Fields
    */
    static const QString FIELD__ID;
    static const QString FIELD__CLASS;
    static const QString FIELD__BASE_ADDR;
    static const QString FIELD__IP;
    static const QString FIELD__PORT;
    static const QString FIELD__ALLOW;
    static const QString FIELD__REGISTERS;
    static const QString FIELD__REG_BLOCK_TABLE;
    static const QString FIELD__REG_BLOCK_FIRST;
    static const QString FIELD__REG_BLOCK_SIZE;
    static const QString FIELD__CHECKSUM;
    static const QString FIELD__WAIT_READ;
    static const QString FIELD__RECONNECT;

    //** archive
    static const QString FIELD__ARH_TABLE;
    static const QString FIELD__ARH_FILE;

    /**
    @brief Permission codes.
    */
    static const quint8 DENY  = 0;
    static const quint8 ALLOW = 1;

    /**
    @brief Classes.
    */
    static const QString CLASS__DUMMY;
    static const QString CLASS__I7041;

    /**
    @brief Communication protocols
    */
    static const QString PROTO_COMM__SERIAL;
    static const QString PROTO_COMM__ETH;
    static const QString PROTO_COMM__DUMMY;


    /**
    Public options
    */

    /**
    @brief ID.
    */
    quint16 mID;

    /**
    @brief Class.
    */
    QString mClass;

    /**
    @brief Base address.
    */
    quint16 mBaseAddr;

    /**
    @brief IP-address or hostname (for Network::PROTO_COMM__ETH).
    */
    QString mIP;

    /**
    @brief The number of a network port (for Network::PROTO_COMM__ETH).
    */
    quint16 mPort;

    /**
    @brief Path to a file that contains list of registers (JSON).
    */
    QString mFileRegisters;

    /**
    @brief Permission to use the device.
    */
    bool mAllow;

    /**
    @brief Permission to use checksum (for Network::PROTO_DATA__DCON).
    */
    bool mChecksum;

    /**
    @brief Reconnect to port after read/write group of registers
           for Network::PROTO_DATA__MODBUS_RTU
    */
    bool mReconnect;

    /**
    @brief The waiting time for ReadyRead-signal (for Network::PROTO_DATA__DCON).
    @detailed -1 is no timeout
              1000 msec by default
    */
    qint16 mWaitRead;

    /**
    @brief The name of storage table (DB).
    */
    QString mArhTable;

    /**
    @brief The name of storage file.
    */
    QString mArhFile;

    /**
    @brief Network Communication protocol.
    */
    QString mNetProtoComm;

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
    @brief  Pack data to String buffer.
    @param  StringIn - link to string buffer.
    @return None.
    */
    void toString(QString &StringIn);

    /**
    @brief  Pack data to JSON buffer.
    @param  ObjIn - link to JsonObject.
    @return None.
    @detailed { "ID":mID, "Class":mClass, "BaseAddr":mBaseAddr, ..., Register.mVar:Register.mValue }
    */
    void toJson(QJsonObject &ObjIn);

    /**
    @brief  Pack values to JSON string.
    @param  StringIn - link to string buffer
    @return None.
    */
    void toJsonString(QString &StringIn);

    /**
    @brief  Pack value of registers into String-buffer in SQL-format.
    @param  ProfileIn - name of profile;
    @param  EventsIn - true for event values, false for current values;
    @param  ListStringsIn - link to list of string buffers.
    @return The number of packed values.
    @detailed Output SQL for mArhFile:
                [0] = "INSERT INTO `{mArhFile}`"
                [1] = "(`stamp`, `profile`, `device_id`, `register_id`, `value`, `ex`, `err`, `sign`)"
                [2] = " VALUES "
                [3] = "('{Register[0].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[0].mID}, {Register[0].FormattedValue}, {Register[0].mExLast}, {Register[0].mErrLast}, {Register[0].mSignLast})"
                [4] = "('{Register[1].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[1].mID}, {Register[1].FormattedValue}, {Register[1].mExLast}, {Register[1].mErrLast}, {Register[1].mSignLast})"
                 ...
                [n] = "('{Register[n].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[n].mID}, {Register[n].FormattedValue}, {Register[n].mExLast}, {Register[n].mErrLast}, {Register[n].mSignLast})"
              Output SQL for mArhFile:
                [0] = "INSERT INTO `{mArhTable}`"
                [1] = "(`stamp`, `profile`, `device_id`, `register_id`, `value`, `ex`, `err`, `sign`)"
                [2] = " VALUES "
                [3] = "('{Register[0].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[0].mID}, {Register[0].FormattedValue}, {Register[0].mExLast}, {Register[0].mErrLast}, {Register[0].mSignLast})"
                [4] = "('{Register[1].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[1].mID}, {Register[1].FormattedValue}, {Register[1].mExLast}, {Register[1].mErrLast}, {Register[1].mSignLast})"
                 ...
                [n] = "('{Register[n].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[n].mID}, {Register[n].FormattedValue}, {Register[n].mExLast}, {Register[n].mErrLast}, {Register[n].mSignLast})"
              Required: mArhTable or mArhFile
    */
    int toSql(const QString &ProfileIn, bool EventsIn, QList<QString> &ListStringsIn);

    /**
    @brief  Pack current value of registers into String-buffer in SQL-format.
    @param  ProfileIn - name of profile;
    @param  ListStringsIn - link to list of string buffers.
    @return The number of packed values.
    @detailed Output SQL for mArhFile:
                [0] = "INSERT INTO `{mArhFile}`"
                [1] = "(`stamp`, `profile`, `device_id`, `register_id`, `value`, `ex`, `err`, `sign`)"
                [2] = " VALUES "
                [3] = "('{Register[0].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[0].mID}, {Register[0].FormattedValue}, {Register[0].mExLast}, {Register[0].mErrLast}, {Register[0].mSignLast})"
                [4] = "('{Register[1].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[1].mID}, {Register[1].FormattedValue}, {Register[1].mExLast}, {Register[1].mErrLast}, {Register[1].mSignLast})"
                 ...
                [n] = "('{Register[n].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[n].mID}, {Register[n].FormattedValue}, {Register[n].mExLast}, {Register[n].mErrLast}, {Register[n].mSignLast})"
              Output SQL for mArhFile:
                [0] = "INSERT INTO `{mArhTable}`"
                [1] = "(`stamp`, `profile`, `device_id`, `register_id`, `value`, `ex`, `err`, `sign`)"
                [2] = " VALUES "
                [3] = "('{Register[0].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[0].mID}, {Register[0].FormattedValue}, {Register[0].mExLast}, {Register[0].mErrLast}, {Register[0].mSignLast})"
                [4] = "('{Register[1].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[1].mID}, {Register[1].FormattedValue}, {Register[1].mExLast}, {Register[1].mErrLast}, {Register[1].mSignLast})"
                 ...
                [n] = "('{Register[n].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[n].mID}, {Register[n].FormattedValue}, {Register[n].mExLast}, {Register[n].mErrLast}, {Register[n].mSignLast})"
              Required: mArhTable or mArhFile
    */
    int toSqlCurrent(const QString &ProfileIn, QList<QString> &ListStringsIn);

    /**
    @brief  Pack event value of registers into String-buffer in SQL-format.
    @param  ProfileIn - name of profile;
    @param  ListStringsIn - link to list of string buffers.
    @return The number of packed values.
    @detailed Output SQL for mArhFile:
                [0] = "INSERT INTO `{mArhFile}`"
                [1] = "(`stamp`, `profile`, `device_id`, `register_id`, `value`, `ex`, `err`, `sign`)"
                [2] = " VALUES "
                [3] = "('{Register[0].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[0].mID}, {Register[0].Event.FormattedValue}, {Register[0].Event.mEx}, {Register[0].Event.mErr}, {Register[0].Event.mSign})"
                [4] = "('{Register[1].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[1].mID}, {Register[1].Event.FormattedValue}, {Register[1].Event.mEx}, {Register[1].Event.mErr}, {Register[1].Event.mSign})"
                 ...
                [n] = "('{Register[n].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[n].mID}, {Register[n].Event.FormattedValue}, {Register[n].Event.mEx}, {Register[n].Event.mErr}, {Register[n].Event.mSign})"
              Output SQL for mArhFile:
                [0] = "INSERT INTO `{mArhTable}`"
                [1] = "(`stamp`, `profile`, `device_id`, `register_id`, `value`, `ex`, `err`, `sign`)"
                [2] = " VALUES "
                [3] = "('{Register[0].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[0].mID}, {Register[0].Event.FormattedValue}, {Register[0].Event.mEx}, {Register[0].Event.mErr}, {Register[0].Event.mSign})"
                [4] = "('{Register[1].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[1].mID}, {Register[1].Event.FormattedValue}, {Register[1].Event.mEx}, {Register[1].Event.mErr}, {Register[1].Event.mSign})"
                 ...
                [n] = "('{Register[n].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[n].mID}, {Register[n].Event.FormattedValue}, {Register[n].Event.mEx}, {Register[n].Event.mErr}, {Register[n].Event.mSign})"
              Required: mArhTable or mArhFile
    */
    int toSqlEvent(const QString &ProfileIn, QList<QString> &ListStringsIn);

    /**
    @brief  Merge list of string buffers into one SQL.
    @param  ListStringsIn - link to list of string buffers;
    @param  StringIn - string buffer.
    @return The number of merged values.
    @details Items 0, 1, 2 must be header values (INSERT, field names, VALUES)
    */
    static int mergeSql(const QList<QString> &ListStringsIn, QString &StringIn);

    /**
    @brief  Check option "BaseAddr".
    @param  None.
    @return True if value of option "BaseAddr" is correct, otherwise - False.
    */
    bool isCorrectBaseAddr();

    /**
    @brief  Check option "IP" (for Network::PROTO_COMM__ETH).
    @param  None.
    @return True if value of option "IP" is correct, otherwise - False.
    */
    bool isCorrectIP();

    /**
    @brief  Check option "Port" (for Network::PROTO_COMM__ETH).
    @param  None.
    @return True if value of option "Port" is correct, otherwise - False.
    */
    bool isCorrectPort();

    /**
    @brief  Check general options.
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
    @brief  Read list of registers from a file.
    @param  FileIn - path to a file.
    @return The number of registers that had been reading.
    */
    quint16 readFileRegisters(const QString &FileIn);

    /**
    @brief  Clear list of registers.
    @param  None.
    @return None.
    */
    void clearListRegisters();

    /**
    @brief  Get size of list of registers.
    @param  None.
    @return Size of list of registers.
    */
    quint16 sizeListRegisters();

    /**
    @brief  Get size of list of registers groups.
    @param  None.
    @return Size of list of registers groups.
    */
    quint16 sizeListRegsGroups();

    /**
    @brief  Check size of list of registers.
    @param  None.
    @return True if list of registers is empty, otherwise - False.
    */
    bool isListRegistersEmpty();

    /**
    @brief  Read randomized data.
    @param  None.
    @return The number of registers that had been reading.
    */
    quint16 randomize();

    /**
    @brief  Read registers by Serial.
    @param  SerialPortIn - the name (full path) of serial port,
    @param         SpdIn - the speed of serial port,
    @param        PrtyIn - the parity of serial port,
    @param    DataBitsIn - the number of data bits of serial port,
    @param    StopBitsIn - the number of stop bits of serial port,
    @param        ModeIn - the mode of serial port.
    @return The number of registers that had been reading.
    */
    virtual quint16 readRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn, const QString &ModeIn);

    /**
    @brief  Read registers by Serial.
    @param  SerialPortIn - the name (full path) of serial port,
    @param         SpdIn - the speed of serial port,
    @param        PrtyIn - the parity of serial port,
    @param    DataBitsIn - the number of data bits of serial port,
    @param    StopBitsIn - the number of stop bits of serial port,
    @return The number of registers that had been reading.
    */
    virtual quint16 readRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn);

    /**
    @brief  Read registers.
    @param  None.
    @return The number of registers that had been reading.
    */
    virtual quint16 readRegisters();

    /**
    @brief  Write registers by Serial.
    @param  SerialPortIn - the name (full path) of serial port,
    @param  SpdIn - the speed of serial port,
    @param  PrtyIn - the parity of serial port,
    @param  DataBitsIn - the number of data bits of serial port,
    @param  StopBitsIn - the number of stop bits of serial port,
    @param  ModeIn - the mode of serial port.
    @return The number of registers that had been written.
    */
    virtual quint16 writeRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn, const QString &ModeIn);

    /**
    @brief  Write registers.
    @param  SerialPortIn - the name (full path) of serial port,
    @param  SpdIn - the speed of serial port,
    @param  PrtyIn - the parity of serial port,
    @param  DataBitsIn - the number of data bits of serial port,
    @param  StopBitsIn - the number of stop bits of serial port,
    @param  ModeIn - the mode of serial port.
    @param  ObjIn - link to JsonObject.
    @return The number of registers that had been written.
    @detailed ObjIn = { Var:Value, ... }
    */
    virtual quint16 writeRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn, const QString &ModeIn, QJsonObject &ObjIn);

    /**
    @brief  Write registers.
    @param  None.
    @return The number of registers that had been written.
    */
    virtual quint16 writeRegisters();

    /**
    @brief  Write registers.
    @param  ObjIn - link to JsonObject (associative array of values).
    @return The number of registers that had been written.
    @detailed { RegVar:RegValue, ... }
    */
    virtual quint16 writeRegisters(QJsonObject &ObjIn);

    /**
    @brief  Read Dummy-registers.
    @param  None.
    @return The number of registers that had been reading.
    */
    quint16 readDummyRegisters();


protected:

    /**
    Private options
    */

    /**
    @brief List of registers groups.
    */
    QList<RegsGroup *> mListRegsGroups;

    /**
    @brief Mutex on registers.
    */
    QMutex mMutex;


    /**
    Private methods
    */

    /**
    @brief  Parse data of list registers.
    @param  DocIn - link to JsonDocument.
    @return The number of registers that had been parsing.
    */
    quint16 parseDataRegisters(const QJsonDocument &DocIn);

    /**
    @brief  Log of registers (raw).
    @param  RawDataIn - pointer to array of raw-data.
    @return None.
    */
    void logRawRegisters(const quint16 *RawDataIn);

    /**
    @brief  Get registers.
    @param  ListIDsIn - list of ID of target registers.
    @return List of registers.
    */
    QList<Register *> getRegisters(const QList<quint16> &ListIDsIn);

    /**
    @brief  Get values.
    @param  ListIDsIn - list of ID of target registers.
    @return List of raw-values.
    */
    QList<quint16 *> getValues(const QList<quint16> &ListIDsIn);

    /**
    @brief  Get registers.
    @param  ClassIn - class of target registers.
    @return List of registers.
    */
    QList<Register *> getRegisters(const QString ClassIn);

    /**
    @brief  Init. calculated registers.
    @param  None.
    @return None.
    */
    void initCalcRegisters();
};

#endif // DEVICE_H
