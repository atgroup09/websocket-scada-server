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

#ifndef DEVICE_MODBUS_TCP_H
#define DEVICE_MODBUS_TCP_H

#include "modbus-tcp-cli.h"
#include "device.h"


/**
@brief ModBusTCP Device.
*/
class DeviceModBusTCP : public Device
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit DeviceModBusTCP(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~DeviceModBusTCP();


    /**
    Public methods
    */

    /**
    @brief  Read registers.
    @param  None.
    @return The number of registers that had been reading.
    */
    quint16 readRegisters();

    /**
    @brief  Write registers.
    @param  None.
    @return The number of registers that had been written.
    */
    quint16 writeRegisters();

    /**
    @brief  Write registers.
    @param  ObjIn - link to JsonObject.
    @return The number of registers that had been written.
    @detailed @detailed ObjIn = { Var:Value, ... }
    */
    quint16 writeRegisters(QJsonObject &ObjIn);

private:

    /**
    Private methods
    */

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
    int readBits(HelperModBusTCPClient *ModBusCliIn, RegsGroup *GroupIn);

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
    int writeBits(HelperModBusTCPClient *ModBusCliIn, RegsGroup *GroupIn);

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
    int readRegs(HelperModBusTCPClient *ModBusCliIn, RegsGroup *GroupIn);

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
    int writeRegs(HelperModBusTCPClient *ModBusCliIn, RegsGroup *GroupIn);
};

#endif // DEVICE_MODBUS_TCP_H
