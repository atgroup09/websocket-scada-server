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

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <iostream>
#include <QObject>
#include <QtGlobal>
#include <QByteArray>
#include <QIODevice>
#include <QtSerialPort/QSerialPort>


/**
@brief   Serial port.
*/
class SerialPort : public QObject
{
    Q_OBJECT

public:

    explicit SerialPort(QObject *parent = nullptr);
    ~SerialPort();


    /**
    Public options
    */

    /**
    @brief  Speed (baudrate) (2400, 4800, 9600, 19200, 38400, 57600, 115200).
    */
    static const int SPD__2400;
    static const int SPD__4800;
    static const int SPD__9600;
    static const int SPD__19200;
    static const int SPD__38400;
    static const int SPD__57600;
    static const int SPD__115200;

    /**
    @brief Parity ("No", "Even", "Odd", "Space", "Mark", "Unknown" or "None").
    */
    static const QString PRTY__NO;
    static const QString PRTY__EVEN;
    static const QString PRTY__ODD;
    static const QString PRTY__NONE;
    static const QString PRTY__SPACE;
    static const QString PRTY__MARK;
    static const QString PRTY__UNKNOWN;

    /**
    @brief Data bits (5, 6, 7, 8).
    */
    static const quint8 DATABITS__5;
    static const quint8 DATABITS__6;
    static const quint8 DATABITS__7;
    static const quint8 DATABITS__8;

    /**
    @brief Stop bits (1, 2, 3 (OneAndHalfStop), 0).
    */
    static const quint8 STOPBIT__1;
    static const quint8 STOPBIT__2;
    static const quint8 STOPBIT__3;
    static const quint8 STOPBIT__UNKNOWN;

    /**
    @brief Flow control ("No", "Hardware", "Software", "Unknown" or "None").
    */
    static const QString FLOWCTRL__NO;
    static const QString FLOWCTRL__HARDWARE;
    static const QString FLOWCTRL__SOFTWARE;
    static const QString FLOWCTRL__UNKNOWN;
    static const QString FLOWCTRL__NONE;

    /**
    @brief      Option: Name of Serial Port
    @detailed   Windows: COM1 ... COMn
                Linux: /dev/ttyUSBn ...
    */
    QString mPortN;

    /**
    @brief      Option: Baud Rate (QSerialPort::Baud9600 by default)
    */
    QSerialPort::BaudRate mPortSpeed;

    /**
    @brief      Option: The parity checking mode (QSerialPort::NoParity by default)
    */
    QSerialPort::Parity mPortParity;

    /**
    @brief      Option: Data bits in a frame (QSerialPort::Data8 by default)
    */
    QSerialPort::DataBits mPortDataBits;

    /**
    @brief      Option: The number of Stop bits in a frame (QSerialPort::OneStop by default)
    */
    QSerialPort::StopBits mPortStopBits;

    /**
    @brief      Option: The desired flow control mode (QSerialPort::NoFlowControl by default)
    */
    QSerialPort::FlowControl mPortFlowControl;

    /**
    @brief      Option: The waiting time for ReadyRead-signal (10000 msec by default)
    @detailed   -1 is no timeout
    */
    qint16 mWaitReadyRead;


    /**
    Public methods
    */

    /**
    @brief      Method: Convert the number of serial port to system port name.
    @param      PortPrefIn - the prefix of path of serial port.
    @param      PortNumIn - the number of serial port (0 by default).
    @return     System port name (PortPrefIn + PortNumIn or "0").
    */
    static QString comToCode(const QString &PortPrefIn, const int PortNumIn);

    /**
    @brief      Method: Convert the number of serial port to system port name.
    @param      PortNumIn - the number of serial port (0 by default).
    @return     System port name ("COM0" by default).
    @detailed   Prefix of serial port name is "COM"
    */
    static QString comToCode(const int PortNumIn);

    /**
    @brief      Method: Convert value "Speed" from Normal format to QSerialPort::BaudRate.
    @detailed   Supported values: 2400, 4800, 9600, 14400, 19200, 28800, 38800, 57600, 115200
    @param      ValueIn - value "spd" in normal format (baud).
    @return     QSerialPort::BaudRate (QSerialPort::Baud9600 by default).
    */
    static QSerialPort::BaudRate spdToCode(const int ValueIn);

    /**
    @brief      Method: Convert value "Parity" from Normal format to QSerialPort::Parity.
    @detailed   Supported values: "None", "Even", "Odd"
    @param      ValueIn - value "prty" in normal format (string).
    @return     QSerialPort::Parity (QSerialPort::NoParity by default).
    */
    static QSerialPort::Parity prtyToCode(const QString &ValueIn);

    /**
    @brief      Method: Convert value "The count of data bits" from Normal format to QSerialPort::DataBits.
    @detailed   Supported values: 7, 8
    @param      ValueIn - value "databits" in normal format.
    @return     QSerialPort::DataBits (QSerialPort::Data8 by default)..
    */
    static QSerialPort::DataBits databitsToCode(const int ValueIn);

    /**
    @brief      Method: Convert value "The count of stop bits" from Normal format to QSerialPort::StopBits.
    @detailed   Supported values: 1, 2
    @param      ValueIn - value "stopbit" in normal format.
    @return     QSerialPort::StopBits (QSerialPort::OneStop by default)..
    */
    static QSerialPort::StopBits stopbitToCode(const int ValueIn);

    /**
    @brief      Method: Convert value "The flow control" from Normal format to QSerialPort::FlowControl.
    @detailed   Supported values: "No", "Hardware", "Software", "Unknown" or "None".
    @param      ValueIn - value "The flow control" in normal format.
    @return     QSerialPort::FlowControl (QSerialPort::NoFlowControl by default).
    */
    static QSerialPort::FlowControl flowctrlToCode(const QString &ValueIn);

    /**
    @brief      Method: Get last Error code.
    @param      None.
    @return     Last Error code.
    */
    QSerialPort::SerialPortError getError();

    /**
    @brief      Method: Get last Error as String.
    @param      None.
    @return     Last Error as string.
    */
    QString getErrorString();

    /**
    @brief      Method: Check Serial port status.
    @param      None.
    @return     true if Serial port is opened, otherwise - false.
    */
    bool isOpened();

    /**
    @brief      Method: Get last Input Buffer result
    @param      None
    @return     Last Input Buffer result.
    */
    QByteArray getInputBuffer();


signals:

    /**
    Signals
    */

    /**
    @brief      Signal: Connection with the Serial Port is opened
    @param      None.
    @return     None.
    */
    void sigPortOpened();

    /**
    @brief      Signal: Connection with the Serial Port is closed
    @param      None.
    @return     None.
    */
    void sigPortClosed();

    /**
    @brief      Signal: Serial Port error signal
    @param      ErrorCodeIn - error code;
                ErrorIn - error string.
    @return     None.
    */
    void sigPortError(QSerialPort::SerialPortError ErrorCodeIn, QString ErrorIn);

    /**
    @brief      Signal: Serial Port "ReadyRead" signal
    @param      DataIn - read data.
    @return     None.
    */
    void sigPortReadyRead(QByteArray DataIn);


public slots:

    /**
    Public slots
    */

    /**
    @brief      Method: Open connection with the Serial Port
    @param      OpenModeIn - open mode.
    @return     True if successful, otherwise returns False
    */
    bool open(QIODevice::OpenMode OpenModeIn);

    /**
    @brief      Method: Open connection with the Serial Port
    @param      None.
    @detailed   Open mode is QIODevice::ReadOnly.
    @return     True if successful, otherwise returns False
    */
    bool open();

    /**
    @brief      Method: Close connection with the Serial Port
    @param      None.
    @return     None.
    */
    void close();

    /**
    @brief      Method: Write data to Serial Port
    @param      DataIn - data to write.
    @return     The number of bytes that were actually written, or -1 if an error
    */
    qint64 write(const QByteArray &DataIn);

    /**
    @brief      Method: Read data from Serial Port
    @param      None.
    @return     QByteArray (empty if error)
    */
    QByteArray read();


protected:

    /**
    Protected options
    */

    /**
    @brief  Serial Port Error code
    */
    QSerialPort::SerialPortError mError;


private:

    /**
    Private options
    */

    /**
    @brief  Serial Port
    */
    QSerialPort mPort;

    /**
    @brief  Serial Port Input Buffer
    */
    QByteArray mInputBuff;


private slots:

    /**
    Private slots
    */

    /**
    @brief      Method: Handler of Serial Port errors
    @param      ErrorCodeIn - Serial Port Error code.
    @return     None.
    */
    void handleMainPortErrors(QSerialPort::SerialPortError ErrorCodeIn);

    /**
    @brief      Method: Handler of Serial Port "ReadyRead" signal
    @param      None.
    @return     None.
    */
    void handleMainPortReadyRead();
};

#endif // SERIALPORT_H
