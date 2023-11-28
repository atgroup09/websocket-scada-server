/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  Serial port transfer protocol helper.
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

#include "serialport.h"


/**
@brief      Constant: the speed (baudrate) of serial port (2400, 4800, 9600, 19200, 38400, 57600, 115200).
*/
const int SerialPort::SPD__2400                = 2400;
const int SerialPort::SPD__4800                = 4800;
const int SerialPort::SPD__9600                = 9600;
const int SerialPort::SPD__19200               = 19200;
const int SerialPort::SPD__38400               = 38400;
const int SerialPort::SPD__57600               = 57600;
const int SerialPort::SPD__115200              = 115200;

/**
@brief      Constant: the parity of serial port ("No", "Even", "Odd", "Space", "Mark", "Unknown" or "None").
*/
const QString SerialPort::PRTY__NO             = QString("No");
const QString SerialPort::PRTY__EVEN           = QString("Even");
const QString SerialPort::PRTY__ODD            = QString("Odd");
const QString SerialPort::PRTY__NONE           = QString("None");
const QString SerialPort::PRTY__SPACE          = QString("Space");
const QString SerialPort::PRTY__MARK           = QString("Mark");
const QString SerialPort::PRTY__UNKNOWN        = QString("Unknown");

/**
@brief      Constant: the count of serial port data bits (5, 6, 7, 8).
*/
const quint8 SerialPort::DATABITS__5            = 5;
const quint8 SerialPort::DATABITS__6            = 6;
const quint8 SerialPort::DATABITS__7            = 7;
const quint8 SerialPort::DATABITS__8            = 8;

/**
@brief      Constant: the count of serial port stop bits (1, 2, 3 (OneAndHalfStop), -1).
*/
const quint8 SerialPort::STOPBIT__1             = 1;
const quint8 SerialPort::STOPBIT__2             = 2;
const quint8 SerialPort::STOPBIT__3             = 3;
const quint8 SerialPort::STOPBIT__UNKNOWN       = 0;

/**
@brief      Constant: the flow control ("No", "Hardware", "Software", "Unknown" or "None").
*/
const QString SerialPort::FLOWCTRL__NO          = QString("No");
const QString SerialPort::FLOWCTRL__HARDWARE    = QString("Hardware");
const QString SerialPort::FLOWCTRL__SOFTWARE    = QString("Software");
const QString SerialPort::FLOWCTRL__UNKNOWN     = QString("Unknown");
const QString SerialPort::FLOWCTRL__NONE        = QString("None");


/**
@brief      Constructor.
@param      None.
@return     None.
*/
SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    mPortSpeed       = QSerialPort::Baud9600;
    mPortDataBits    = QSerialPort::Data8;
    mPortParity      = QSerialPort::NoParity;
    mPortStopBits    = QSerialPort::OneStop;
    mPortFlowControl = QSerialPort::NoFlowControl;

    mError           = QSerialPort::NoError;
    mWaitReadyRead   = 10000;

    //connect(&mPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleMainPortErrors(QSerialPort::SerialPortError)));
    //connect(&mPort, SIGNAL(readyRead()), this, SLOT(read()));
}


/**
@brief      Destructor.
@param      None.
@return     None.
*/
SerialPort::~SerialPort()
{
    this->close();
}


/**
@brief      Method: Convert the number of serial port to system port name.
@param      PortPrefIn - the prefix of path of serial port.
@param      PortNumIn - the number of serial port (0 by default).
@return     System port name (PortPrefIn + PortNumIn or "0").
*/
QString SerialPort::comToCode(const QString &PortPrefIn, const int PortNumIn)
{
    int PortNum = ((PortNumIn >= 0) ? PortNumIn : 0);

    return ((!PortPrefIn.isEmpty()) ? QString("%1%2").arg(PortPrefIn, QString::number(PortNum)) : QString("%1").arg(QString::number(PortNum)));
}


/**
@brief      Method: Convert the number of serial port to system port name.
@param      PortNumIn - the number of serial port (0 by default).
@return     System port name ("COM0" by default).
@detailed   Prefix of serial port name is "COM"
*/
QString SerialPort::comToCode(const int PortNumIn)
{
    return (comToCode(QString("COM"), PortNumIn));
}


/**
@brief      Method: convert value "Speed" from Normal format to QSerialPort::BaudRate.
@detailed   Supported values: 2400, 4800, 9600, 19200, 38400, 57600, 115200
@param      ValueIn - value "spd" in normal format (baud).
@return     QSerialPort::BaudRate (QSerialPort::Baud9600 by default).
*/
QSerialPort::BaudRate SerialPort::spdToCode(const int ValueIn)
{
    switch(ValueIn)
    {
        case SPD__2400:

            return (QSerialPort::Baud2400);

        case SPD__4800:

            return (QSerialPort::Baud4800);

        case SPD__19200:

            return (QSerialPort::Baud19200);

        case SPD__38400:

            return (QSerialPort::Baud38400);

        case SPD__57600:

            return (QSerialPort::Baud57600);

        case SPD__115200:

            return (QSerialPort::Baud115200);
    }

    return (QSerialPort::Baud9600);
}


/**
@brief      Method: convert value "Parity" from Normal format to QSerialPort::Parity.
@detailed   Supported values: "No", "Even", "Odd", "Space", "Mark", "Unknown" or "None"
@param      ValueIn - value "prty" in normal format (string).
@return     QSerialPort::Parity (QSerialPort::NoParity by default).
*/
QSerialPort::Parity SerialPort::prtyToCode(const QString &ValueIn)
{
    if(ValueIn == PRTY__EVEN)
    {
        return (QSerialPort::EvenParity);
    }
    else if(ValueIn == PRTY__ODD)
    {
        return (QSerialPort::OddParity);
    }
    else if(ValueIn == PRTY__SPACE)
    {
        return (QSerialPort::SpaceParity);
    }
    else if(ValueIn == PRTY__MARK)
    {
        return (QSerialPort::MarkParity);
    }
    else if(ValueIn == PRTY__UNKNOWN || ValueIn == PRTY__NONE)
    {
        return (QSerialPort::UnknownParity);
    }

    return (QSerialPort::NoParity);
}


/**
@brief      Method: convert value "The count of data bits" from Normal format to QSerialPort::DataBits.
@detailed   Supported values: 5, 6, 7, 8
@param      ValueIn - value "databits" in normal format.
@return     QSerialPort::DataBits (QSerialPort::Data8 by default)..
*/
QSerialPort::DataBits SerialPort::databitsToCode(const int ValueIn)
{
    switch(ValueIn)
    {
        case DATABITS__5:

            return (QSerialPort::Data5);

        case DATABITS__6:

            return (QSerialPort::Data6);

        case DATABITS__7:

            return (QSerialPort::Data7);
    }

    return (QSerialPort::Data8);
}


/**
@brief      Method: convert value "The count of stop bits" from Normal format to QSerialPort::StopBits.
@detailed   Supported values: 1, 2, 3 (OneAndHalfStop), 0
@param      ValueIn - value "stopbit" in normal format.
@return     QSerialPort::StopBits (QSerialPort::OneStop by default).
*/
QSerialPort::StopBits SerialPort::stopbitToCode(const int ValueIn)
{
    switch(ValueIn)
    {
        case STOPBIT__2:

            return (QSerialPort::TwoStop);

        case STOPBIT__3:

            return (QSerialPort::OneAndHalfStop);

        case STOPBIT__UNKNOWN:

            return (QSerialPort::UnknownStopBits);
    }

    return (QSerialPort::OneStop);
}


/**
@brief      Method: convert value "The flow control" from Normal format to QSerialPort::FlowControl.
@detailed   Supported values: "No", "Hardware", "Software", "Unknown" or "None".
@param      ValueIn - value "The flow control" in normal format.
@return     QSerialPort::FlowControl (QSerialPort::NoFlowControl by default).
*/
QSerialPort::FlowControl SerialPort::flowctrlToCode(const QString &ValueIn)
{
    if(ValueIn == FLOWCTRL__HARDWARE)
    {
        return (QSerialPort::HardwareControl);
    }
    else if(ValueIn == FLOWCTRL__SOFTWARE)
    {
        return (QSerialPort::SoftwareControl);
    }
    else if(ValueIn == FLOWCTRL__UNKNOWN || ValueIn == FLOWCTRL__NONE)
    {
        return (QSerialPort::UnknownFlowControl);
    }

    return (QSerialPort::NoFlowControl);
}


/**
@brief      Method: check Serial port status.
@param      None.
@return     true if Serial port is opened, otherwise - false.
*/
bool SerialPort::isOpened()
{
    return (mPort.isOpen());
}


/**
@brief      Method: get last Error code.
@param      None.
@return     Last Error code.
*/
QSerialPort::SerialPortError SerialPort::getError()
{
    mError = mPort.error();

    return (mError);
}


/**
@brief      Method: get last Error as String.
@param      None.
@return     Last Error as string.
*/
QString SerialPort::getErrorString()
{
    return (mPort.errorString());
}


/**
@brief      Method: get last Input Buffer result
@param      None
@return     Last Input Buffer result.
*/
QByteArray SerialPort::getInputBuffer()
{
    return (mInputBuff);
}


/**
@brief      Method: Open connection with the Serial Port
@param      OpenModeIn - open mode.
@return     True if successful, otherwise returns False
*/
bool SerialPort::open(QIODevice::OpenMode OpenModeIn)
{
    if(this->isOpened())
    {
       this->close();
    }

    mPort.setPortName(mPortN);

    if(mPort.setBaudRate(mPortSpeed) &&
       mPort.setDataBits(mPortDataBits) &&
       mPort.setParity(mPortParity) &&
       mPort.setStopBits(mPortStopBits) &&
       mPort.setFlowControl(mPortFlowControl))
    {
        if(mPort.open(OpenModeIn))
        {
            emit sigPortOpened();
            return (true);
        }
    }

    return (false);
}


/**
@brief      Method: Open connection with the Serial Port
@param      None.
@detailed   Open mode is QIODevice::ReadOnly.
@return     True if successful, otherwise returns False
*/
bool SerialPort::open()
{
    return (this->open(QIODevice::ReadOnly));
}


/**
@brief      Method: Close connection with the Serial Port
@param      None.
@return     None.
*/
void SerialPort::close()
{
    if(this->isOpened())
    {
        mPort.close();
        emit sigPortClosed();
    }
}


/**
@brief      Method: Write data to Serial Port
@param      DataIn - data to write.
@return     The number of bytes that were actually written, or -1 if an error
*/
qint64 SerialPort::write(const QByteArray &DataIn)
{
    qint64 NumBytes = 0;

    if(this->isOpened()) NumBytes = mPort.write(DataIn);

    return (NumBytes);
}


/**
@brief      Method: Read data from Serial Port
@param      None.
@return     QByteArray (empty if error)
*/
QByteArray SerialPort::read()
{
    mInputBuff.clear();

    if(this->isOpened())
    {
        qint16 WaitReadyRead = ((mWaitReadyRead > 0) ? mWaitReadyRead : 1000);
        mInputBuff = mPort.readAll();

        while(mPort.waitForReadyRead(WaitReadyRead)) mInputBuff += mPort.readAll();

        emit sigPortReadyRead(mInputBuff);
    }

    return (mInputBuff);
}


/**
@brief      Method: Handler of Serial Port errors
@param      ErrorCodeIn - Serial Port Error code.
@return     None.
*/
void SerialPort::handleMainPortErrors(QSerialPort::SerialPortError ErrorCodeIn)
{
    mError = ErrorCodeIn;

    emit sigPortError(ErrorCodeIn, mPort.errorString().toLocal8Bit());
}


/**
@brief      Method: Handler of Serial Port "ReadyRead" signal
@param      None.
@return     None.
*/
void SerialPort::handleMainPortReadyRead()
{
    emit sigPortReadyRead(mPort.readAll());
}
