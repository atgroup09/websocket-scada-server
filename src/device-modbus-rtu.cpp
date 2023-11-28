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

#include "device-modbus-rtu.h"


/**
@brief  Constructor.
@param  None.
@return None.
*/
DeviceModBusRTU::DeviceModBusRTU(QObject *parent) : Device(parent)
{

}


/**
@brief  Destructor.
@param  None.
@return None.
*/
DeviceModBusRTU::~DeviceModBusRTU()
{

}


/**
@brief  Read registers (DISC, COILS).
@param  ModBusCliIn - pointer to ModBus RTU client,
@param  GroupIn     - pointer to group.
@return Result:
@arg      > 0 - quantity of read registers
@arg     = -1 - register class is not supported
@arg     = -2 - ModBus RTU client error connection
@arg     = -3 - ModBus RTU client error read data
*/
int DeviceModBusRTU::readBits(HelperModBusRTUClient *ModBusCliIn, RegsGroup *GroupIn)
{
    int Res = -1;

    if(ModBusCliIn && GroupIn)
    {
        int Func = -1;

        if(GroupIn->isClass(Register::CLASS__COIL))
        {
            Func = HelperModBusClient::FUNC__READ_COIL_REGS;
        }
        else if(GroupIn->isClass(Register::CLASS__DISC))
        {
            Func = HelperModBusClient::FUNC__READ_DISC_REGS;
        }

        if(GroupIn->size() > 0 && GroupIn->isAllowToRead() && Func > 0)
        {
            if(mReconnect) ModBusCliIn->reconnect();

            if(ModBusCliIn->isConnected())
            {
                quint8 *Values = nullptr;
                Values = new quint8[GroupIn->size()];

                Res = ModBusCliIn->readBits(Values, GroupIn->getFirstAddr(), GroupIn->size(), Func);

                if(Res != HelperModBusClient::ERROR_RES)
                {
                    GroupIn->setValues(Values, GroupIn->size());
                }
                else
                {
                    Res = -3;
                }

                delete Values;
            }
            else
            {
                Res = -2;
            }
        }
    }

    return (Res);
}

/**
@brief  Read registers (INPT, HOLD).
@param  ModBusCliIn - pointer to ModBus RTU client,
@param  GroupIn     - pointer to group.
@return Result:
@arg      > 0 - quantity of read registers
@arg     = -1 - register class is not supported
@arg     = -2 - ModBus RTU client error connection
@arg     = -3 - ModBus RTU client error read data
*/
int DeviceModBusRTU::readRegs(HelperModBusRTUClient *ModBusCliIn, RegsGroup *GroupIn)
{
    int Res = -1;

    if(ModBusCliIn && GroupIn)
    {
        int Func = -1;

        if(GroupIn->isClass(Register::CLASS__HOLDING))
        {
            Func = HelperModBusClient::FUNC__READ_HOLDING_REGS;
        }
        else if(GroupIn->isClass(Register::CLASS__IN) || GroupIn->isClass(Register::CLASS__INPT))
        {
            Func = HelperModBusClient::FUNC__READ_INPUT_REGS;
        }

        if(GroupIn->size() > 0 && GroupIn->isAllowToRead() && Func > 0)
        {
            if(mReconnect) ModBusCliIn->reconnect();

            if(ModBusCliIn->isConnected())
            {
                quint16 *Values = nullptr;
                Values = new quint16[GroupIn->size()];

                Res = ModBusCliIn->readRegs(Values, GroupIn->getFirstAddr(), GroupIn->size(), Func);

                if(Res != HelperModBusClient::ERROR_RES)
                {
                    GroupIn->setValues(Values, GroupIn->size());
                }
                else
                {
                    Res = -3;
                }

                delete Values;
            }
            else
            {
                Res = -2;
            }
        }
    }

    return (Res);
}

/**
@brief  Read registers by Serial.
@param  SerialPortIn - the name (full path) of serial port,
@param  SpdIn - the speed of serial port,
@param  PrtyIn - the parity of serial port,
@param  DataBitsIn - the number of data bits of serial port,
@param  StopBitsIn - the number of stop bits of serial port,
@param  ModeIn - the mode of serial port.
@return The number of registers that had been reading.
*/
quint16 DeviceModBusRTU::readRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn, const QString &ModeIn)
{
    Log::log(QString("DeviceModBusRTU::readRegisters(SerialPort=%1,Spd=%2,Prty=%3,DataBits=%4,StopBits=%5,Mode=%6,Class=%7,BaseAddr=%8)").arg(SerialPortIn, QString::number(SpdIn), PrtyIn, QString::number(DataBitsIn), QString::number(StopBitsIn), ModeIn, mClass, QString::number(mBaseAddr)), mFileLog, mUseLog);

    quint16 Num = 0;

    if(this->isCorrect() && this->sizeListRegisters() > 0)
    {
        HelperModBusRTUClient *ModBusCli = new HelperModBusRTUClient();
        ModBusCli->mSerialPortNum        = SerialPortIn;
        ModBusCli->mSerialPortSpd        = static_cast<int>(SpdIn);
        ModBusCli->mSerialPortPrty       = HelperModBusRTUClient::convSerialPrty(PrtyIn);
        ModBusCli->mSerialPortDataBits   = static_cast<int>(DataBitsIn);
        ModBusCli->mSerialPortStopBits   = static_cast<int>(StopBitsIn);
        ModBusCli->mSerialPortMode       = HelperModBusRTUClient::convSerialMode(ModeIn);
        ModBusCli->mDevBaseAddr          = mBaseAddr;
        ModBusCli->mWaitRead             = mWaitRead;

        if(ModBusCli->connect())
        {
            RegsGroup *Group  = nullptr;
            int Res = -1;
//MUTEX LOCK
            QMutexLocker MutexLk(&mMutex);
            for(int i=0; i<mListRegsGroups.size(); i++)
            {
                Group = mListRegsGroups.at(i);

                if(Group)
                {
                    if(!(Group->size() > 0 && Group->isAllowToRead() && (Group->isClass(Register::CLASS__COIL) || Group->isClass(Register::CLASS__DISC) || Group->isClass(Register::CLASS__IN) || Group->isClass(Register::CLASS__INPT) || Group->isClass(Register::CLASS__HOLDING))))
                    {
                        continue;
                    }

                    //Read registers (DIC, COILS, INPT, HOLD)
                    Res = ((Group->isClass(Register::CLASS__COIL) || Group->isClass(Register::CLASS__DISC)) ? this->readBits(ModBusCli, Group) : this->readRegs(ModBusCli, Group));

                    if(Res == -3)
                    {
                        Log::log(QString("Error read data from '%1' (%2)! %3").arg(ModBusCli->mSerialPortNum, QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
                    }
                    else if(Res == -2)
                    {
                        Log::log(QString("Error connection with '%1' after reconnect (%2)! %3").arg(ModBusCli->mSerialPortNum, QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
                        break;
                    }
                    else if(Res == -1)
                    {
                        Log::log(QString("The class '%1' of registers is not supported!").arg(Group->getClass()), mFileLog, mUseLog, false);
                    }
                    else
                    {
                        Num+= static_cast<int>(Res);
                    }
                }
            }
//MUTEX UNLOCK
            ModBusCli->disconnect();
        }
        else
        {
            Log::log(QString("Error connection with '%1' (%2)! %3").arg(ModBusCli->mSerialPortNum, QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
        }

        delete ModBusCli;
    }
    else
    {
        Log::log(QString("The configuration is incorrect or list of registers is empty!"), mFileLog, mUseLog, false);
    }

    return (Num);
}


/**
@brief  Write registers (COILS).
@param  ModBusCliIn - pointer to ModBus RTU client,
@param  GroupIn     - pointer to group.
@return Result:
@arg      > 0 - quantity of read registers
@arg     = -1 - register class is not supported
@arg     = -2 - ModBus RTU client error connection
@arg     = -3 - ModBus RTU client error read data
*/
int DeviceModBusRTU::writeBits(HelperModBusRTUClient *ModBusCliIn, RegsGroup *GroupIn)
{
    int Res = -1;

    if(ModBusCliIn && GroupIn)
    {
        int Func = -1;

        if(GroupIn->isClass(Register::CLASS__COIL))
        {
            Func = HelperModBusClient::FUNC__WRITE_COIL_REGS;
        }

        if(GroupIn->size() > 0 && GroupIn->isAllowToWrite() && Func > 0)
        {
            if(mReconnect) ModBusCliIn->reconnect();

            if(ModBusCliIn->isConnected())
            {
                quint8  *Values = new quint8[GroupIn->size()];
                quint16 Len = GroupIn->getValues(Values, GroupIn->size());
                Res = ModBusCliIn->writeCoilRegs(Values, GroupIn->getFirstAddr(), Len);
                delete Values;
            }
            else
            {
                Res = -2;
            }
        }
    }

    return (Res);
}

/**
@brief  Write registers (HOLD).
@param  ModBusCliIn - pointer to ModBus RTU client,
@param  GroupIn     - pointer to group.
@return Result:
@arg      > 0 - quantity of read registers
@arg     = -1 - register class is not supported
@arg     = -2 - ModBus RTU client error connection
@arg     = -3 - ModBus RTU client error read data
*/
int DeviceModBusRTU::writeRegs(HelperModBusRTUClient *ModBusCliIn, RegsGroup *GroupIn)
{
    int Res = -1;

    if(ModBusCliIn && GroupIn)
    {
        int Func = -1;

        if(GroupIn->isClass(Register::CLASS__HOLDING))
        {
            Func = HelperModBusClient::FUNC__WRITE_HOLDING_REGS;
        }

        if(GroupIn->size() > 0 && GroupIn->isAllowToWrite() && Func > 0)
        {
            if(mReconnect) ModBusCliIn->reconnect();

            if(ModBusCliIn->isConnected())
            {
                quint16 *Values = new quint16[GroupIn->size()];
                quint16 Len = GroupIn->getValues(Values, GroupIn->size());
                Res = ModBusCliIn->writeHoldingRegs(Values, GroupIn->getFirstAddr(), Len);
                delete Values;
            }
            else
            {
                Res = -2;
            }
        }
    }

    return (Res);
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
quint16 DeviceModBusRTU::writeRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn, const QString &ModeIn)
{
    QJsonObject Obj;
    return (this->writeRegisters(SerialPortIn, SpdIn, PrtyIn, DataBitsIn, StopBitsIn, ModeIn, Obj));
}

/**
@brief  Write registers by Serial.
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
quint16 DeviceModBusRTU::writeRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn, const QString &ModeIn, QJsonObject &ObjIn)
{
    Log::log(QString("DeviceModBusRTU::writeRegisters(SerialPort=%1,Spd=%2,Prty=%3,DataBits=%4,StopBits=%5,Mode=%6,Class=%7,BaseAddr=%8)").arg(SerialPortIn, QString::number(SpdIn), PrtyIn, QString::number(DataBitsIn), QString::number(StopBitsIn), ModeIn, mClass, QString::number(mBaseAddr)), mFileLog, mUseLog);

    quint16 Num = 0;

    if(this->isCorrect() && this->sizeListRegisters() > 0)
    {
        HelperModBusRTUClient *ModBusCli = new HelperModBusRTUClient();
        ModBusCli->mSerialPortNum        = SerialPortIn;
        ModBusCli->mSerialPortSpd        = static_cast<int>(SpdIn);
        ModBusCli->mSerialPortPrty       = HelperModBusRTUClient::convSerialPrty(PrtyIn);
        ModBusCli->mSerialPortDataBits   = static_cast<int>(DataBitsIn);
        ModBusCli->mSerialPortStopBits   = static_cast<int>(StopBitsIn);
        ModBusCli->mSerialPortMode       = HelperModBusRTUClient::convSerialMode(ModeIn);
        ModBusCli->mDevBaseAddr          = mBaseAddr;

        if(ModBusCli->connect())
        {
            RegsGroup *Group = nullptr;
            int Res = -1;
//MUTEX LOCK
            QMutexLocker MutexLk(&mMutex);
            for(int i=0; i<mListRegsGroups.size(); i++)
            {
                Group = mListRegsGroups.at(i);

                if(Group)
                {
                    if(!(Group->size() > 0 && Group->isAllowToWrite() && (Group->isClass(Register::CLASS__COIL) || Group->isClass(Register::CLASS__HOLDING))))
                    {
                        continue;
                    }

                    //Set values
                    if(!ObjIn.isEmpty())
                    {
                        Group->setValues(ObjIn);
                    }

                    //Write registers (COILS, HOLD)
                    Res = ((Group->isClass(Register::CLASS__COIL)) ? this->writeBits(ModBusCli, Group) : this->writeRegs(ModBusCli, Group));

                    if(Res == -3)
                    {
                        Log::log(QString("Error write data from '%1' (%2)! %3").arg(ModBusCli->mSerialPortNum, QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
                    }
                    else if(Res == -2)
                    {
                        Log::log(QString("Error connection with '%1' after reconnect (%2)! %3").arg(ModBusCli->mSerialPortNum, QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
                        break;
                    }
                    else if(Res == -1)
                    {
                        Log::log(QString("The class '%1' of registers is not supported!").arg(Group->getClass()), mFileLog, mUseLog, false);
                    }
                    else
                    {
                        Num+= static_cast<int>(Res);
                    }
                }
            }
//MUTEX UNLOCK
            ModBusCli->disconnect();
        }
        else
        {
            Log::log(QString("Error connection with '%1' (%2)! %3").arg(ModBusCli->mSerialPortNum, QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
        }

        delete ModBusCli;
    }
    else
    {
        Log::log(QString("The configuration is incorrect or list of registers is empty!"), mFileLog, mUseLog, false);
    }

    return (Num);
}
