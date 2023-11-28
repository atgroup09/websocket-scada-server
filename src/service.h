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

#ifndef SERVICE_H
#define SERVICE_H

#include <iostream>
#include <QCoreApplication>
#include <QString>
#include <qtservice.h>

#include "global.h"
#include "args.h"
#include "server.h"


/**
    Installation on Windows:

        sc create "<ServiceName>" binpath= "<Path to Exe-file with input arguments>" DisplayName= "<Service Display Name>" start= <startType: auto|demand|delayed-auto|disabled>

    Service arguments:

        --config Path-to-Config-file [ --sname ServiceName --sdesc ServiceDescription ]

        [  ] - optional arguments

        if argument --sname is not contains in the list of arguments, then used service name by default (SERVICE_NAME).
        if argument --sdesc is not contains in the list of arguments, then used service name by default (SERVICE_DESC).
*/


/**
@brief Service class.
*/
class Service : public QtService<QCoreApplication>
{

public:

    /**
    @brief  Constructor.
    @param  argc  - size of argv;
            argv  - list of input arguments;
            sname - service name.
    @return None.
    */
    Service(int argc, char *argv[], const QString sname);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    ~Service();


    /**
    Public options
    */

    /**
    @brief Input arguments.
    */
    static const QString ARG_KEY__CONFIG;
    static const QString ARG_KEY__SNAME;
    static const QString ARG_KEY__SDESC;


    /**
    Public methods
    */

    /**
    @brief  Start the service.
    @detailed this method is called by click on button Start in Service Manager.
    @param  None.
    @return None.
    */
    void start();

    /**
    @brief  Stop the service.
    @detailed this method is called by click on button Stop in Service Manager.
    @param  None.
    @return None.
    */
    void stop();

    /**
    @brief  Pause the service.
    @detailed this method is called by click on button Pause in Service Manager.
    @param  None.
    @return None.
    */
    void pause();

    /**
    @brief  Resume the service.
    @detailed this method is called by click on button Resume in Service Manager.
    @param  None.
    @return None.
    */
    void resume();


private:

    /**
    Private options
    */

    /**
    @brief Server.
    */
    Server *mServer;
};

#endif // SERVICE_H
