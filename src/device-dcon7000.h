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

#ifndef DEVICE_DCON7000_H
#define DEVICE_DCON7000_H

#include "serialport.h"
#include "dcon7000.h"
#include "device.h"


/**
@brief Device of ICP DAS i7000 series (DCON).
*/
class DeviceDCON7000 : public Device
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit DeviceDCON7000(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~DeviceDCON7000();


    /**
    Public methods
    */

    /**
    @brief  Read registers by Serial.
    @param  SerialPortIn - the name (full path) of serial port,
    @param  SpdIn - the speed of serial port,
    @param  PrtyIn - the parity of serial port,
    @param  DataBitsIn - the number of data bits of serial port,
    @param  StopBitsIn - the number of stop bits of serial port,
    @return The number of registers that had been reading.
    */
    quint16 readRegisters(const QString &SerialPortIn, const quint32 SpdIn, const QString &PrtyIn, const quint8 DataBitsIn, const quint8 StopBitsIn);


private:

    /**
    Private methods
    */

    /**
    @brief  Read group by Serial.
    @param  SerialPortIn - the pointer to object of opened SerialPort,
    @param  GroupIn - the pointer to group.
    @return The number of registers that had been reading.
    */
    quint16 readGroup(SerialPort *SerialPortIn, RegsGroup *GroupIn);

    /**
    @brief  Send command AA6.
    @param  SerialPortIn - the pointer to object of opened SerialPort
    @param  RegisterIn - pointer to register.
    @return Word.
    */
    quint16 cmdAA6(SerialPort *SerialPortIn, Register *RegisterIn);
};

#endif // DEVICE_DCON7000_H
