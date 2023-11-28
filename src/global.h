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

#ifndef GLOBAL_H
#define GLOBAL_H

//Global Settings


#define ARG_KEY_CONFIG "--config"
#define ARG_KEY_LOG    "--log"
#define ARG_KEY_SNAME  "--sname"
#define ARG_KEY_SDESC  "--sdesc"


//* Service
//** if define: WinService
//** otherwise: Non-Service
#define SERVICE

//* Service Name and Title
#define SERVICE_NAME   "WsLogger"
#define SERVICE_DESC   "WsLogger"


//* Server Name
#define SERVER_NAME    "WsLogger"

//* LibModbus version
//** if define: libmodbus-3.0.x
//** otherwise: libmodbus-3.1.x
#define LIBMODBUS_OLD

//* Structure of DB-table
//** if define: INSERT INTO `{TableName}(`stamp`,`profile`,`channel_id`,`value`,`exception`,`errcode`,`obj_status`,`prog`,`step`) VALUES (...)
//** otherwise: INSERT INTO `{TableName}(`stamp`,`profile`,`device_id`,`register_id`,`value`,`ex`,`err`,`sign`) VALUES (...)
//#define SQL_PROC_TRM

#endif // GLOBAL_H
