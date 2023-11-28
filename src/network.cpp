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

#include "network.h"

/**
@brief Fields
*/
const QString Network::FIELD__ID              = "ID";
const QString Network::FIELD__PROTO_COMM      = "ProtoComm";
const QString Network::FIELD__PROTO_DATA      = "ProtoData";
const QString Network::FIELD__PORT_PREF       = "PortPref";
const QString Network::FIELD__PORT            = "Port";
const QString Network::FIELD__PORT_DEV        = "PortDev";
const QString Network::FIELD__SPD             = "Spd";
const QString Network::FIELD__PRTY            = "Prty";
const QString Network::FIELD__DATA_BITS       = "DataBits";
const QString Network::FIELD__STOP_BITS       = "StopBits";
const QString Network::FIELD__MODE            = "Mode";
const QString Network::FIELD__ALLOW           = "Allow";
const QString Network::FIELD__DEVICES         = "Devices";

/**
@brief Communication protocols
*/
const QString Network::PROTO_COMM__SERIAL     = "serial";
const QString Network::PROTO_COMM__ETH        = "eth";
const QString Network::PROTO_COMM__DUMMY      = "dummy";

/**
@brief Data transfer protocol
*/
const QString Network::PROTO_DATA__MODBUS_RTU = "modbus_rtu";
const QString Network::PROTO_DATA__MODBUS_TCP = "modbus_tcp";
const QString Network::PROTO_DATA__DCON       = "dcon";

/**
@brief Serial port :: The parities
*/
const QString Network::SERIAL_PRTY__NONE      = "None";
const QString Network::SERIAL_PRTY__EVEN      = "Even";
const QString Network::SERIAL_PRTY__ODD       = "Odd";


/**
@brief Serial port :: The modes
*/
const QString Network::SERIAL_MODE__RS232     = "RS232";
const QString Network::SERIAL_MODE__RS485     = "RS485";


/**
@brief  Constructor.
@param  None.
@return None.
*/
Network::Network(QObject *parent) : QObject(parent)
{
    mUseLog  = false;
    mFileLog = QString("");

    this->byDefault();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Network::~Network()
{
    this->byDefault();
}


/**
@brief  Set values by default.
@param  None.
@return None.
*/
void Network::byDefault()
{
    mID             = 0;
    mProtoComm      = PROTO_COMM__DUMMY;
    mProtoData      = QString("");
    mSerialPort     = 0;
    mSerialPortPref = QString("");
    mSerialPortDev  = QString("");
    mSerialSpd      = SERIAL_SPD__9600;
    mSerialPrty     = SERIAL_PRTY__NONE;
    mSerialDataBits = SERIAL_DATA_BITS__8;
    mSerialStopBits = SERIAL_STOP_BITS__1;
    mSerialMode     = SERIAL_MODE__RS232;
    mFileDevices    = QString("");
    mAllow          = false;

    this->clearListDevices();
}


/**
@brief  Load values from JsonObject.
@param  DataIn - data.
@return True if value of options are correct, otherwise - False.
*/
bool Network::fromJson(const QJsonObject &DataIn)
{
    this->byDefault();

    if(!DataIn.isEmpty())
    {
        mID             = static_cast<quint16>(DataIn.value(FIELD__ID).toInt(0));
        mProtoComm      = DataIn.value(FIELD__PROTO_COMM).toString(PROTO_COMM__DUMMY);
        mProtoData      = DataIn.value(FIELD__PROTO_DATA).toString(QString(""));
        mSerialPort     = static_cast<quint16>(DataIn.value(FIELD__PORT).toInt(0));
        mSerialPortPref = DataIn.value(FIELD__PORT_PREF).toString(QString(""));
        mSerialPortDev  = DataIn.value(FIELD__PORT_DEV).toString(QString(""));
        mSerialSpd      = static_cast<quint32>(DataIn.value(FIELD__SPD).toInt(0));
        mSerialPrty     = DataIn.value(FIELD__PRTY).toString(QString(""));
        mSerialDataBits = static_cast<quint8>(DataIn.value(FIELD__DATA_BITS).toInt(0));
        mSerialStopBits = static_cast<quint8>(DataIn.value(FIELD__STOP_BITS).toInt(0));
        mSerialMode     = DataIn.value(FIELD__MODE).toString(QString(""));
        mFileDevices    = DataIn.value(FIELD__DEVICES).toString(QString(""));

        int Boo = (DataIn.value(FIELD__ALLOW).toInt(static_cast<int>(DENY)));
        mAllow  = ((Boo) ? true : false);

        this->normilize();

        if(mAllow && !mFileDevices.isEmpty()) this->readFileDevices(mFileDevices);
    }

    return (this->isCorrect());
}


/**
@brief  Pack values to String buffer.
@param  StringIn - link to string buffer.
@return None.
*/
void Network::toString(QString &StringIn)
{
    QString Correct    = QString(" (is correct)");
    QString NotCorrect = QString(" (is not correct)");

    StringIn+= QString("[ NETWORK ]\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ID;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mID);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PROTO_COMM;
    StringIn+= QString(" = ");
    StringIn+= mProtoComm;
    StringIn+= ((this->isCorrectProtoComm()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PROTO_DATA;
    StringIn+= QString(" = ");
    StringIn+= mProtoData;
    StringIn+= ((this->isCorrectProtoData()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PORT;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mSerialPort);
    StringIn+= ((this->isCorrectSerialPort()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PORT_PREF;
    StringIn+= QString(" = ");
    StringIn+= mSerialPortPref;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PORT_DEV;
    StringIn+= QString(" = ");
    StringIn+= mSerialPortDev;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__SPD;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mSerialSpd);
    StringIn+= ((this->isCorrectSerialSpd()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PRTY;
    StringIn+= QString(" = ");
    StringIn+= mSerialPrty;
    StringIn+= ((this->isCorrectSerialPrty()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__DATA_BITS;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mSerialDataBits);
    StringIn+= ((this->isCorrectSerialDataBits()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__STOP_BITS;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mSerialStopBits);
    StringIn+= ((this->isCorrectSerialStopBits()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__MODE;
    StringIn+= QString(" = ");
    StringIn+= mSerialMode;
    StringIn+= ((this->isCorrectSerialMode()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ALLOW;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllow) ? ALLOW : DENY));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__DEVICES;
    StringIn+= QString(" = ");
    StringIn+= mFileDevices;
    StringIn+= QString("\r\n");

    StringIn+= QString(" ListDevices.size() = %1").arg(QString::number(mListDevices.size()));
    StringIn+= QString("\r\n");

    Device *Dev = nullptr;

    for(int i=0; i<mListDevices.size(); i++)
    {
        Dev = mListDevices.at(i);

        if(Dev)
        {
            StringIn+= QString(" device[%1]").arg(QString::number(i));
            StringIn+= QString("\r\n");
            Dev->toString(StringIn);
        }
    }
}


/**
@brief  Pack data to JSON buffer.
@param  ObjIn - link to JsonObject.
@return None.
@detailed { "ID":mID, "Devices": [ {...}, ... ] }
*/
void Network::toJson(QJsonObject &ObjIn)
{
    QJsonArray  ArrDevs;
    QJsonObject ObjDev;
    Device     *Dev = nullptr;

    for(int i=0; i<mListDevices.size(); i++)
    {
        Dev = mListDevices.at(i);

        if(Dev != nullptr)
        {
             ObjDev = QJsonObject();
             Dev->toJson(ObjDev);
             ArrDevs.append(ObjDev);
        }
    }

    ObjIn.insert(FIELD__ID, QJsonValue(mID));
    ObjIn.insert(FIELD__DEVICES, QJsonValue(ArrDevs));
}


/**
@brief  Pack values to JSON string.
@param  StringIn - link to string buffer
@return None.
*/
void Network::toJsonString(QString &StringIn)
{
    QJsonDocument Doc;
    QJsonObject Obj;

    this->toJson(Obj);
    Doc.setObject(Obj);

    StringIn+= QString(Doc.toJson(QJsonDocument::Compact));
}


/**
@brief  Check option "ProtoComm".
@param  None.
@return True if value of option "ProtoComm" is correct, otherwise - False.
*/
bool Network::isCorrectProtoComm()
{
    return ((mProtoComm == PROTO_COMM__SERIAL || mProtoComm == PROTO_COMM__ETH || mProtoComm == PROTO_COMM__DUMMY) ? true : false);
}


/**
@brief  Check option "ProtoData".
@param  None.
@return True if value of option "ProtoData" is correct, otherwise - False.
*/
bool Network::isCorrectProtoData()
{
    if(mProtoComm == PROTO_COMM__SERIAL)
    {
        if(mProtoData == PROTO_DATA__MODBUS_RTU || mProtoData == PROTO_DATA__DCON) return (true);
    }
    else if(mProtoComm == PROTO_COMM__ETH)
    {
        if(mProtoData == PROTO_DATA__MODBUS_TCP) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check option "SerialPort".
@param  None.
@return True if value of option "SerialPort" is correct, otherwise - False.
*/
bool Network::isCorrectSerialPort()
{
    return (true);
}


/**
@brief  Check option "SerialSpd".
@param  None.
@return True if value of option "SerialSpd" is correct, otherwise - False.
*/
bool Network::isCorrectSerialSpd()
{
    if(mProtoComm == PROTO_COMM__SERIAL)
    {
        if(mSerialSpd == SERIAL_SPD__2400 || mSerialSpd == SERIAL_SPD__4800 || mSerialSpd == SERIAL_SPD__9600 || mSerialSpd == SERIAL_SPD__14400 || mSerialSpd == SERIAL_SPD__19200 || mSerialSpd == SERIAL_SPD__28800 || mSerialSpd == SERIAL_SPD__38800 || mSerialSpd == SERIAL_SPD__57600 || mSerialSpd == SERIAL_SPD__115200) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check option "SerialPrty".
@param  None.
@return True if value of option "SerialPrty" is correct, otherwise - False.
*/
bool Network::isCorrectSerialPrty()
{
    if(mProtoComm == PROTO_COMM__SERIAL)
    {
        if(mSerialPrty == SERIAL_PRTY__EVEN || mSerialPrty == SERIAL_PRTY__NONE || mSerialPrty == SERIAL_PRTY__ODD) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check option "SerialDataBits".
@param  None.
@return True if value of option "SerialDataBits" is correct, otherwise - False.
*/
bool Network::isCorrectSerialDataBits()
{
    if(mProtoComm == PROTO_COMM__SERIAL)
    {
        if(mSerialDataBits == SERIAL_DATA_BITS__7 || mSerialDataBits == SERIAL_DATA_BITS__8) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check option "SerialStopBits".
@param  None.
@return True if value of option "SerialStopBits" is correct, otherwise - False.
*/
bool Network::isCorrectSerialStopBits()
{
    if(mProtoComm == PROTO_COMM__SERIAL)
    {
        if(mSerialStopBits == SERIAL_STOP_BITS__1 || mSerialStopBits == SERIAL_STOP_BITS__2) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check option "SerialMode".
@param  None.
@return True if value of option "SerialMode" is correct, otherwise - False.
*/
bool Network::isCorrectSerialMode()
{
    if(mProtoComm == PROTO_COMM__SERIAL)
    {
        if(mSerialMode == SERIAL_MODE__RS232 || mSerialMode == SERIAL_MODE__RS485) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Network::isCorrect()
{
    if(mProtoComm == PROTO_COMM__SERIAL)
    {
        if((mProtoData == PROTO_DATA__MODBUS_RTU || mProtoData == PROTO_DATA__DCON) && this->isCorrectSerialPort() && this->isCorrectSerialSpd() && this->isCorrectSerialPrty() && this->isCorrectSerialDataBits() && this->isCorrectSerialStopBits() && this->isCorrectSerialMode()) return (true);
    }
    else if(mProtoComm == PROTO_COMM__ETH)
    {
        if(mProtoData == PROTO_DATA__MODBUS_TCP) return (true);
    }
    else if(mProtoComm == PROTO_COMM__DUMMY)
    {
        return (true);
    }

    return (false);
}


/**
@brief  Normilize options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Network::normilize()
{
    return (this->isCorrect());
}


/**
@brief  Read list of devices from a file.
@param  FileIn - path to a file.
@return The number of devices have read.
*/
quint16 Network::readFileDevices(const QString &FileIn)
{
    Log::log(QString("Network::readFileDevices(%1)").arg(FileIn), mFileLog, mUseLog);

    this->clearListDevices();

    QString Err;
    QJsonDocument Doc = Json::readFile(FileIn, Err);
    if(!Err.isEmpty()) Log::log(Err, mFileLog, mUseLog, false);

    this->parseDataDevices(Doc);

    return (this->sizeListDevices());
}


/**
@brief  Parse data of list devices.
@param  DocIn - link to JsonDocument.
@return The number of devices have parsed.
*/
quint16 Network::parseDataDevices(const QJsonDocument &DocIn)
{
    if(DocIn.isArray())
    {
        QJsonArray Arr = DocIn.array();
        QJsonObject Obj;
        Device     *Dev = nullptr;

        for(int i=0; i<Arr.size(); i++)
        {
            Obj = Arr.at(i).toObject();

            if(!Obj.isEmpty())
            {
                Dev = nullptr;

                if(mProtoComm == PROTO_COMM__SERIAL)
                {
                    if(mProtoData == PROTO_DATA__MODBUS_RTU)
                    {
                        Dev = new DeviceModBusRTU();
                    }
                    else if(mProtoData == PROTO_DATA__DCON)
                    {
                        Dev = new DeviceDCON7000();
                    }
                }
                else if(mProtoComm == PROTO_COMM__ETH)
                {
                    if(mProtoData == PROTO_DATA__MODBUS_TCP) Dev = new DeviceModBusTCP();
                }
                else
                {
                    Dev = new Device();
                }

                if(Dev != nullptr)
                {
                    Dev->mFileLog      = mFileLog;
                    Dev->mUseLog       = mUseLog;
                    Dev->mNetProtoComm = mProtoComm;
                    Dev->fromJson(Obj);

                    mListDevices.append(Dev);
                }
            }
        }
    }

    return (this->sizeListDevices());
}


/**
@brief  Clear list of devices.
@param  None.
@return None.
*/
void Network::clearListDevices()
{
    if(!mListDevices.isEmpty())
    {
        Device *Dev = nullptr;

        while(mListDevices.size())
        {
            Dev = mListDevices.takeLast();
            if(Dev) delete Dev;
        }
    }
}


/**
@brief  Get size of list of devices.
@param  None.
@return Size of list of devices.
*/
quint16 Network::sizeListDevices()
{
    return (static_cast<quint16>(mListDevices.size()));
}


/**
@brief  Check size of list of devices.
@param  None.
@return True if list of devices is empty, otherwise - False.
*/
bool Network::isListDevicesEmpty()
{
    return ((this->sizeListDevices() == 0) ? true : false);
}


/**
@brief  Get value of option `ArhFile` of a device.
@param  IdxIn - index of list of devices (0...ListDevices.size()-1).
@return Value of the option.
*/
QString Network::getDeviceArhFile(const int IdxIn)
{
    QString Res("");
    quint16 Size = this->sizeListDevices();

    if(IdxIn >= 0 && Size > 0)
    {
        if(IdxIn < Size)
        {
            Device *Dev = mListDevices.at(IdxIn);
            if(Dev) Res = Dev->mArhFile;
        }
    }

    return (Res);
}


/**
@brief  Get value of option `ArhTable` of a device.
@param  IdxIn - index of list of devices (0...ListDevices.size()-1).
@return Value of the option.
*/
QString Network::getDeviceArhTable(const int IdxIn)
{
    QString Res("");
    quint16 Size = this->sizeListDevices();

    if(IdxIn >= 0 && Size > 0)
    {
        if(IdxIn < Size)
        {
            Device *Dev = mListDevices.at(IdxIn);
            if(Dev) Res = Dev->mArhTable;
        }
    }

    return (Res);
}


/**
@brief  Get SQL-package of a Device data.
@param  ProfileIn - name of profile;
@param  EventsIn - true for event values, false for current values;
@param  IdxIn - index of list of devices (0...ListDevices.size()-1);
@param  ListStringsIn - link to list of string buffers.
@return The number of packed values.
*/
int Network::getDeviceSql(const QString &ProfileIn, bool EventsIn, const int IdxIn, QList<QString> &ListStringsIn)
{
    int Res = 0;

    quint16 Size = this->sizeListDevices();

    if(IdxIn >= 0 && Size > 0)
    {
        if(IdxIn < Size)
        {
            Device *Dev = mListDevices.at(IdxIn);
            if(Dev) Res = Dev->toSql(ProfileIn, EventsIn, ListStringsIn);
        }
    }

    return (Res);
}


/**
@brief  Start randomized surey.
@param  None.
@return True if the survey has completed, otherwise - False.
*/
bool Network::randomize()
{
    return (this->survey(true));
}


/**
@brief  Start surey.
@param  None.
@return True if the survey has completed, otherwise - False.
*/
bool Network::survey()
{
    return (this->survey(false));
}


/**
@brief  Start survey.
@param  RandomIn - true if the survey is randomized.
@return True if the survey has completed, otherwise - False.
*/
bool Network::survey(const bool RandomIn)
{
    Log::log(QString("Network::survey(%1)").arg(((RandomIn) ? QString("randomized") : QString("real"))), mFileLog, mUseLog);

    if(this->isCorrect())
    {
        Device *Dev = nullptr;

        for(int i=0; i<mListDevices.size(); i++)
        {
            Dev = mListDevices.at(i);
            if(Dev)
            {
               if(Dev->mAllow)
               {
                   if(!RandomIn)
                   {
                       if(mProtoComm == PROTO_COMM__SERIAL)
                       {
                           QString PortDev = ((!mSerialPortDev.isEmpty()) ? mSerialPortDev : QString(""));

                           if(mProtoData == PROTO_DATA__MODBUS_RTU)
                           {
                               if(PortDev.isEmpty()) PortDev = ((!mSerialPortPref.isEmpty()) ? HelperModBusRTUClient::convSerialNum(mSerialPortPref, mSerialPort) : HelperModBusRTUClient::convSerialNum(mSerialPort));
                               Dev->readRegisters(PortDev, mSerialSpd, mSerialPrty, mSerialDataBits, mSerialStopBits, mSerialMode);
                           }
                           else if(mProtoData == PROTO_DATA__DCON)
                           {
                               if(PortDev.isEmpty()) PortDev = ((!mSerialPortPref.isEmpty()) ? SerialPort::comToCode(mSerialPortPref, mSerialPort) : SerialPort::comToCode(mSerialPort));
                               Dev->readRegisters(PortDev, mSerialSpd, mSerialPrty, mSerialDataBits, mSerialStopBits);
                           }
                       }
                       else if(mProtoComm == PROTO_COMM__ETH)
                       {
                           Dev->readRegisters();
                       }
                       else
                       {
                           Dev->readDummyRegisters();
                       }
                   }
                   else
                   {
                       Dev->randomize();
                   }
               }
            }
        }

        return (true);
    }
    else
    {
        Log::log(QString("The configuration is incorrect!"), mFileLog, mUseLog, false);
    }

    return (false);
}


/**
@brief  Write data.
@param  DevID - Device ID.
@param  ObjIn - link to JsonObject-data.
@return True if the data has written, otherwise - False.
@detailed ObjIn = { RegVar:RegValue, ... }
*/
bool Network::write(quint16 DevID, QJsonObject &ObjIn)
{
    Log::log(QString("Network::write(DevID:%1)").arg(QString::number(DevID)), mFileLog, mUseLog);

    if(this->isCorrect())
    {
        Device *Dev;

        for(int i=0; i<mListDevices.size(); i++)
        {
            Dev = mListDevices.at(i);
            if(Dev)
            {
               if(Dev->mAllow && Dev->mID == DevID)
               {
                   if(mProtoComm == PROTO_COMM__SERIAL)
                   {
                       QString PortDev = ((!mSerialPortDev.isEmpty()) ? mSerialPortDev : QString(""));

                       if(mProtoData == PROTO_DATA__MODBUS_RTU)
                       {
                           if(PortDev.isEmpty()) PortDev = ((!mSerialPortPref.isEmpty()) ? HelperModBusRTUClient::convSerialNum(mSerialPortPref, mSerialPort) : HelperModBusRTUClient::convSerialNum(mSerialPort));
                           Dev->writeRegisters(PortDev, mSerialSpd, mSerialPrty, mSerialDataBits, mSerialStopBits, mSerialMode, ObjIn);
                       }
                   }
                   else if(mProtoComm == PROTO_COMM__ETH)
                   {
                       Dev->writeRegisters(ObjIn);
                   }
               }
            }
        }

        return (true);
    }
    else
    {
        Log::log(QString("The configuration is incorrect!"), mFileLog, mUseLog, false);
    }

    return (false);
}
