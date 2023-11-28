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

#include "device-dcon7000.h"


/**
@brief  Constructor.
@param  None.
@return None.
*/
DeviceDCON7000::DeviceDCON7000(QObject *parent) : Device(parent)
{

}


/**
@brief  Destructor.
@param  None.
@return None.
*/
DeviceDCON7000::~DeviceDCON7000()
{

}


/**
@brief  Send command AA6.
@param  SerialPortIn - the pointer to object of opened SerialPort
@param  RegisterIn - pointer to register.
@return Word.
*/
quint16 DeviceDCON7000::cmdAA6(SerialPort *SerialPortIn, Register *RegisterIn)
{
    quint16 Result = 0;

    if(SerialPortIn && RegisterIn)
    {
        if(RegisterIn->mClass == Register::CLASS__6)
        {
            QByteArray Request, Response;

            DCON7000::cmdAA6(static_cast<quint8>(mBaseAddr), mChecksum, Request);

            Log::log(QString("Request: %1").arg(QString(Request)), mFileLog, mUseLog);

            if(SerialPortIn->write(Request))
            {
                Response = SerialPortIn->read();

                if((SerialPortIn->getError()) != QSerialPort::ReadError)
                {
                    Log::log(QString("Response: %1").arg(QString(Response)), mFileLog, mUseLog);

                    if(RegisterIn->mDevClass == Device::CLASS__I7041)
                    {
                        Result = DCON7000::respAA6_7041(Response);
                        Log::log(QString("Result: %1").arg(QString::number(Result)), mFileLog, mUseLog);
                    }
                }
                else
                {
                    Log::log(QString("Error read data from serial port '%1' (%2)! %3").arg(SerialPortIn->mPortN, QString::number(SerialPortIn->getError()), SerialPortIn->getErrorString()), mFileLog, mUseLog);
                }
            }
        }
    }

    return (Result);
}


/**
@brief  Read group by Serial.
@param  SerialPortIn - the pointer to object of opened SerialPort,
@param  GroupIn - the pointer to group.
@return The number of registers that had been reading.
*/
quint16 DeviceDCON7000::readGroup(SerialPort *SerialPortIn, RegsGroup *GroupIn)
{
    quint16 Result = 0;

    if(SerialPortIn && GroupIn)
    {
        Register *Reg    = nullptr;
        quint16  *Values = nullptr;

        if(GroupIn->size() > 0 && GroupIn->isAllowToRead() && GroupIn->isClass(Register::CLASS__6))
        {
            Values = new quint16[GroupIn->size()];

            for(quint16 i=0; i<GroupIn->size(); i++)
            {
                Reg = GroupIn->get(i);

                if(Reg)
                {
                    if(Reg->mClass == Register::CLASS__6) Values[i] = cmdAA6(SerialPortIn, Reg);
                    Result++;
                }
            }

            if(Result > 0) GroupIn->setValues(Values, GroupIn->size());
            delete Values;
        }
        else
        {
            Log::log(QString("The class '%1' of registers is not supported!").arg(GroupIn->getClass()), mFileLog, mUseLog);
        }
    }

    return (Result);
}


/**
@brief  Read registers by Serial.
@param  SerialPortIn - the name (full path) of serial port,
@param  SpdIn - the speed of serial port,
@param  PrtyIn - the parity of serial port,
@param  DataBitsIn - the number of data bits of serial port,
@param  StopBitsIn - the number of stop bits of serial port,
@return The number of registers that had been reading.
*/
quint16 DeviceDCON7000::readRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn)
{
    Log::log(QString("DeviceDCON7000::readRegisters(SerialPort=%1,Spd=%2,Prty=%3,DataBits=%4,StopBits=%5,Class=%6,BaseAddr=%7)").arg(SerialPortIn, QString::number(SpdIn), PrtyIn, QString::number(DataBitsIn), QString::number(StopBitsIn), mClass, QString::number(mBaseAddr)), mFileLog, mUseLog);

    quint16 Result = 0;

    if(this->isCorrect() && this->sizeListRegisters() > 0)
    {
        SerialPort *Port       = new SerialPort();
        Port->mPortN           = SerialPortIn;
        Port->mPortSpeed       = SerialPort::spdToCode(static_cast<int>(SpdIn));
        Port->mPortParity      = SerialPort::prtyToCode(PrtyIn);
        Port->mPortDataBits    = SerialPort::databitsToCode(static_cast<int>(DataBitsIn));
        Port->mPortStopBits    = SerialPort::stopbitToCode(static_cast<int>(StopBitsIn));
        Port->mPortFlowControl = SerialPort::flowctrlToCode(SerialPort::FLOWCTRL__NO);
        Port->mWaitReadyRead   = mWaitRead;

        if(Port->open(QIODevice::ReadWrite))
        {
            RegsGroup *Group = nullptr;
//MUTEX LOCK
            QMutexLocker MutexLk(&mMutex);
            for(int i=0; i<mListRegsGroups.size(); i++)
            {
                Group = mListRegsGroups.at(i);
                Result+= readGroup(Port, Group);
            }
//MUTEX UNLOCK
            Port->close();
        }
        else
        {
            Log::log(QString("Error connection with '%1' (%2)! %3").arg(Port->mPortN, QString::number(Port->getError()), Port->getErrorString()), mFileLog, mUseLog, false);
        }

        delete Port;
    }
    else
    {
        Log::log(QString("The configuration is incorrect or list of registers is empty!"), mFileLog, mUseLog, false);
    }

    return (Result);
}
