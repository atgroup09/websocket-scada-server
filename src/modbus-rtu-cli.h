/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  ModBus RTU data protocol helper.
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

#ifndef HELPERMODBUSRTUCLIENT_H
#define HELPERMODBUSRTUCLIENT_H

#include "modbus-cli.h"


/**
@brief      ModBus Serial(RTU)
@detailed   Used limmodbus.dll.
*/
class HelperModBusRTUClient : public HelperModBusClient
{
    Q_OBJECT

public:

    /**
    @brief      Constructor.
    @param      None.
    @return     None.
    */
    explicit HelperModBusRTUClient(QObject *parent = nullptr);

    /**
    @brief      Destructor.
    @detailed   Close serial port.
    @param      None.
    @return     None.
    */
    virtual ~HelperModBusRTUClient();


    /**
    Public options
    */

    /**
    @brief      Constant: the speed (baudrate) of serial port (2400, 4800, 9600, 14400, 19200, 28800, 38800, 57600, 115200).
    */
    static const int SERIAL_PORT_SPD__2400;
    static const int SERIAL_PORT_SPD__4800;
    static const int SERIAL_PORT_SPD__9600;
    static const int SERIAL_PORT_SPD__14400;
    static const int SERIAL_PORT_SPD__19200;
    static const int SERIAL_PORT_SPD__28800;
    static const int SERIAL_PORT_SPD__38800;
    static const int SERIAL_PORT_SPD__57600;
    static const int SERIAL_PORT_SPD__115200;

    /**
    @brief      Constant: the parity of serial port ("N", "E", "O").
    */
    static const char SERIAL_PORT_PRTY__NONE;
    static const char SERIAL_PORT_PRTY__EVEN;
    static const char SERIAL_PORT_PRTY__ODD;

    /**
    @brief      Constant: the count of serial port data bits (7, 8).
    */
    static const int SERIAL_PORT_DATABITS__7;
    static const int SERIAL_PORT_DATABITS__8;

    /**
    @brief      Constant: the count of serial port stop bits (1, 2).
    */
    static const int SERIAL_PORT_STOPBITS__1;
    static const int SERIAL_PORT_STOPBITS__2;

    /**
    @brief      Constant: the mode of serial port.
    */
    static const int SERIAL_PORT_MODE__RS232;
    static const int SERIAL_PORT_MODE__RS485;

    /**
    @brief      Constant: serial port number prefix.
    */
    static const QString SERIAL_PORT_NUM_PREF;


    /**
    @brief      Option: the number of serial port.
    @detailed   "COM1" by default.
    */
    QString mSerialPortNum;

    /**
    @brief      Option: the speed of serial port.
    @detailed   SERIAL_PORT_SPD__9600 by default.
    */
    int mSerialPortSpd;

    /**
    @brief      Option: the parity of serial port.
    @detailed   SERIAL_PORT_PRTY__NONE by default.
    */
    char mSerialPortPrty;

    /**
    @brief      Option: the count of serial port data bits.
    @detailed   SERIAL_PORT_DATABITS__8 by default.
    */
    int mSerialPortDataBits;

    /**
    @brief      Option: the count of serial port stop bits.
    @detailed   SERIAL_PORT_STOPBITS__1 by default.
    */
    int mSerialPortStopBits;

    /**
    @brief      Option: the mode of serial port.
    @detailed   SERIAL_PORT_MODE__RS232 by default.
    */
    int mSerialPortMode;


    /**
    Public methods
    */

    /**
    @brief      Method: Convert the number of serial port to system port name.
    @param      PortPrefIn - the prefix of path of serial port.
    @param      PortNumIn - the number of serial port (0 by default).
    @return     System port name (PortPrefIn + PortNumIn or "0").
    @detailed   Prefix: on Windows, it’s necessary to prepend COM name with "\\.\" for COM number greater than 9, eg. "\\\\.\\COM10"
                        on Linux - "/dev/ttyUSB" or "/dev/ttyS"
    */
    static QString convSerialNum(const QString &PortPrefIn, const int PortNumIn);

    /**
    @brief      Method: Convert the number of serial port to system port name.
    @param      PortNumIn - the number of serial port (0 by default).
    @return     System port name ("COM0" by default).
    @detailed   Prefix of serial port name is "COM" or "\\\\.\\COM" for COM number greater than 9
    */
    static QString convSerialNum(const int PortNumIn);

    /**
    @brief      Method: Convert value of serial parity from QString to Char.
    @param      PrtyIn - link to value of parity as QString.
    @return     value of parity as Char.
    */
    static char convSerialPrty(const QString &PrtyIn);

    /**
    @brief      Method: Convert value of serial mode from QString ("RS232", RS485) to Char.
    @param      ModeIn - link to value of mode as QString.
    @return     value of mode as number.
    */
    static int convSerialMode(const QString &ModeIn);


public slots:

    /**
    Public slots
    */

    /**
    @brief      Public slot: Init.
    @param      None.
    @return     True if initialization is performed, otherwise - false.
    */
    bool init();
};

#endif // HELPERMODBUSRTUCLIENT_H
