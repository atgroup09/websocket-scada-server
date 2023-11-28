/* Copyright (C) 2019-2023 ATgroup09
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

#include "device-modbus-tcp.h"


/**
@brief  Constructor.
@param  None.
@return None.
*/
DeviceModBusTCP::DeviceModBusTCP(QObject *parent) : Device(parent)
{

}

/**
@brief  Destructor.
@param  None.
@return None.
*/
DeviceModBusTCP::~DeviceModBusTCP()
{

}


/**
@brief  Read registers (DISC, COILS).
@param  ModBusCliIn - pointer to ModBus TCP client,
@param  GroupIn     - pointer to group.
@return Result:
@arg      > 0 - quantity of read registers
@arg     = -1 - register class is not supported
@arg     = -2 - ModBus TCP client error connection
@arg     = -3 - ModBus TCP client error read data
*/
int DeviceModBusTCP::readBits(HelperModBusTCPClient *ModBusCliIn, RegsGroup *GroupIn)
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
@param  ModBusCliIn - pointer to ModBus TCP client,
@param  GroupIn     - pointer to group.
@return Result:
@arg      > 0 - quantity of read registers
@arg     = -1 - register class is not supported
@arg     = -2 - ModBus TCP client error connection
@arg     = -3 - ModBus TCP client error read data
*/
int DeviceModBusTCP::readRegs(HelperModBusTCPClient *ModBusCliIn, RegsGroup *GroupIn)
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
@brief  Read registers.
@param  None.
@return The number of registers that had been reading.
*/
quint16 DeviceModBusTCP::readRegisters()
{
    Log::log(QString("DeviceModBusTCP::readRegisters(IP=%1,Port=%2,BaseAddr=%3)").arg(mIP, QString::number(mPort), QString::number(mBaseAddr)), mFileLog, mUseLog);

    quint16 Num = 0;

    if(this->isCorrectBaseAddr() && this->isCorrectIP() && this->isCorrectPort() && this->sizeListRegisters() > 0)
    {
        HelperModBusTCPClient *ModBusCli = new HelperModBusTCPClient();
        ModBusCli->mIP                   = mIP;
        ModBusCli->mPort                 = mPort;
        ModBusCli->mDevBaseAddr          = mBaseAddr;

        if(ModBusCli->connect())
        {
            RegsGroup *Group = nullptr;
            int        Res   = -1;
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
                        Log::log(QString("Error read data from '%1:%2' (%3)! %4").arg(ModBusCli->mIP, QString::number(mPort), QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
                    }
                    else if(Res == -2)
                    {
                        Log::log(QString("Error connection with '%1:%2' after reconnect (%3)! %4").arg(ModBusCli->mIP, QString::number(mPort), QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
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
            Log::log(QString("Error connection with '%1:%2' (%3)! %4").arg(ModBusCli->mIP, QString::number(mPort), QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
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
@param  ModBusCliIn - pointer to ModBus TCP client,
@param  GroupIn     - pointer to group.
@return Result:
@arg      > 0 - quantity of read registers
@arg     = -1 - register class is not supported
@arg     = -2 - ModBus TCP client error connection
@arg     = -3 - ModBus TCP client error read data
*/
int DeviceModBusTCP::writeBits(HelperModBusTCPClient *ModBusCliIn, RegsGroup *GroupIn)
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
@param  ModBusCliIn - pointer to ModBus TCP client,
@param  GroupIn     - pointer to group.
@return Result:
@arg      > 0 - quantity of read registers
@arg     = -1 - register class is not supported
@arg     = -2 - ModBus TCP client error connection
@arg     = -3 - ModBus TCP client error read data
*/
int DeviceModBusTCP::writeRegs(HelperModBusTCPClient *ModBusCliIn, RegsGroup *GroupIn)
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
@brief  Write registers.
@param  None.
@return The number of registers that had been written.
*/
quint16 DeviceModBusTCP::writeRegisters()
{
    QJsonObject Obj;
    return (this->writeRegisters(Obj));
}

/**
@brief  Write registers.
@param  ObjIn - link to JsonObject.
@return The number of registers that had been written.
@detailed ObjIn = { Var:Value, ... }
*/
quint16 DeviceModBusTCP::writeRegisters(QJsonObject &ObjIn)
{
    Log::log(QString("DeviceModBusTCP::writeRegisters(IP=%1,Port=%2,BaseAddr=%3)").arg(mIP, QString::number(mPort), QString::number(mBaseAddr)), mFileLog, mUseLog);

    quint16 Num = 0;

    if(this->isCorrectBaseAddr() && this->isCorrectIP() && this->isCorrectPort() && this->sizeListRegisters() > 0)
    {
        HelperModBusTCPClient *ModBusCli = new HelperModBusTCPClient();
        ModBusCli->mIP                   = mIP;
        ModBusCli->mPort                 = mPort;
        ModBusCli->mDevBaseAddr          = mBaseAddr;

        if(ModBusCli->connect())
        {
            RegsGroup *Group;
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
                        Log::log(QString("Error write data to '%1:%2' (%3)! %4").arg(ModBusCli->mIP, QString::number(mPort), QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
                    }
                    else if(Res == -2)
                    {
                        Log::log(QString("Error connection with '%1:%2' after reconnect (%3)! %4").arg(ModBusCli->mIP, QString::number(mPort), QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
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
            Log::log(QString("Error connection with '%1:%2' (%3)! %4").arg(ModBusCli->mIP, QString::number(mPort), QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mFileLog, mUseLog, false);
        }

        delete ModBusCli;
    }
    else
    {
        Log::log(QString("The configuration is incorrect or list of registers is empty!"), mFileLog, mUseLog, false);
    }

    return (Num);
}
