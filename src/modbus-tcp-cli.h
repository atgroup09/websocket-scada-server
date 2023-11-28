/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  ModBus TCP data protocol helper.
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

#ifndef HELPERMODBUSTCPCLIENT_H
#define HELPERMODBUSTCPCLIENT_H

#include "modbus-cli.h"


/**
@brief      ModBus TCP
@detailed   Used limmodbus.dll.
*/
class HelperModBusTCPClient : public HelperModBusClient
{
    Q_OBJECT

public:

    /**
    @brief      Constructor.
    @param      None.
    @return     None.
    */
    explicit HelperModBusTCPClient(QObject *parent = nullptr);

    /**
    @brief      Destructor.
    @param      None.
    @return     None.
    */
    virtual ~HelperModBusTCPClient();


    /**
    Public options
    */

    /**
    @brief      Constant: the number of ModBus-server port.
    */
    static const int PORT__502;

    /**
    @brief      Option: IP-address of target device.
    */
    QString mIP;

    /**
    @brief      Option: the number of network port.
    @detailed   PORT__502 by default.
    */
    int mPort;


    /**
    Public methods
    */

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

#endif // HELPERMODBUSTCPCLIENT_H
