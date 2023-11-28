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

#include "modbus-tcp-cli.h"


/**
@brief      Constant: the number of ModBus-server port.
*/
const int HelperModBusTCPClient::PORT__502 = 502;


/**
@brief      Constructor.
@param      None.
@return     None.
*/
HelperModBusTCPClient::HelperModBusTCPClient(QObject *parent) : HelperModBusClient(parent)
{
    mIP   = QString("");
    mPort = PORT__502;
}


/**
@brief      Destructor.
@param      None.
@return     None.
*/
HelperModBusTCPClient::~HelperModBusTCPClient()
{

}


/**
@brief      Public slot: Init.
@param      None.
@return     True if initialization is performed, otherwise - false.
*/
bool HelperModBusTCPClient::init()
{
    this->disconnect();

    mCtx    = modbus_new_tcp(mIP.toUtf8().data(), mPort);
    mInited = ((mCtx != nullptr) ? true : false);

    if(mInited)
    {
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
