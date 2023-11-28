/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  MySQL data protocol helper.
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

#ifndef HELPERMODBUSCLIENT_H
#define HELPERMODBUSCLIENT_H

#include <iostream>
#include <QObject>
#include <QString>

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <winsock2.h>
#include <windows.h>
#endif

#include "lib/modbus/include/modbus.h"

#include "global.h"


/**
@brief      ModBus Client
@detailed   Used limmodbus.dll.
*/
class HelperModBusClient : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit HelperModBusClient(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    @detailed Close serial port.
    */
    virtual ~HelperModBusClient();


    /**
    Public options
    */

    /**
    @brief The functions.
    */
    static const int FUNC__READ_HOLDING_REGS;
    static const int FUNC__READ_INPUT_REGS;
    static const int FUNC__READ_COIL_REGS;
    static const int FUNC__READ_DISC_REGS;
    static const int FUNC__WRITE_COIL_REGS;
    static const int FUNC__WRITE_HOLDING_REGS;

    /**
    @brief Error result.
    */
    static const int ERROR_RES = -1;

    /**
    @brief The device base address.
    */
    int mDevBaseAddr;

    /**
    @brief      Option: The waiting time for ReadyRead-signal
    @detailed -1 is 1000 msec by default
    */
    qint16 mWaitRead;


    /**
    Public methods
    */

    /**
    @brief  Check init. status.
    @param  None.
    @return True if initialization is performed, otherwise - false.
    */
    bool isInited();

    /**
    @brief  Check connection.
    @param  None.
    @return True if connection was established, otherwise - false.
    */
    bool isConnected();

    /**
    @brief  Get last Error code.
    @param  None.
    @return Last Error code.
    */
    int getErrorNo();

    /**
    @brief  Get Error message.
    @param  None.
    @return Error string.
    @detailed If no errors, then returns empty string
    */
    QString getError();


signals:

    /**
    @brief  Initialization is performed.
    @param  None.
    @return None.
    */
    void sigInited();

    /**
    @brief  Connection was established successfully.
    @param  None.
    @return None.
    */
    void sigConnected();

    /**
    @brief  Connection was closed successfully.
    @param  None.
    @return None.
    */
    void sigDisconnected();

    /**
    @brief  Error.
    @param  ErrorNoIn - error code;
    @param  ErrorStrIn - error string.
    @return None.
    */
    void sigError(int ErrorNoIn, QString ErrorStrIn);

    /**
    @brief  Reading completed
    @param  None.
    @return None.
    */
    void sigReadCompleted();

    /**
    @brief  Writing completed
    @param  None.
    @return None.
    */
    void sigWriteCompleted();


public slots:

    /**
    Public slots
    */

    /**
    @brief  Init.
    @param  None.
    @return True if initialization is performed, otherwise - false.
    */
    virtual bool init();

    /**
    @brief  Establish connection.
    @param  None.
    @return True if connection was established successfully, otherwise - false.
    */
    bool connect();

    /**
    @brief  Close connection.
    @param  None.
    @return None.
    */
    void disconnect();

    /**
    @brief  ReConnect.
    @param  None.
    @return True if connection was established successfully, otherwise - false.
    */
    bool reconnect();

    /**
    @brief  Write Coil-registers.
    @param  RawDataIn - pointer to data array;
    @param  RegAddrIn - the address of first register of the data block;
    @param  RegNbIn - the number of registers to write.
    @return The number of written registers if successful, otherwise -1
    @detailed Write coil registers, functon code 0x0F (write)
    */
    int writeCoilRegs(uint8_t *RawDataIn, int RegAddrIn, int RegNbIn);

    /**
    @brief  Read Coil-registers.
    @param  RawDataIn - pointer to array to save result;
    @param  RegAddrIn - the address of first register of the data block;
    @param  RegNbIn - the number of register to read.
    @return The number of read registers if successful, otherwise -1
    @detailed Read coil registers, functon code 0x01 (read)
    */
    int readCoilRegs(uint8_t *RawDataIn, int RegAddrIn, int RegNbIn);

    /**
    @brief  Read DicreteInput-registers.
    @param  RawDataIn - pointer to array to save result;
    @param  RegAddrIn - the address of first register of the data block;
    @param  RegNbIn - the number of register to read.
    @return The number of read registers if successful, otherwise -1
    @detailed Read dicrete input registers, functon code 0x02 (read)
    */
    int readDiscRegs(uint8_t *RawDataIn, int RegAddrIn, int RegNbIn);

    /**
    @brief  Write Holding-registers.
    @param  RawDataIn - pointer to array to save result;
    @param  RegAddrIn - the address of first register of the data block;
    @param  RegNbIn - the number of register to read.
    @return The number of written registers if successful, otherwise -1
    @detailed Write holding registers, functon code 0x10 (write)
    */
    int writeHoldingRegs(uint16_t *RawDataIn, int RegAddrIn, int RegNbIn);

    /**
    @brief  Read Holding-registers.
    @param  RawDataIn - pointer to array to save result;
    @param  RegAddrIn - the address of first register of the data block;
    @param  RegNbIn - the number of register to read.
    @return The number of read registers if successful, otherwise -1
    @detailed Read holding registers, functon code 0x03 (read)
    */
    int readHoldingRegs(uint16_t *RawDataIn, int RegAddrIn, int RegNbIn);

    /**
    @brief  Read Input-registers.
    @param  RawDataIn - pointer to array to save result;
    @param  RegAddrIn - the address of first register of the data block;
    @param  RegNbIn - the number of register to read.
    @return The number of read registers if successful, otherwise -1
    @detailed Read input registers, functon code 0x04 (read)
    */
    int readInputRegs(uint16_t *RawDataIn, int RegAddrIn, int RegNbIn);

    /**
    @brief Read registers.
    @param RawDataIn - pointer to array to save result;
    @param RegAddrIn - the address of first register of the data block;
    @param RegNbIn - the number of register to read;
    @param FuncIn - the function code.
    @return The number of read registers if successful, otherwise -1
    @detailed Read registers, functon codes: 3, 4
    */
    int readRegs(uint16_t *RawDataIn, int RegAddrIn, int RegNbIn, int FuncIn);

    /**
    @brief Read bits.
    @param RawDataIn - pointer to array to save result;
    @param RegAddrIn - the address of first register of the data block;
    @param RegNbIn - the number of register to read;
    @param FuncIn - the function code.
    @return The number of read registers if successful, otherwise -1
    @detailed Read registers, functon codes: 1, 2
    */
    int readBits(uint8_t *RawDataIn, int RegAddrIn, int RegNbIn, int FuncIn);

    /**
    @brief  Set Device base address.
    @param  DevBaseAddrIn - new device base address.
    @return None
    */
    void setDevBaseAddr(int DevBaseAddrIn);


protected:

    /**
    Protected options
    */

    /**
    @brief Context.
    */
    modbus_t *mCtx;

    /**
    @brief Init. status
    */
    bool mInited;

    /**
    @brief Connection status
    */
    bool mConnected;
};

#endif // HELPERMODBUSCLIENT_H
