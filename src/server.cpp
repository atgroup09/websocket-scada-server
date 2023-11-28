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

#include "server.h"


/**
@brief  Get ID of connected peer.
@param  PeerIn - connected peer (socket).
@return ID as "IP:port" or empty QString
*/
static QString getPeerID(QWebSocket *PeerIn)
{
    return ((PeerIn) ? (QStringLiteral("%1:%2").arg(PeerIn->peerAddress().toString(), QString::number(PeerIn->peerPort()))) : QString());
}


/**
@brief  Constructor.
@param  ConfigFileIn - path to config-file.
@param  LogOutFileIn - path to logout-file.
@return None.
*/
Server::Server(QString ConfigFileIn, QString LogOutFileIn, QObject *parent) : QObject(parent)
{
    mConfigFile      = ConfigFileIn;
    mSurveyTimer     = new QTimer(this);
    mWebSocketServer = nullptr;

    if(!LogOutFileIn.isEmpty())
    {
        mConfig.mFileLog = mConfig.mFileLogArg = LogOutFileIn;
        mConfig.mUseLog  = true;
    }

    connect(mSurveyTimer, &QTimer::timeout, this, &Server::startSurvey);
    connect(this, &Server::surveyCompleted, this, &Server::sendSurveyDataToCli);
    connect(this, &Server::surveyDataToCliSent, this, &Server::startSurveyDelay);
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Server::~Server()
{
    this->stop();
    delete mSurveyTimer;
}


/**
@brief  Read main config.
@param  None.
@return True if OK, otherwise - False.
*/
bool Server::readConfig()
{
    return (mConfig.read(mConfigFile));
}


/**
@brief  Init. thread of Archive (periodic).
@param  None.
@return True if OK, otherwise - False.
*/
bool Server::initArhThread()
{
    if(mConfig.mUseArh && !mConfig.mFileArh.isEmpty())
    {
        Log::log(QString("Server::initArhThread()"), mConfig.mFileLog, mConfig.mUseLog);

        mArh = new Archive(&mConfig.mListNetworks, Archive::MODE__PERIODIC);
        mArh->readFileConfig(mConfig.mFileArh);

        mArhThread = new QThread();
        mArh->moveToThread(mArhThread);

        connect(mArhThread, &QThread::started, mArh, &Archive::start);
        connect(this, &Server::stopped, mArh, &Archive::stop);
        connect(mArh, &Archive::sigStopped, mArhThread, &QThread::quit);
        connect(mArhThread, &QThread::finished, mArh, &Archive::deleteLater);
        connect(mArhThread, &QThread::finished, mArhThread, &QThread::deleteLater);

        mArhThread->start();

        return (true);
    }

    return (false);
}


/**
@brief  Init. thread of WebSocket-server.
@param  None.
@return True if OK, otherwise - False.
*/
bool Server::initWsThread()
{
    if(mConfig.mUseWs && mConfig.isCorrectPort())
    {
        Log::log(QString("Server::initWsThread()"), mConfig.mFileLog, mConfig.mUseLog);

        mWebSocketServer = new QWebSocketServer(SERVER_NAME, QWebSocketServer::NonSecureMode, this);
        if(!mConfig.mID.isEmpty()) mWebSocketServer->setServerName(mConfig.mID);

        if(mWebSocketServer->listen(QHostAddress::Any, mConfig.mPort))
        {
            Log::log(QString("WebSocketServer '%1' is listening on port %2").arg(mWebSocketServer->serverName(), QString::number(mWebSocketServer->serverPort())), mConfig.mFileLog, mConfig.mUseLog);
            connect(mWebSocketServer, &QWebSocketServer::newConnection, this, &Server::cliConnected);

            return (true);
        }
        else
        {
            Log::log(QString("WebSocketServer has terminated with error! %1").arg(mWebSocketServer->errorString()), mConfig.mFileLog, mConfig.mUseLog);
        }
    }

    return (false);
}


/**
@brief  Stop thread of WebSocket-server.
@param  None.
@return True if OK, otherwise - False.
*/
bool Server::stopWsThread()
{
    Log::log(QString("Server::stopWsThread()"), mConfig.mFileLog, mConfig.mUseLog);

    if(mWebSocketServer) mWebSocketServer->pauseAccepting();

    this->disconnectMarkedCli();
    this->disconnectAllCli();

    if(mWebSocketServer)
    {
        delete mWebSocketServer;
        mWebSocketServer = nullptr;
    }

    return (true);
}


/**
@brief  Start the server.
@param  None.
@return true if the server started, otherwise - false.
*/
bool Server::start()
{
    if(this->readConfig())
    {
        Log::log(QString("Server::start()"), mConfig.mFileLog, mConfig.mUseLog);

        this->initWsCli();
        this->initWsThread();
        this->initArhThread();

        if(mConfig.mFirstSurveyNow)
        {
            this->startSurvey();
        }
        else
        {
            this->startSurveyDelay();
        }

        emit started();

        return (true);
    }

    return (false);
}


/**
@brief  Stop the server.
@param  None.
@return true if the server stopped, otherwise - false.
*/
bool Server::stop()
{
    Log::log(QString("Server::stop()"), mConfig.mFileLog, mConfig.mUseLog);

    if(mSurveyTimer->isActive()) mSurveyTimer->stop();

    this->stopWsThread();
    mWsClients.clear();

    emit stopped();

    return (true);
}


/**
@brief  Apply filtering by black-list and ConnMax.
@param  WebSocketIn - connected client (WebSocket).
@return true - blocked, false - not blocked.
*/
bool Server::hasCliBlocked(QWebSocket *WebSocketIn)
{
    if(mConfig.mUseWsBlack)
    {
        if(WebSocketIn && mClients.size() < mConfig.mConnMax)
        {
            if(this->size(WebSocketIn->peerAddress()) < mConfig.mConnPerCli)
            {
                QString Key = WebSocketIn->peerAddress().toString();

                if(mConfig.mListWsBlack.contains(Key))
                {
                    quint16 BloPort = mConfig.mListWsBlack.value(Key, static_cast<quint16>(0));
                    quint16 CliPort = WebSocketIn->peerPort();
                    if(BloPort == 0 || BloPort == CliPort) return (true);
                }
                return (false);
            }
        }
        return (true);
    }

    return (false);
}


/**
@brief  Check life-time.
@param  ClientIn - connected client.
@return true - expired, false - not expired.
*/
bool Server::hasCliTimeExpired(Client *ClientIn)
{
    if(ClientIn && mConfig.mConnLifeTime > Config::CONN_LIFE_TIME_MIN)
    {
        QDateTime CurrentStamp = QDateTime::currentDateTime();
        QDateTime ClientStamp  = ClientIn->getStampActivity(static_cast<qint16>(mConfig.mConnLifeTime));
        if(ClientStamp <= CurrentStamp) return (true);
    }

    return (false);
}


/**
@brief  Disconnect client.
@param  ClientIn - client.
@return None.
@detailed The client will be disconnect and remove from the List of clients.
          The memory, allocated to object of the client will be free.
*/
void Server::disconnectCli(Client *ClientIn)
{
    if(ClientIn && !mClients.isEmpty())
    {
        if(!ClientIn->mIsWs)
        {
            mClients.removeAll(ClientIn);
            delete ClientIn;
        }
        else
        {
            ClientIn->disconnect();
            ClientIn->refreshStampActivity();
        }
    }
}


/**
@brief  Disconnect all client.
@param  None.
@return None.
*/
void Server::disconnectAllCli()
{
    if(!mClients.isEmpty())
    {
        Client *pClient = nullptr;

        while(!mClients.isEmpty())
        {
            pClient = mClients.takeLast();
            if(pClient)
            {
                delete pClient;
            }
        }
    }
}


/**
@brief  Disconnect marked clients.
@param  None.
@return None.
*/
void Server::disconnectMarkedCli()
{
    if(!mClientsMarked.isEmpty() && !mClients.isEmpty())
    {
        Client *pClient = nullptr;

        while(!mClientsMarked.isEmpty())
        {
            pClient = mClientsMarked.takeLast();
            this->disconnectCli(pClient);
        }
    }
}


/**
@brief  Get client by WebSocket pointer.
@param  WebSocketIn - connected client (WebSocket).
@return Pointer to Client or nullptr.
*/
Client *Server::getCli(QWebSocket *WebSocketIn)
{
    if(WebSocketIn && !mClients.isEmpty())
    {
        Client *pClient = nullptr;
        int Size = mClients.size();

        for(int i=0; i<Size; i++)
        {
            pClient = mClients.at(i);
            if(pClient)
            {
                 if(pClient->mWebSocket == WebSocketIn) return (pClient);
            }
        }
    }

    return (nullptr);
}


/**
@brief  Get the number of clients by IP.
@param  HostIn - host address of a client.
@return The number of connected clients.
*/
quint32 Server::size(QHostAddress HostIn)
{
    quint32 Num = 0;

    if(!mClients.isEmpty())
    {
        QHostAddress Host;
        Client *pClient = nullptr;
        int Size = mClients.size();

        for(int i=0; i<Size; i++)
        {
            pClient = mClients.at(i);
            if(pClient)
            {
                Host = pClient->mWebSocket->peerAddress();
                if(Host.toString() == HostIn.toString()) Num++;
            }
        }
    }

    return (Num);
}


/**
@brief  Client connected.
@param  None.
@return None.
*/
void Server::cliConnected()
{
    Log::log(QString("Server::cliConnected()"), mConfig.mFileLog, mConfig.mUseLog);

    Client *pClient = new Client();
    pClient->mWebSocket = mWebSocketServer->nextPendingConnection();

    if(!this->hasCliBlocked(pClient->mWebSocket))
    {
        pClient->mWebSocket->setParent(this);
        connect(pClient->mWebSocket, &QWebSocket::disconnected, this, &Server::cliDisconnected);
        connect(pClient->mWebSocket, &QWebSocket::textMessageReceived, this, &Server::cliProcessMessage);
        mClients << pClient;
        Log::log(QString("%1 has connected").arg(getPeerID(pClient->mWebSocket)), mConfig.mFileLog, mConfig.mUseLog, false);
        Log::log(QString("clients = %1").arg(QString::number(mClients.size())), mConfig.mFileLog, mConfig.mUseLog, false);
    }
    else
    {
        Log::log(QString("%1 has blocked by black-list").arg(getPeerID(pClient->mWebSocket)), mConfig.mFileLog, mConfig.mUseLog, false);
        delete pClient;
    }
}


/**
@brief  Client disconnected.
@param  None.
@return None.
*/
void Server::cliDisconnected()
{
    Log::log(QString("Server::cliDisconnected()"), mConfig.mFileLog, mConfig.mUseLog);

    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    Client *pClient = this->getCli(pSender);

    if(pClient)
    {
        Log::log(QString("%1 has disconnected").arg(getPeerID(pClient->mWebSocket)), mConfig.mFileLog, mConfig.mUseLog, false);
        this->disconnectCli(pClient);
        Log::log(QString("clients = %1").arg(QString::number(mClients.size())), mConfig.mFileLog, mConfig.mUseLog, false);
    }
}


/**
@brief  Process the client message.
@param  MessageIn - the client message.
@return None.
*/
void Server::cliProcessMessage(const QString &MessageIn)
{
    Log::log(QString("Server::cliProcessMessage()"), mConfig.mFileLog, mConfig.mUseLog);

    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    Client *pClient     = this->getCli(pSender);

    if(pClient)
    {
        mCliMsg.append(MessageIn);
        Log::log(QString("%1 has send message").arg(getPeerID(pClient->mWebSocket)), mConfig.mFileLog, mConfig.mUseLog, false);
        Log::log(MessageIn, mConfig.mFileLog, mConfig.mUseLog, false);
    }
}


/**
@brief  Init. list of Ws-clients.
@param  None.
@return None.
*/
void Server::initWsCli()
{
    int nClients = mConfig.mListWsCli.size();

    if(nClients > 0)
    {
        Log::log(QString("Server::initWsCli()"), mConfig.mFileLog, mConfig.mUseLog);

        QString Uri;

        for(int i=0; i<nClients; i++)
        {
            Uri = mConfig.mListWsCli.at(i);
            Log::log(QString("+ %1").arg(Uri), mConfig.mFileLog, mConfig.mUseLog);

            if(!Uri.isEmpty())
            {
                Client *pClient = new Client();
                pClient->mIsWs  = true;
                pClient->mWsUri = Uri;
                pClient->mWebSocket = new QWebSocket(Uri);
                pClient->mWebSocket->setParent(this);
                connect(pClient->mWebSocket, &QWebSocket::disconnected, this, &Server::cliDisconnected);
                mClients << pClient;
            }
        }
    }
}


/**
@brief  Write data.
@param  None.
@return None.
*/
void Server::write()
{
    Log::log(QString("Server::write(start)"), mConfig.mFileLog, mConfig.mUseLog);

    if(!mCliMsg.isEmpty())
    {
        bool Res;
        int  Len = mCliMsg.size();
        QString CliMsg;

        for(int i=0; i<Len; i++)
        {
            CliMsg = mCliMsg.takeFirst();
            Res = mConfig.write(CliMsg);
            if(Res)
            {
                Log::log(QString("Server::write(done)"), mConfig.mFileLog, mConfig.mUseLog);
            }
        }
    }
}


/**
@brief  Start survey shot.
@param  None.
@return None.
@detailed emit signal surveyCompleted()
*/
void Server::startSurvey()
{
    Log::log(QString("Server::startSurvey()"), mConfig.mFileLog, mConfig.mUseLog);

    mDataToSend.clear();

    if(mConfig.isCorrect())
    {
        //write client data
        this->write();

        //survey
        bool Res = mConfig.survey();
        if(Res)
        {
           mConfig.toJsonString(mDataToSend);
           Log::log(mDataToSend, mConfig.mFileLog, mConfig.mUseLog, false);
        }
    }
    else
    {
        Log::log(QString("The configuration is incorrect!"), mConfig.mFileLog, mConfig.mUseLog, false);
    }

    emit surveyCompleted();
}


/**
@brief  Send survey data to connected clients.
@param  None.
@return None.
@detailed emit signal surveyDataToCliSent()
*/
void Server::sendSurveyDataToCli()
{
    Log::log(QString("Server::sendSurveyDataToCli()"), mConfig.mFileLog, mConfig.mUseLog);

    if(!mClients.isEmpty())
    {
        if(!mDataToSend.isEmpty())
        {
            Client *pClient = nullptr;
            int Size = mClients.size();

            for(int i=0; i<Size; i++)
            {
                pClient = mClients.at(i);

                if(pClient)
                {
                    if(!this->hasCliTimeExpired(pClient))
                    {
                        if(pClient->mWebSocket->state() == QAbstractSocket::ConnectedState)
                        {
                            pClient->mWebSocket->sendTextMessage(mDataToSend);
                        }
                        else
                        {
                            if(pClient->mIsWs && !pClient->mWsUri.isEmpty())
                            {
                                //reconnect to Ws-cli
                                Log::log(QString("try reconnect %1").arg(pClient->mWsUri), mConfig.mFileLog, mConfig.mUseLog);
                                pClient->reconnect();
                            }
                        }
                    }
                    else
                    {
                        //mark all expired cli
                        mClientsMarked.append(pClient);
                    }
                }
            }

            this->disconnectMarkedCli();
        }
        else
        {
            Log::log(QString("The data to send is empty!"), mConfig.mFileLog, mConfig.mUseLog, false);
        }
    }
    else
    {
        Log::log(QString("The list of clients is empty!"), mConfig.mFileLog, mConfig.mUseLog, false);
    }

    emit surveyDataToCliSent();
}


/**
@brief  Start survey delay.
@param  None.
@return None.
*/
void Server::startSurveyDelay()
{
    Log::log(QString("Server::startSurveyDelay(%1 msec)").arg(QString::number(mConfig.mSurveyDelay)), mConfig.mFileLog, mConfig.mUseLog);

    if(mSurveyTimer->isActive()) mSurveyTimer->stop();

    if(mConfig.mSurveyDelay > 0)
    {
       mSurveyTimer->setInterval(static_cast<int>(mConfig.mSurveyDelay));
       mSurveyTimer->setSingleShot(true);
       mSurveyTimer->start();
    }
    else
    {
        this->startSurvey();
    }
}
