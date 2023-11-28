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

#include "device.h"

/**
@brief Fields
*/
const QString Device::FIELD__ID              = "ID";
const QString Device::FIELD__CLASS           = "Class";
const QString Device::FIELD__BASE_ADDR       = "BaseAddr";
const QString Device::FIELD__IP              = "IP";
const QString Device::FIELD__PORT            = "Port";
const QString Device::FIELD__ALLOW           = "Allow";
const QString Device::FIELD__REGISTERS       = "Registers";
const QString Device::FIELD__REG_BLOCK_TABLE = "table";
const QString Device::FIELD__REG_BLOCK_FIRST = "first";
const QString Device::FIELD__REG_BLOCK_SIZE  = "size";
const QString Device::FIELD__CHECKSUM        = "Checksum";
const QString Device::FIELD__RECONNECT       = "Reconnect";
const QString Device::FIELD__WAIT_READ       = "WaitRead";

//** archive
const QString Device::FIELD__ARH_TABLE       = "ArhTable";
const QString Device::FIELD__ARH_FILE        = "ArhFile";

/**
@brief Classes.
*/
const QString Device::CLASS__DUMMY           = "dummy";
const QString Device::CLASS__I7041           = "i7041";

/**
@brief Communication protocols
*/
const QString Device::PROTO_COMM__SERIAL     = "serial";
const QString Device::PROTO_COMM__ETH        = "eth";
const QString Device::PROTO_COMM__DUMMY      = "dummy";


/**
@brief  Constructor.
@param  None.
@return None.
*/
Device::Device(QObject *parent) : QObject(parent)
{
    mNetProtoComm = QString("");
    mUseLog       = false;
    mFileLog      = QString("");

    this->byDefault();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Device::~Device()
{
    this->byDefault();
}


/**
@brief  Set values by default.
@param  None.
@return None.
*/
void Device::byDefault()
{
    mID             = 0;
    mClass          = CLASS__DUMMY;
    mBaseAddr       = 0;
    mIP             = QString("");
    mPort           = 0;
    mFileRegisters  = QString("");
    mAllow          = false;
    mChecksum       = false;
    mReconnect      = false;
    mWaitRead       = -1;
    mArhTable       = QString("");
    mArhFile        = QString("");

    this->clearListRegisters();
}


/**
@brief  Load values from JsonObject.
@param  DataIn - data.
@return True if value of options are correct, otherwise - False.
*/
bool Device::fromJson(const QJsonObject &DataIn)
{
    this->byDefault();

    if(!DataIn.isEmpty())
    {
        mID            = static_cast<quint16>(DataIn.value(FIELD__ID).toInt(0));
        mClass         = DataIn.value(FIELD__CLASS).toString(CLASS__DUMMY);
        mBaseAddr      = static_cast<quint16>(DataIn.value(FIELD__BASE_ADDR).toInt(0));
        mIP            = DataIn.value(FIELD__IP).toString(QString(""));
        mPort          = static_cast<quint16>(DataIn.value(FIELD__PORT).toInt(0));
        mFileRegisters = DataIn.value(FIELD__REGISTERS).toString(QString(""));
        mWaitRead      = static_cast<qint16>(DataIn.value(FIELD__WAIT_READ).toInt(-1));
        mArhTable      = DataIn.value(FIELD__ARH_TABLE).toString(QString(""));
        mArhFile       = DataIn.value(FIELD__ARH_FILE).toString(QString(""));

        int Boo = (DataIn.value(FIELD__ALLOW).toInt(static_cast<int>(DENY)));
        mAllow = ((Boo) ? true : false);

        Boo = (DataIn.value(FIELD__CHECKSUM).toInt(static_cast<int>(DENY)));
        mChecksum = ((Boo) ? true : false);

        Boo = (DataIn.value(FIELD__RECONNECT).toInt(static_cast<int>(DENY)));
        mReconnect = ((Boo) ? true : false);

        this->normilize();

        if(mAllow && !mFileRegisters.isEmpty())
        {
            if(this->readFileRegisters(mFileRegisters)) this->initCalcRegisters();
        }
    }

    return (this->isCorrect());
}


/**
@brief  Pack data to String buffer.
@param  StringIn - link to string buffer.
@return None.
*/
void Device::toString(QString &StringIn)
{
    QString Correct    = QString(" (is correct)");
    QString NotCorrect = QString(" (is not correct)");

    StringIn+= QString("[ DEVICE ]\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ID;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mID);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__CLASS;
    StringIn+= QString(" = ");
    StringIn+= mClass;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__BASE_ADDR;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mBaseAddr);
    StringIn+= ((this->isCorrectBaseAddr()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__IP;
    StringIn+= QString(" = ");
    StringIn+= mIP;
    StringIn+= ((this->isCorrectIP()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PORT;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mPort);
    StringIn+= ((this->isCorrectPort()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ALLOW;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllow) ? ALLOW : DENY));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__CHECKSUM;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mChecksum) ? ALLOW : DENY));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__RECONNECT;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mReconnect) ? ALLOW : DENY));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__WAIT_READ;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mWaitRead);
    StringIn+= QString("\r\n");
    quint32 Sec  = 0;
    quint32 uSec = 0;
    if(mWaitRead > 0)
    {
        Sec  = ((mWaitRead < 1000) ? static_cast<quint32>(0) : static_cast<quint32>(mWaitRead/1000));
        uSec = ((mWaitRead < 1000) ? static_cast<quint32>(mWaitRead*1000) : static_cast<quint32>(0));

    }
    StringIn+= QString("  sec = %1\r\n").arg(QString::number(Sec));
    StringIn+= QString(" usec = %1\r\n").arg(QString::number(uSec));

    StringIn+= QString(" - ");
    StringIn+= FIELD__ARH_TABLE;
    StringIn+= QString(" = ");
    StringIn+= mArhTable;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ARH_FILE;
    StringIn+= QString(" = ");
    StringIn+= mArhFile;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__REGISTERS;
    StringIn+= QString(" = ");
    StringIn+= mFileRegisters;
    StringIn+= QString("\r\n");

    StringIn+= QString(" ListRegsGroups.size() = %1").arg(QString::number(mListRegsGroups.size()));
    StringIn+= QString("\r\n");

    RegsGroup *Group = nullptr;
//MUTEX LOCK
    QMutexLocker MutexLk(&mMutex);
    for(int i=0; i<mListRegsGroups.size(); i++)
    {
        Group = mListRegsGroups.at(i);

        if(Group)
        {
            StringIn+= QString(" group[%1]").arg(QString::number(i));
            StringIn+= QString("\r\n");
            Group->toString(StringIn);
        }
    }
//MUTEX UNLOCK
}


/**
@brief  Pack data to JSON buffer.
@param  ObjIn - link to JsonObject.
@return None.
@detailed { "ID":mID, "Class":mClass, "BaseAddr":mBaseAddr, ..., Register.mVar:Register.mValue }
*/
void Device::toJson(QJsonObject &ObjIn)
{
    ObjIn.insert(FIELD__ID, QJsonValue(mID));
    ObjIn.insert(FIELD__CLASS, QJsonValue(mClass));
    ObjIn.insert(FIELD__BASE_ADDR, QJsonValue(mBaseAddr));

    RegsGroup *Group = nullptr;

//MUTEX LOCK
    QMutexLocker MutexLk(&mMutex);
    for(int i=0; i<mListRegsGroups.size(); i++)
    {
        Group = mListRegsGroups.at(i);
        if(Group) Group->toJson(ObjIn);
    }
//MUTEX UNLOCK
}


/**
@brief  Pack values to JSON string.
@param  StringIn - link to string buffer
@return None.
*/
void Device::toJsonString(QString &StringIn)
{
    QJsonDocument Doc;
    QJsonObject Obj;

    this->toJson(Obj);
    Doc.setObject(Obj);

    StringIn+= QString(Doc.toJson(QJsonDocument::Compact));
}


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
int Device::toSql(const QString &ProfileIn, bool EventsIn, QList<QString> &ListStringsIn)
{
    int Res = 0;

    if(!mArhFile.isEmpty() || !mArhTable.isEmpty())
    {
        RegsGroup *Group = nullptr;
//MUTEX LOCK
        QMutexLocker MutexLk(&mMutex);
        for(int i=0; i<mListRegsGroups.size(); i++)
        {
            Group = mListRegsGroups.at(i);
            if(Group) Res+= ((EventsIn == true) ? Group->toSqlEventValue(ProfileIn, ListStringsIn) : Group->toSqlCurrentValue(ProfileIn, ListStringsIn));
        }
//MUTEX UNLOCK
        if(Res > 0)
        {
            ListStringsIn.prepend(QString(" VALUES "));
#ifdef SQL_PROC_TRM
            //(`stamp`,`profile`,`channel_id`,`value`,`exception`,`errcode`,`obj_status`,`prog`,`step`)
            ListStringsIn.prepend(QString("(`stamp`,`profile`,`channel_id`,`value`,`exception`,`errcode`,`obj_status`,`prog`,`step`)"));
#else
            //(`stamp`,`profile`,`device_id`,`register_id`,`value`,`ex`,`err`,`sign`)
            ListStringsIn.prepend(QString("(`stamp`,`profile`,`device_id`,`register_id`,`value`,`ex`,`err`,`sign`)"));
#endif
            ListStringsIn.prepend(QString("INSERT INTO `%1`").arg(((!mArhFile.isEmpty()) ? mArhFile : mArhTable)));
            Res+= 3;
        }
    }

    return (Res);
}


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
int Device::toSqlCurrent(const QString &ProfileIn, QList<QString> &ListStringsIn)
{
    return (this->toSql(ProfileIn, false, ListStringsIn));
}


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
int Device::toSqlEvent(const QString &ProfileIn, QList<QString> &ListStringsIn)
{
    return (this->toSql(ProfileIn, true, ListStringsIn));
}


/**
@brief  Merge list of string buffers into one SQL.
@param  ListStringsIn - link to list of string buffers;
@param  StringIn - string buffer.
@return The number of merged values.
*/
int Device::mergeSql(const QList<QString> &ListStringsIn, QString &StringIn)
{
    int Res = 0;

    if(!ListStringsIn.isEmpty())
    {
        int Size = ListStringsIn.size();
        QString Str;

        for(int i=0; i<Size; i++)
        {
            Str = ListStringsIn.at(i);

            if(!Str.isEmpty())
            {
                if(Res > 3) StringIn+= QString(",");
                StringIn+= Str;
                Res++;
            }
        }
    }

    return (Res);
}


/**
@brief  Check option "BaseAddr".
@param  None.
@return True if value of option "BaseAddr" is correct, otherwise - False.
*/
bool Device::isCorrectBaseAddr()
{
    if(mNetProtoComm == PROTO_COMM__SERIAL || mNetProtoComm == PROTO_COMM__ETH)
    {
        if(mBaseAddr > 0) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check option "IP" (for Network::PROTO_COMM__ETH).
@param  None.
@return True if value of option "IP" is correct, otherwise - False.
*/
bool Device::isCorrectIP()
{
    if(mNetProtoComm == PROTO_COMM__ETH)
    {
        if(!mIP.isEmpty()) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check option "Port" (for Network::PROTO_COMM__ETH).
@param  None.
@return True if value of option "Port" is correct, otherwise - False.
*/
bool Device::isCorrectPort()
{
    if(mNetProtoComm == PROTO_COMM__ETH)
    {
        if(mPort > 0) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check general options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Device::isCorrect()
{
    return ((this->isCorrectBaseAddr()) ? true : false);
}


/**
@brief  Normilize options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Device::normilize()
{
    return (this->isCorrect());
}


/**
@brief  Read list of registers from a file.
@param  FileIn - path to a file.
@return The number of registers that had been reading.
*/
quint16 Device::readFileRegisters(const QString &FileIn)
{
    Log::log(QString("Device::readFileRegisters(%1)").arg(FileIn), mFileLog, mUseLog);

    this->clearListRegisters();

    QString Err;
    QJsonDocument Doc = Json::readFile(FileIn, Err);
    if(!Err.isEmpty()) Log::log(Err, mFileLog, mUseLog, false);

    this->parseDataRegisters(Doc);

    return (this->sizeListRegisters());
}


/**
@brief  Parse data of list registers.
@param  DocIn - link to JsonDocument.
@return The number of registers that had been parsing.
*/
quint16 Device::parseDataRegisters(const QJsonDocument &DocIn)
{
    if(DocIn.isArray())
    {
        QJsonObject Obj;
        QJsonArray  Arr     = DocIn.array();
        Register   *Reg     = nullptr;
        RegsGroup  *Group   = nullptr;
        bool        IsValid = false;
        QString     LogBuff = QString("");

//MUTEX LOCK
        QMutexLocker MutexLk(&mMutex);
        for(int i=0; i<Arr.size(); i++)
        {
            Obj = Arr.at(i).toObject();

            if(!Obj.isEmpty())
            {
                Reg = new Register();
                Reg->mDevID    = mID;
                Reg->mDevClass = mClass;
                Reg->mUseLog   = mUseLog;
                Reg->mFileLog  = mFileLog;
                Reg->fromJson(Obj);

                if(Reg->isCorrect())
                {
                    IsValid = ((Group) ? Group->isValid(Reg) : false);

                    if(!IsValid)
                    {
                        Group = new RegsGroup();
                        Group->mFileLog = mFileLog;
                        Group->mUseLog  = mUseLog;
                        mListRegsGroups.append(Group);
                    }

                    Group->add(Reg);
                }
                else
                {
                    if(mUseLog)
                    {
                        LogBuff = QString("The register is incorrect!\r\n");
                        Reg->toString(LogBuff);
                        Log::log(LogBuff, mFileLog, mUseLog, false);
                    }
                    delete Reg;
                }
            }
        }
//MUTEX UNLOCK
    }

    return (this->sizeListRegisters());
}


/**
@brief  Clear list of registers.
@param  None.
@return None.
*/
void Device::clearListRegisters()
{
    if(!mListRegsGroups.isEmpty())
    {
        RegsGroup *Group = nullptr;
//MUTEX LOCK
        QMutexLocker MutexLk(&mMutex);
        while(mListRegsGroups.size())
        {
            Group = mListRegsGroups.takeLast();
            if(Group) delete Group;
        }
//MUTEX UNLOCK
    }
}


/**
@brief  Get size of list of registers.
@param  None.
@return Size of list of registers.
*/
quint16 Device::sizeListRegisters()
{
    quint16 Num      = 0;
    RegsGroup *Group = nullptr;

    for(int i=0; i<mListRegsGroups.size(); i++)
    {
        Group = mListRegsGroups.at(i);
        if(Group) Num+= Group->size();
    }

    return (Num);
}


/**
@brief  Get size of list of registers groups.
@param  None.
@return Size of list of registers groups.
*/
quint16 Device::sizeListRegsGroups()
{
    return (static_cast<quint16>(mListRegsGroups.size()));
}


/**
@brief  Check size of list of registers.
@param  None.
@return True if list of registers is empty, otherwise - False.
*/
bool Device::isListRegistersEmpty()
{
    return ((this->sizeListRegisters() == 0) ? true : false);
}


/**
@brief  Read randomized data.
@param  None.
@return The number of registers that had been reading.
*/
quint16 Device::randomize()
{
    Log::log(QString("Device::randomize(%1.%2)").arg(mClass, QString::number(mBaseAddr)), mFileLog, mUseLog);

    quint16 Num = 0;
    RegsGroup *Group = nullptr;
//MUTEX LOCK
    QMutexLocker MutexLk(&mMutex);
    for(int i=0; i<mListRegsGroups.size(); i++)
    {
        Group = mListRegsGroups.at(i);
        if(Group) Num+= Group->randomize();
    }
//MUTEX UNLOCK
    return (Num);
}


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
quint16 Device::readRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn, const QString &ModeIn)
{
    Log::log(QString("%1 %2 %3 %4 %5 %6").arg(SerialPortIn, QString::number(SpdIn), PrtyIn, QString::number(DataBitsIn), QString::number(StopBitsIn), ModeIn), mFileLog, mUseLog);
    return (static_cast<quint16>(0));
}


/**
@brief  Read registers by Serial.
@param  SerialPortIn - the name (full path) of serial port,
@param         SpdIn - the speed of serial port,
@param        PrtyIn - the parity of serial port,
@param    DataBitsIn - the number of data bits of serial port,
@param    StopBitsIn - the number of stop bits of serial port,
@return The number of registers that had been reading.
*/
quint16 Device::readRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn)
{
    Log::log(QString("%1 %2 %3 %4 %5 %6 %7").arg(SerialPortIn, QString::number(SpdIn), PrtyIn, QString::number(DataBitsIn), QString::number(StopBitsIn), (mChecksum ? QString("1") : QString("0")), QString::number(mWaitRead)), mFileLog, mUseLog);
    return (static_cast<quint16>(0));
}


/**
@brief  Read registers.
@param  None.
@return The number of registers that had been reading.
*/
quint16 Device::readRegisters()
{
    return (static_cast<quint16>(0));
}


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
quint16 Device::writeRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn, const QString &ModeIn)
{
    (void)SerialPortIn;
    (void)SpdIn;
    (void)PrtyIn;
    (void)DataBitsIn;
    (void)StopBitsIn;
    (void)ModeIn;
    return (static_cast<quint16>(0));
}

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
quint16 Device::writeRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn, const QString &ModeIn, QJsonObject &ObjIn)
{
    (void)SerialPortIn;
    (void)SpdIn;
    (void)PrtyIn;
    (void)DataBitsIn;
    (void)StopBitsIn;
    (void)ModeIn;
    (void)ObjIn;
    return (static_cast<quint16>(0));
}

/**
@brief  Write registers.
@param  None.
@return The number of registers that had been written.
*/
quint16 Device::writeRegisters()
{
    return (static_cast<quint16>(0));
}

/**
@brief  Write registers.
@param  ObjIn - link to JsonObject (associative array of values).
@return The number of registers that had been written.
@detailed { RegID:RegValue, ... }
*/
quint16 Device::writeRegisters(QJsonObject &ObjIn)
{
    (void)ObjIn;
    return (static_cast<quint16>(0));
}


/**
@brief  Read Dummy-registers.
@param  None.
@return The number of registers that had been reading.
*/
quint16 Device::readDummyRegisters()
{
    Log::log(QString("Device::readDummyRegisters(%1.%2)").arg(mClass, QString::number(mBaseAddr)), mFileLog, mUseLog);

    quint16 Result   = 0;
    RegsGroup *Group = nullptr;
    Register *Reg    = nullptr;
    quint16  *Values = nullptr;
    quint16 j;

//MUTEX LOCK
    QMutexLocker MutexLk(&mMutex);
    for(int i=0; i<mListRegsGroups.size(); i++)
    {
        Group = mListRegsGroups.at(i);

        if(Group)
        {
            if(Group->size() > 0)
            {
                Values = new quint16[Group->size()];

                for(j=0; j<Group->size(); j++)
                {
                    Reg = Group->get(j);

                    if(Reg)
                    {
                        Values[j] = Reg->mValue;
                        if(!(Reg->mClass == Register::CLASS__CONST || Reg->mClass == Register::CLASS__CALC)) Values[j] = static_cast<quint16>(Reg->randomize());
                        Result++;
                    }
                }

                if(Result > 0) Group->setValues(Values, Group->size());
                delete Values;
            }
        }
    }
//MUTEX UNLOCK
    return (Result);
}


/**
@brief  Get registers.
@param  ListIDsIn - list of ID of target registers.
@return List of registers.
*/
QList<Register *> Device::getRegisters(const QList<quint16> &ListIDsIn)
{
    QList<Register *> ListRegs;

    if(!ListIDsIn.isEmpty())
    {
        RegsGroup *Group = nullptr;
//MUTEX LOCK
        QMutexLocker MutexLk(&mMutex);
        for(int i=0; i<mListRegsGroups.size(); i++)
        {
            Group = mListRegsGroups.at(i);
            if(Group != nullptr) ListRegs+= Group->getByID(ListIDsIn);
        }
//MUTEX UNLOCK
    }

    return (ListRegs);
}


/**
@brief  Get values.
@param  ListIDsIn - list of ID of target registers.
@return List of raw-values.
*/
QList<quint16 *> Device::getValues(const QList<quint16> &ListIDsIn)
{
    QList<quint16 *> ListValues;

    if(!ListIDsIn.isEmpty())
    {
        RegsGroup *Group = nullptr;
//MUTEX LOCK
        QMutexLocker MutexLk(&mMutex);
        for(int i=0; i<mListRegsGroups.size(); i++)
        {
            Group = mListRegsGroups.at(i);
            if(Group != nullptr) ListValues+= Group->getValuesByID(ListIDsIn);
        }
//MUTEX UNLOCK
    }

    Log::log(QString("Device::getValues() = %1").arg(QString::number(ListValues.size())), mFileLog, mUseLog);

    return (ListValues);
}


/**
@brief  Get registers.
@param  ClassIn - class of target registers.
@return List of registers.
*/
QList<Register *> Device::getRegisters(const QString ClassIn)
{
    QList<Register *> ListRegs;

    if(!ClassIn.isEmpty())
    {
        RegsGroup *Group = nullptr;
//MUTEX LOCK
        QMutexLocker MutexLk(&mMutex);
        for(int i=0; i<mListRegsGroups.size(); i++)
        {
            Group = mListRegsGroups.at(i);
            if(Group != nullptr)
            {
                if(Group->isClass(ClassIn)) ListRegs+= Group->getAll();
            }
        }
//MUTEX UNLOCK
    }

    Log::log(QString("Device::getRegisters(%1) = %2").arg(ClassIn, QString::number(ListRegs.size())), mFileLog, mUseLog);

    return (ListRegs);
}


/**
@brief  Init. calculated registers.
@param  None.
@return None.
*/
void Device::initCalcRegisters()
{
    Log::log(QString("Device::initCalcRegisters()"), mFileLog, mUseLog);

    QList<Register *> ListRegs = this->getRegisters(Register::CLASS__CALC);

    if(!ListRegs.isEmpty())
    {
        Register *Reg = nullptr;
        for(int i=0; i<ListRegs.size(); i++)
        {
            Reg = ListRegs.at(i);
            if(Reg) Reg->mListValuesToCalc = this->getValues(Reg->mListTargetsToCalc);
        }
    }
}
