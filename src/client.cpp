/* Copyright (C) 2019-2020 ATgroup09
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

#include "client.h"


/**
@brief  Constructor.
@param  None.
@return None.
*/
Client::Client(QObject *parent) : QObject(parent)
{
    mIsWs      = false;
    mWsUri     = QString("");
    mWebSocket = nullptr;
    this->refreshStampActivity();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Client::~Client()
{
    this->disconnect();
    if(mWebSocket != nullptr) mWebSocket->deleteLater();
}


/**
@brief  Disconnect.
@param  None.
@return None.
*/
void Client::disconnect()
{
    if(mWebSocket != nullptr)
    {
        if(mWebSocket->state() != QAbstractSocket::UnconnectedState)
        {
            mWebSocket->close(QWebSocketProtocol::CloseCodeNormal);
        }
    }
}


/**
@brief  Connect.
@param  None.
@return None.
*/
void Client::connect()
{
    if(mWebSocket != nullptr && !mWsUri.isEmpty())
    {
        if(mWebSocket->state() == QAbstractSocket::UnconnectedState)
        {
            mWebSocket->open(QUrl(mWsUri));
        }
    }
}


/**
@brief  ReConnect.
@param  None.
@return None.
*/
void Client::reconnect()
{
    this->disconnect();
    this->connect();
}


/**
@brief  Refresh Stamp of Activity.
@param  None.
@return None.
*/
void Client::refreshStampActivity()
{
    mStampActivity = QDateTime::currentDateTime();
}


/**
@brief  Get value od Stamp of last activity.
@param  None.
@return QDateTime.
*/
QDateTime Client::getStampActivity()
{
    return (mStampActivity);
}


/**
@brief  Get value od Stamp of last activity (+ additional seconds).
@param  SecIn - additional seconds.
@return Stamp of last activity + SecIn.
*/
QDateTime Client::getStampActivity(const qint16 SecIn)
{
    return (mStampActivity.addSecs(SecIn));
}
