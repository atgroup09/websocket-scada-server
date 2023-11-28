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

#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDateTime>
#include <QtWebSockets>
#include <QUrl>


/**
@brief Client.
*/
class Client : public QObject
{
    Q_OBJECT

public:

    explicit Client(QObject *parent = nullptr);
    ~Client();


    /**
    Public options
    */

    /**
    @brief Sign of Ws-client.
    */
    bool mIsWs;
    QString mWsUri;

    /**
    @brief Pointer to WebSocket.
    */
    QWebSocket *mWebSocket;


    /**
    Public methods
    */

    /**
    @brief  Refresh Stamp of last activity.
    @param  None.
    @return None.
    */
    void refreshStampActivity();

    /**
    @brief  Get value od Stamp of last activity.
    @param  None.
    @return Stamp of last activity.
    */
    QDateTime getStampActivity();

    /**
    @brief  Get value od Stamp of last activity (+ additional seconds).
    @param  SecIn - additional seconds.
    @return Stamp of last activity + SecIn.
    */
    QDateTime getStampActivity(const qint16 SecIn);

    /**
    @brief  Disconnect.
    @param  None.
    @return None.
    */
    void disconnect();

    /**
    @brief  Connect.
    @param  None.
    @return None.
    */
    void connect();

    /**
    @brief  ReConnect.
    @param  None.
    @return None.
    */
    void reconnect();


private:

    /**
    Private options
    */

    /**
    @brief DateTime of last activity.
    */
    QDateTime mStampActivity;
};

#endif // CLIENT_H
