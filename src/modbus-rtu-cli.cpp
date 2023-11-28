/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  ModBus RTU data protocol helher.
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

#include "modbus-rtu-cli.h"


/**
@brief      Constant: the speed (baudrate) of serial port
*/
const int HelperModBusRTUClient::SERIAL_PORT_SPD__2400         = 2400;
const int HelperModBusRTUClient::SERIAL_PORT_SPD__4800         = 4800;
const int HelperModBusRTUClient::SERIAL_PORT_SPD__9600         = 9600;
const int HelperModBusRTUClient::SERIAL_PORT_SPD__14400        = 14400;
const int HelperModBusRTUClient::SERIAL_PORT_SPD__19200        = 19200;
const int HelperModBusRTUClient::SERIAL_PORT_SPD__28800        = 28800;
const int HelperModBusRTUClient::SERIAL_PORT_SPD__38800        = 38800;
const int HelperModBusRTUClient::SERIAL_PORT_SPD__57600        = 57600;
const int HelperModBusRTUClient::SERIAL_PORT_SPD__115200       = 115200;

/**
@brief      Constant: the parity of serial port ("N", "E", "O").
*/
const char HelperModBusRTUClient::SERIAL_PORT_PRTY__NONE       = 'N';
const char HelperModBusRTUClient::SERIAL_PORT_PRTY__EVEN       = 'E';
const char HelperModBusRTUClient::SERIAL_PORT_PRTY__ODD        = 'O';

/**
@brief      Constant: the count of serial port data bits (7, 8).
*/
const int HelperModBusRTUClient::SERIAL_PORT_DATABITS__7       = 7;
const int HelperModBusRTUClient::SERIAL_PORT_DATABITS__8       = 8;

/**
@brief      Constant: the count of serial port stop bits (1, 2).
*/
const int HelperModBusRTUClient::SERIAL_PORT_STOPBITS__1       = 1;
const int HelperModBusRTUClient::SERIAL_PORT_STOPBITS__2       = 2;

/**
@brief      Constant: the mode of serial port.
*/
const int HelperModBusRTUClient::SERIAL_PORT_MODE__RS232       =  MODBUS_RTU_RS232;
const int HelperModBusRTUClient::SERIAL_PORT_MODE__RS485       =  MODBUS_RTU_RS485;

/**
@brief      Constant: the serial port number prefix.
*/
const QString HelperModBusRTUClient::SERIAL_PORT_NUM_PREF      = QString("COM");


/**
@brief      Constructor.
@param      None.
@return     None.
*/
HelperModBusRTUClient::HelperModBusRTUClient(QObject *parent) : HelperModBusClient(parent)
{
    mSerialPortNum      = QString("COM1");
    mSerialPortSpd      = SERIAL_PORT_SPD__9600;
    mSerialPortPrty     = SERIAL_PORT_PRTY__NONE;
    mSerialPortDataBits = SERIAL_PORT_DATABITS__8;
    mSerialPortStopBits = SERIAL_PORT_STOPBITS__1;
    mSerialPortMode     = SERIAL_PORT_MODE__RS232;
}


/**
@brief      Destructor.
@detailed   Close connection.
@param      None.
@return     None.
*/
HelperModBusRTUClient::~HelperModBusRTUClient()
{

}


/**
@brief      Public slot: Init.
@param      None.
@return     True if initialization is performed, otherwise - false.
*/
bool HelperModBusRTUClient::init()
{
    this->disconnect();

    mCtx    = modbus_new_rtu(mSerialPortNum.toUtf8().data(), mSerialPortSpd, mSerialPortPrty, mSerialPortDataBits, mSerialPortStopBits);
    mInited = ((mCtx != nullptr) ? true : false);

    if(mInited)
    {
        int _Mode = ((mSerialPortMode == SERIAL_PORT_MODE__RS485) ? SERIAL_PORT_MODE__RS485 : SERIAL_PORT_MODE__RS232);
        modbus_rtu_set_serial_mode(mCtx, _Mode);

        emit sigInited();
    }
    else
    {
        int ErrNo = this->getErrorNo();
        QString ErrStr = this->getError();
        emit sigError(ErrNo, ErrStr);
    }

    return (this->isInited());
}


/**
@brief      Method: Convert the number of serial port to system port name.
@param      PortPrefIn - the prefix of path of serial port.
@param      PortNumIn - the number of serial port (0 by default).
@return     System port name (PortPrefIn + PortNumIn or "0").
@detailed   Prefix: on Windows, it’s necessary to prepend COM name with "\\.\" for COM number greater than 9, eg. "\\\\.\\COM10"
                    on Linux - "/dev/ttyUSB" or "/dev/ttyS"
*/
QString HelperModBusRTUClient::convSerialNum(const QString &PortPrefIn, const int PortNumIn)
{
    int PortNum = ((PortNumIn >= 0) ? PortNumIn : 0);

    return ((!PortPrefIn.isEmpty()) ? QString("%1%2").arg(PortPrefIn, QString::number(PortNum)) : QString("%1").arg(QString::number(PortNum)));
}


/**
@brief      Method: Convert the number of serial port to system port name.
@param      PortNumIn - the number of serial port (0 by default).
@return     System port name ("COM0" by default).
@detailed   Prefix of serial port name is "COM" or "\\\\.\\COM" for COM number greater than 9
*/
QString HelperModBusRTUClient::convSerialNum(const int PortNumIn)
{
    return ((PortNumIn > 9) ? convSerialNum(QString("\\\\.\\%1").arg(SERIAL_PORT_NUM_PREF), PortNumIn) : convSerialNum(SERIAL_PORT_NUM_PREF, PortNumIn));
}


/**
@brief      Method: Convert value of serial parity from QString to Char.
@param      PrtyIn - link to value of parity as QString.
@return     value of parity as Char ('N' by default).
*/
char HelperModBusRTUClient::convSerialPrty(const QString &PrtyIn)
{
    return ((!PrtyIn.isEmpty()) ? PrtyIn.toLatin1().at(0) : HelperModBusRTUClient::SERIAL_PORT_PRTY__NONE);
}


/**
@brief      Method: Convert value of serial mode from QString ("RS232", RS485) to Char.
@param      ModeIn - link to value of mode as QString.
@return     value of mode as number.
*/
int HelperModBusRTUClient::convSerialMode(const QString &ModeIn)
{
    return ((ModeIn == "RS485") ? SERIAL_PORT_MODE__RS485 : SERIAL_PORT_MODE__RS232);
}
