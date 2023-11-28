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

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QtWebSockets>
#include <QDebug>

#include "global.h"
#include "log.h"
#include "config.h"
#include "arh.h"
#include "client.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QString)


/**
@brief Logger Server
*/
class Server : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  ConfigFileIn - path to config-file.
    @param  LogOutFileIn - path to logout-file.
    @return None.
    */
    explicit Server(QString ConfigFileIn, QString LogOutFileIn, QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Server();


public slots:

    /**
    Public slots
    */

    /**
    @brief  Start the server.
    @param  None.
    @return true if the server started, otherwise - false.
    */
    bool start();

    /**
    @brief  Stop the server.
    @param  None.
    @return true if the server stopped, otherwise - false.
    */
    bool stop();


private:

    /**
    Private options
    */

    /**
    @brief Path to Config file
    */
    QString mConfigFile;

    /**
    @brief Config
    */
    Config mConfig;

    /**
    @brief Clients
    */
    QList<Client *> mClients;

    /**
    @brief Clients marked to disconnect
    */
    QList<Client *> mClientsMarked;

    /**
    @brief Ws-Clients
    */
    QList<Client *> mWsClients;

    /**
    @brief Data Buffer to send over WebSocket.
    */
    QString mDataToSend;

    /**
    @brief WebSocketServer
    */
    QWebSocketServer *mWebSocketServer;

    /**
    @brief Survey timer.
    */
    QTimer *mSurveyTimer;

    /**
    @brief Arhive.
    */
    Archive *mArh;
    QThread *mArhThread;

    /**
    @brief Messages from clients
    */
    QList<QString> mCliMsg;


    /**
    Private methods
    */

    /**
    @brief  Read main config.
    @param  None.
    @return True if OK, otherwise - False.
    */
    bool readConfig();

    /**
    @brief  Init. thread of Archive (periodic).
    @param  None.
    @return True if OK, otherwise - False.
    */
    bool initArhThread();

    /**
    @brief  Init. thread of WebSocket-server.
    @param  None.
    @return True if OK, otherwise - False.
    */
    bool initWsThread();

    /**
    @brief  Stop thread of WebSocket-server.
    @param  None.
    @return True if OK, otherwise - False.
    */
    bool stopWsThread();

    /**
    @brief  Apply filtering by black-list and ConnMax.
    @param  WebSocketIn - connected client (WebSocket).
    @return true - blocked, false - not blocked.
    */
    bool hasCliBlocked(QWebSocket *WebSocketIn);

    /**
    @brief  Check life-time.
    @param  ClientIn - connected client.
    @return true - expired, false - not expired.
    */
    bool hasCliTimeExpired(Client *ClientIn);

    /**
    @brief  Disconnect a client.
    @param  ClientIn - client to disconnect.
    @return None.
    */
    void disconnectCli(Client *ClientIn);

    /**
    @brief  Disconnect all client.
    @param  None.
    @return None.
    */
    void disconnectAllCli();

    /**
    @brief  Disconnect marked clients.
    @param  None.
    @return None.
    */
    void disconnectMarkedCli();

    /**
    @brief  Get client by WebSocket pointer.
    @param  WebSocketIn - connected client (WebSocket).
    @return Pointer to Client or nullptr.
    */
    Client *getCli(QWebSocket *WebSocketIn);

    /**
    @brief  Get the number of clients by IP.
    @param  HostIn - host address of a client.
    @return The number of connected clients.
    */
    quint32 size(QHostAddress HostIn);

    /**
    @brief  Init. list of Ws-clients.
    @param  None.
    @return None.
    */
    void initWsCli();

    /**
    @brief  Write data.
    @param  None.
    @return None.
    */
    void write();


private slots:

    /**
    Private slots
    */

    /**
    @brief  Client connected.
    @param  None.
    @return None.
    */
    void cliConnected();

    /**
    @brief  Client disconnected.
    @param  None.
    @return None.
    */
    void cliDisconnected();

    /**
    @brief  Process the client message.
    @param  MessageIn - the client message.
    @return None.
    */
    void cliProcessMessage(const QString &MessageIn);

    /**
    @brief  Start survey shot.
    @param  None.
    @return None.
    @detailed emit signal surveyCompleted
    */
    void startSurvey();

    /**
    @brief  Send survey data to connected clients.
    @param  None.
    @return None.
    @detailed emit signal surveyDataToCliSent
    */
    void sendSurveyDataToCli();

    /**
    @brief  Start survey delay.
    @param  None.
    @return None.
    @detailed emit signal surveyDelayCompleted
    */
    void startSurveyDelay();


signals:

    /**
    Signals
    */

    /**
    @brief  The server have started.
    @param  None.
    @return None.
    */
    void started();

    /**
    @brief  The server have stopped.
    @param  None.
    @return None.
    */
    void stopped();

    /**
    @brief  The Survey data have completed.
    @param  None.
    @return None.
    */
    void surveyCompleted();

    /**
    @brief  The Survey data have sent to clients.
    @param  None.
    @return None.
    */
    void surveyDataToCliSent();
};

#endif // SERVER_H
