/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  MySQL data transfer protocol helper.
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

#include "mysql-cli.h"


/**
@brief      Constant: Class error codes
*/
const quint32 HelperMySQL::ERROR__INIT            = 1;


/**
@brief      Constant: Host names
*/
const QString HelperMySQL::HOST__LOCALHOST        = QString("localhost");


/**
@brief      Constant: Charsets
*/
const QString HelperMySQL::CHARSET__UTF8          = QString("utf8");


/**
@brief      Constructor.
@param      None.
@return     None.
*/
HelperMySQL::HelperMySQL(QObject *parent) : QObject(parent)
{
    mHost            = HelperMySQL::HOST__LOCALHOST;
    mUser            = QString("");
    mPasswd          = QString("");
    mDB              = QString("");
    mPort            = 3306;
    mCharacterSet    = HelperMySQL::CHARSET__UTF8;
    mInited          = false;
    mConnected       = false;
}


/**
@brief      Destructor.
@param      None.
@return     None.
*/
HelperMySQL::~HelperMySQL()
{
    this->disconnect();
}


/**
@brief      Method: Check init. status.
@param      None.
@return     True if initialization is performed (ERROR__INIT), otherwise - false.
*/
bool HelperMySQL::isInited()
{
    return (mInited);
}


/**
@brief      Method: Check connection.
@param      None.
@return     True if connection was established, otherwise - false.
*/
bool HelperMySQL::isConnected()
{
    return (mConnected);
}


/**
@brief      Method: Get Error code.
@detailed   if no errors, then returns 0
@param      None.
@return     Error code.
*/
quint32 HelperMySQL::getErrorNo()
{
    return ((this->isInited()) ? mysql_errno(mMySQL) : HelperMySQL::ERROR__INIT);
}


/**
@brief      Method: Get Error message.
@detailed   if no errors, then returns empty string
@param      None.
@return     Error code.
*/
QString HelperMySQL::getError()
{
    return ((this->isInited()) ? QString(mysql_error(mMySQL)) : QString("Error init. MySQL plugin!"));
}


/**
@brief      Method: Get Resultset.
@detailed   Resultset is NULL:
                - for INSERT, UPDATE, DELETE, DROP queries (it is normally);
                - for SELECT, SHOW, DESCRIBE, EXPLAIN queries if Error.
            Use mysql_free_result() for free memory!
@param      None.
@return     Pointer to Stored resultset or NULL.s
*/
MYSQL_RES *HelperMySQL::getResultset()
{
    return ((this->isConnected()) ? mysql_store_result(mMySQL) : nullptr);
}


/**
@brief      Public slot: Init.
@param      None.
@return     True if initialization is performed, otherwise - false.
*/
bool HelperMySQL::init()
{
    this->disconnect();

    mMySQL  = mysql_init(nullptr);
    mInited = ((mMySQL != nullptr) ? true : false);

    if(mInited)
    {
        emit sigInited();
    }
    else
    {
        quint32 ErrNo  = this->getErrorNo();
        QString ErrStr = this->getError();
        emit sigError(ErrNo, ErrStr);
    }

    return (this->isInited());
}


/**
@brief      Public slot: Connect to server.
@param      None.
@return     True if connection was established successfully, otherwise - false.
*/
bool HelperMySQL::connect()
{
    if(this->init())
    {
        MYSQL *pMySQL = ((mDB.isEmpty()) ? mysql_real_connect(mMySQL, mHost.toUtf8().data(), mUser.toUtf8().data(), mPasswd.toUtf8().data(), nullptr, 0, nullptr, 0) : mysql_real_connect(mMySQL, mHost.toUtf8().data(), mUser.toUtf8().data(), mPasswd.toUtf8().data(), mDB.toUtf8().data(), 0, nullptr, 0));
        mConnected    = ((pMySQL != nullptr) ? true : false);
    }

    if(mConnected)
    {
        emit sigConnected();
    }
    else
    {
        quint32 ErrNo  = this->getErrorNo();
        QString ErrStr = this->getError();
        emit sigError(ErrNo, ErrStr);
    }

    return (this->isConnected());
}


/**
@brief      Public slot: Close connection with server.
@param      None.
@return     None.
*/
void HelperMySQL::disconnect()
{
    if(this->isInited())
    {
        if(mConnected) mysql_close(mMySQL);
        mMySQL = nullptr;
    }

    mInited    = false;
    mConnected = false;

    emit sigDisconnected();
}


/**
@brief      Public slot: Test the connection.
@param      None.
@return     True if connection is established, otherwise - false.
*/
bool HelperMySQL::ping()
{
    qint32 Res = ((this->isInited()) ? mysql_ping(mMySQL) : -1);

    if(Res == 0)
    {
        emit sigPong();
    }
    else
    {
        quint32 ErrNo  = this->getErrorNo();
        QString ErrStr = this->getError();
        emit sigError(ErrNo, ErrStr);
    }

    return ((Res == 0) ? true : false);
}


/**
@brief      Public slot: Select the database.
@param      DBNameIn - the name of database to select.
@return     True if database was selected successfully, otherwise - false.
*/
bool HelperMySQL::selectDB(const QString &DBNameIn)
{
    qint32 Res = -1;

    if(this->isConnected())
    {
        if(!DBNameIn.isEmpty())
        {
            Res = mysql_select_db(mMySQL, DBNameIn.toUtf8().data());
        }
        else if(!mDB.isEmpty())
        {
            Res = mysql_select_db(mMySQL, mDB.toUtf8().data());
        }
    }

    if(Res == 0)
    {
        emit sigDBselected();
    }
    else
    {
        quint32 ErrNo  = this->getErrorNo();
        QString ErrStr = this->getError();
        emit sigError(ErrNo, ErrStr);
    }

    return ((Res == 0) ? true : false);
}


/**
@brief      Method: Select the database.
@param      None.
@return     True if database was selected successfully, otherwise - false.
*/
bool HelperMySQL::selectDB()
{
    return (this->selectDB(QString("")));
}


/**
@brief      Public slot: Set option.
@param      OptionNameIn  - the option name;
            OptionValueIn - the option value.
@return     True if the option was set successfully, otherwise - false.
*/
bool HelperMySQL::setOption(mysql_option OptionNameIn, const QString &OptionValueIn)
{
    qint32 Res = -1;

    if(!OptionValueIn.isEmpty())
    {
        if(this->init())
        {
            Res = mysql_options(mMySQL, OptionNameIn, OptionValueIn.toUtf8().data());
        }
    }

    if(Res == 0)
    {
        emit sigOptionSet();
    }
    else
    {
        quint32 ErrNo  = this->getErrorNo();
        QString ErrStr = this->getError();
        emit sigError(ErrNo, ErrStr);
    }

    return ((Res == 0) ? true : false);
}


/**
@brief      Public slot: Send Query.
@param      QueryIn - SQL-query.
@return     True if query was sent successfully, otherwise - false.
*/
bool HelperMySQL::sendQuery(const QString &QueryIn)
{
    qint32 Res = -1;

    if(this->isConnected() && !QueryIn.isEmpty())
    {
        Res = mysql_real_query(mMySQL, QueryIn.toUtf8().data(), static_cast<unsigned long>(QueryIn.size()));
    }

    if(Res == 0)
    {
        emit sigQuerySent();
    }
    else
    {
        quint32 ErrNo  = this->getErrorNo();
        QString ErrStr = this->getError();
        emit sigError(ErrNo, ErrStr);
    }

    return ((Res == 0) ? true : false);
}
