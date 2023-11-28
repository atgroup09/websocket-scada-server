/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  ModBus data protocol helper.
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

#include "modbus-cli.h"


/**
@brief      Constant: the functions.
*/
const int HelperModBusClient::FUNC__READ_COIL_REGS       = 1;
const int HelperModBusClient::FUNC__READ_DISC_REGS       = 2;
const int HelperModBusClient::FUNC__READ_HOLDING_REGS    = 3;
const int HelperModBusClient::FUNC__READ_INPUT_REGS      = 4;
const int HelperModBusClient::FUNC__WRITE_COIL_REGS      = 15;
const int HelperModBusClient::FUNC__WRITE_HOLDING_REGS   = 16;


/**
@brief  Constructor.
@param  None.
@return None.
*/
HelperModBusClient::HelperModBusClient(QObject *parent) : QObject(parent)
{
    mDevBaseAddr        = 1;
    mWaitRead           = 1000;
    mCtx                = nullptr;
    mInited             = false;
    mConnected          = false;
}


/**
@brief  Destructor.
@param  None.
@return None.
@detailed Close serial port.
*/
HelperModBusClient::~HelperModBusClient()
{
    this->disconnect();
}


/**
@brief  Check init. status.
@param  None.
@return True if initialization is performed, otherwise - false.
*/
bool HelperModBusClient::isInited()
{
     return (mInited);
}


/**
@brief  Check connection.
@param  None.
@return True if connection was established, otherwise - false.
*/
bool HelperModBusClient::isConnected()
{
    return (mConnected);
}


/**
@brief  Get last Error code.
@param  None.
@return Last Error code.
*/
int HelperModBusClient::getErrorNo()
{
    return ((this->isInited()) ? errno : 0);
}


/**
@brief  Get Error message.
@param  None.
@return Error string.
@detailed If no errors, then returns empty string
*/
QString HelperModBusClient::getError()
{
    return ((this->isInited()) ? QString(modbus_strerror(errno)) : QString(""));
}


/**
@brief  Init.
@param  None.
@return True if initialization is performed, otherwise - false.
*/
bool HelperModBusClient::init()
{
    return (this->isInited());
}


/**
@brief  Establish connection.
@param  None.
@return True if connection was established successfully, otherwise - false.
*/
bool HelperModBusClient::connect()
{
    if(this->init())
    {
        modbus_set_slave(mCtx, ((mDevBaseAddr > 0) ? mDevBaseAddr : 1));

        if(mWaitRead > 0)
        {
            quint32 Sec  = ((mWaitRead < 1000) ? static_cast<quint32>(0) : static_cast<quint32>(mWaitRead/1000));
            quint32 uSec = ((mWaitRead < 1000) ? static_cast<quint32>(mWaitRead*1000) : static_cast<quint32>(0));

#ifdef LIBMODBUS_OLD
            struct timeval ResponseTimeout;
            ResponseTimeout.tv_sec  = static_cast<long>(Sec);
            ResponseTimeout.tv_usec = static_cast<long>(uSec);
            modbus_set_response_timeout(mCtx, &ResponseTimeout);
#else
            modbus_set_response_timeout(mCtx, Sec, uSec);
#endif
        }

        mConnected = ((modbus_connect(mCtx) == 0) ? true : false);
    }

    if(mConnected)
    {
        emit sigConnected();
    }
    else
    {
        int ErrNo = this->getErrorNo();
        QString ErrStr = this->getError();
        emit sigError(ErrNo, ErrStr);
    }

    return (this->isConnected());
}


/**
@brief  Close connection.
@param  None.
@return None.
*/
void HelperModBusClient::disconnect()
{
    if(this->isInited())
    {
        if(mConnected) modbus_close(mCtx);
        modbus_free(mCtx);
        mCtx = nullptr;
    }

    mInited    = false;
    mConnected = false;

    emit sigDisconnected();
}


/**
@brief  ReConnect.
@param  None.
@return True if connection was established successfully, otherwise - false.
*/
bool HelperModBusClient::reconnect()
{
    this->disconnect();
    return (this->connect());
}


/**
@brief Read bits.
@param RawDataIn - pointer to array to save result;
@param RegAddrIn - the address of first register of the data block;
@param RegNbIn - the number of register to read;
@param FuncIn - the function code.
@return The number of read registers if successful, otherwise -1
@detailed Read registers, functon codes: 1, 2
*/
int HelperModBusClient::readBits(uint8_t *RawDataIn, int RegAddrIn, int RegNbIn, int FuncIn)
{
    int _Res = -1;

    if(this->isConnected())
    {
        switch(FuncIn)
        {
            case FUNC__READ_COIL_REGS:
                _Res = modbus_read_bits(mCtx, RegAddrIn, RegNbIn, RawDataIn);
                break;

            case FUNC__READ_DISC_REGS:
                _Res = modbus_read_input_bits(mCtx, RegAddrIn, RegNbIn, RawDataIn);
                break;

            default:
                return (_Res);
        }

        if(_Res != ERROR_RES)
        {
            emit sigReadCompleted();
        }
        else
        {
            int ErrNo = this->getErrorNo();
            QString ErrStr = this->getError();
            emit sigError(ErrNo, ErrStr);
        }
    }

    return (_Res);
}


/**
@brief Read registers.
@param RawDataIn - pointer to array to save result;
@param RegAddrIn - the address of first register of the data block;
@param RegNbIn - the number of register to read;
@param FuncIn - the function code (0x04 by default).
@return The number of read registers if successful, otherwise -1
@detailed Read registers, functon codes: 3, 4.
*/
int HelperModBusClient::readRegs(uint16_t *RawDataIn, int RegAddrIn, int RegNbIn, int FuncIn)
{
    int _Res = -1;

    if(this->isConnected())
    {
        switch(FuncIn)
        {
            case FUNC__READ_HOLDING_REGS:
                _Res = modbus_read_registers(mCtx, RegAddrIn, RegNbIn, RawDataIn);
                break;

            case FUNC__READ_INPUT_REGS:
                _Res = modbus_read_input_registers(mCtx, RegAddrIn, RegNbIn, RawDataIn);
                break;

            default:
                return (_Res);
        }

        if(_Res != ERROR_RES)
        {
            emit sigReadCompleted();
        }
        else
        {
            int ErrNo = this->getErrorNo();
            QString ErrStr = this->getError();
            emit sigError(ErrNo, ErrStr);
        }
    }

    return (_Res);
}


/**
@brief  Read Coil-registers.
@param  RawDataIn - pointer to array to save result;
@param  RegAddrIn - the address of first register of the data block;
@param  RegNbIn - the number of register to read.
@return The number of read registers if successful, otherwise -1
@detailed Read coil registers, functon code 0x01 (read)
*/
int HelperModBusClient::readCoilRegs(uint8_t *RawDataIn, int RegAddrIn, int RegNbIn)
{
    return (this->readBits(RawDataIn, RegAddrIn, RegNbIn, FUNC__READ_COIL_REGS));
}


/**
@brief  Write Coil-registers.
@param  RawDataIn - pointer to data array;
@param  RegAddrIn - the address of first register of the data block;
@param  RegNbIn - the number of registers to write.
@return The number of read registers if successful, otherwise -1
@detailed Write coil registers, functon code 0x0F (write)
*/
int HelperModBusClient::writeCoilRegs(uint8_t *RawDataIn, int RegAddrIn, int RegNbIn)
{
    int _Res = -1;

    if(this->isConnected())
    {
        _Res = modbus_write_bits(mCtx, RegAddrIn, RegNbIn, RawDataIn);

        if(_Res != ERROR_RES)
        {
            emit sigWriteCompleted();
        }
        else
        {
            int ErrNo = this->getErrorNo();
            QString ErrStr = this->getError();
            emit sigError(ErrNo, ErrStr);
        }
    }

    return (_Res);
}


/**
@brief  Read DicreteInput-registers.
@param  RawDataIn - pointer to array to save result;
@param  RegAddrIn - the address of first register of the data block;
@param  RegNbIn - the number of register to read.
@return The number of read registers if successful, otherwise -1
@detailed Read dicrete input registers, functon code 0x02 (read)
*/
int HelperModBusClient::readDiscRegs(uint8_t *RawDataIn, int RegAddrIn, int RegNbIn)
{
    return (this->readBits(RawDataIn, RegAddrIn, RegNbIn, FUNC__READ_DISC_REGS));
}


/**
@brief  Write Holding-registers.
@param  RawDataIn - pointer to array to save result;
@param  RegAddrIn - the address of first register of the data block;
@param  RegNbIn - the number of register to read.
@return The number of written registers if successful, otherwise -1
@detailed Write holding registers, functon code 0x10 (write)
*/
int HelperModBusClient::writeHoldingRegs(uint16_t *RawDataIn, int RegAddrIn, int RegNbIn)
{
    int _Res = -1;

    if(this->isConnected())
    {
        _Res = modbus_write_registers(mCtx, RegAddrIn, RegNbIn, RawDataIn);

        if(_Res != ERROR_RES)
        {
            emit sigWriteCompleted();
        }
        else
        {
            int ErrNo = this->getErrorNo();
            QString ErrStr = this->getError();
            emit sigError(ErrNo, ErrStr);
        }
    }

    return (_Res);
}


/**
@brief  Read Holding-registers.
@param  RawDataIn - pointer to array to save result;
@param  RegAddrIn - the address of first register of the data block;
@param  RegNbIn - the number of register to read.
@return The number of read registers if successful, otherwise -1
@detailed Read holding registers, functon code 0x03 (read)
*/
int HelperModBusClient::readHoldingRegs(uint16_t *RawDataIn, int RegAddrIn, int RegNbIn)
{
    return (this->readRegs(RawDataIn, RegAddrIn, RegNbIn, FUNC__READ_HOLDING_REGS));
}


/**
@brief  Read Input-registers.
@param  RawDataIn - pointer to array to save result;
@param  RegAddrIn - the address of first register of the data block;
@param  RegNbIn - the number of register to read.
@return The number of read registers if successful, otherwise -1
@detailed Read input registers, functon code 0x04 (read)
*/
int HelperModBusClient::readInputRegs(uint16_t *RawDataIn, int RegAddrIn, int RegNbIn)
{
    return (this->readRegs(RawDataIn, RegAddrIn, RegNbIn, FUNC__READ_INPUT_REGS));
}


/**
@brief  Set Device base address.
@param  DevBaseAddrIn - new device base address.
@return None
*/
void HelperModBusClient::setDevBaseAddr(int DevBaseAddrIn)
{
    int _DevBaseAddr = ((DevBaseAddrIn > 0) ? DevBaseAddrIn : 1);
    if(mCtx != nullptr) modbus_set_slave(mCtx, _DevBaseAddr);
}
