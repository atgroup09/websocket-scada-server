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

#include "service.h"


/**
@brief Input arguments.
*/
const QString Service::ARG_KEY__CONFIG   = QString("--config");
const QString Service::ARG_KEY__SNAME    = QString("--sname");
const QString Service::ARG_KEY__SDESC    = QString("--sdesc");


/**
@brief  Constructor.
@param  argc  - size of argv;
        argv  - list of input arguments;
        sname - service name.
@return None.
*/
Service::Service(int argc, char *argv[], const QString sname) : QtService<QCoreApplication>(argc, argv, sname)
{
    QHash<QString, QString> ParsedArgs;
    Args::parse(argc, argv, ParsedArgs);

    mServer = new Server(ParsedArgs.value(ARG_KEY__CONFIG, QString("")), QString(""));

    setServiceDescription(sname);
    setServiceFlags(QtServiceBase::CanBeSuspended);
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Service::~Service()
{
    delete mServer;
}


/**
@brief  Start the service.
@detailed this method is called by click on button Start in Service Manager.
@param  None.
@return None.
*/
void Service::start()
{
    QString SName = serviceName();
    QtServiceController controller(SName);
    controller.start();

    if(controller.isRunning())
    {
        mServer->start();
    }
}


/**
@brief  Stop the service.
@detailed this method is called by click on button Stop in Service Manager.
@param  None.
@return None.
*/
void Service::stop()
{
    QString SName = serviceName();
    QtServiceController controller(SName);
    controller.stop();

    mServer->stop();
}


/**
@brief  Pause the service.
@detailed this method is called by click on button Pause in Service Manager.
@param  None.
@return None.
*/
void Service::pause()
{
    QString SName = serviceName();
    QtServiceController controller(SName);
    controller.pause();

    mServer->stop();
}


/**
@brief  Resume the service.
@detailed this method is called by click on button Resume in Service Manager.
@param  None.
@return None.
*/
void Service::resume()
{
    QString SName = serviceName();
    QtServiceController controller(SName);
    controller.resume();

    mServer->start();
}
