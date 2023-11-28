#include "server.h"


/**
@brief  Static function: Get ID of connected peer.
@param  PeerIn - connected peer (socket).
@return ID as "IP:port" or empty QString
*/
static QString getPeerID(QWebSocket *PeerIn)
{
    return ((PeerIn != nullptr) ? (QStringLiteral("%1:%2").arg(PeerIn->peerAddress().toString(), QString::number(PeerIn->peerPort()))) : QString());
}


/**
@brief  Constructor.
@param  ConfigFileIn - path to config-file;
        LogFileIn - path to log-file or empty QString.
@return None.
*/
Server::Server(QString ConfigFileIn, QString LogFileIn, QObject *parent) : QObject(parent), mWebSocketServer(new QWebSocketServer(QStringLiteral(SERVER_NAME), QWebSocketServer::NonSecureMode, this))
{
    mConfig = new Config(this);
    mConfig->mLogFile = LogFileIn;
    mConfig->mLogUse  = ((!LogFileIn.isEmpty()) ? true : false);
    mConfig->readFile(ConfigFileIn);

    if(mConfig->mCliLifeTime > 0)
    {
        mCliLifeTimer = new QTimer(this);
        connect(mCliLifeTimer, &QTimer::timeout, this, &Server::clientLifeTimeout);
        mCliLifeTimer->start(mConfig->mCliLifeTime*1000);
    }

    if(!mConfig->mID.isEmpty()) mWebSocketServer->setServerName(mConfig->mID);

    if(mWebSocketServer->listen(QHostAddress::Any, mConfig->mPort))
    {
#ifdef LOG
        Log::log(QString("%1: WebSocketServer listening on port %2.").arg(mWebSocketServer->serverName(), QString::number(mWebSocketServer->serverPort())), mConfig->mLogFile, mConfig->mLogUse);
#endif
        connect(mWebSocketServer, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
    }
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Server::~Server()
{
    qDeleteAll(mClients.begin(), mClients.end());
    mWebSocketServer->close();

    if(mCliLifeTimer != nullptr)
    {
        mCliLifeTimer->stop();
        delete mCliLifeTimer;
    }

    if(mConfig != nullptr) delete mConfig;
}


/**
@brief  Private method: Filter by black-list and CliMax.
@param  WebSocketIn - connected client (WebSocket).
@return true - allowed, false - banned.
*/
bool Server::filter(QWebSocket *WebSocketIn)
{
    bool Res = false;

    if(WebSocketIn != nullptr && mConfig->mBlackList.size() < mConfig->mCliMax)
    {
        QString Key = WebSocketIn->peerAddress().toString();
        Res = true;

        if(mConfig->mBlackList.contains(Key))
        {
            quint16 BliPort = mConfig->mBlackList.value(Key, 0);
            quint16 CliPort = WebSocketIn->peerPort();
            if(BliPort == 0 || BliPort == CliPort) Res = false;
        }
    }

    return (Res);
}


/**
@brief  Private method: Disconnect client.
@param  ClientIn - connected client.
@return None.
*/
void Server::disconnectClient(Client *ClientIn)
{
    if(ClientIn != nullptr && !mClients.isEmpty())
    {
#ifdef LOG
        Log::log(QString("%1: %2 disconnected.").arg(mWebSocketServer->serverName(), getPeerID(ClientIn->mWebSocket)), mConfig->mLogFile, mConfig->mLogUse);
#endif
        mClients.removeAll(ClientIn);
        delete ClientIn;
#ifdef LOG
        Log::log(QString("%1: mClients.size() = %2.").arg(mWebSocketServer->serverName(), QString::number(mClients.size())), mConfig->mLogFile, mConfig->mLogUse);
#endif
    }
}


/**
@brief  Private method: Get client by WebSocket pointer.
@param  WebSocketIn - connected client (WebSocket).
@return Pointer to Client or NULL.
*/
Client *Server::getClient(QWebSocket *WebSocketIn)
{
    if(WebSocketIn != nullptr && !mClients.isEmpty())
    {
        Client *pClient = nullptr;

        for(int i=0; i<mClients.size(); i++)
        {
            pClient = mClients.at(i);
            if(pClient != nullptr)
            {
                 if(pClient->mWebSocket == WebSocketIn) return (pClient);
            }
        }
    }

    return (nullptr);
}


/**
@brief  Private slot: New connetion.
@param  None.
@return None.
*/
void Server::onNewConnection()
{
    Client *pClient = new Client();
    pClient->mWebSocket = mWebSocketServer->nextPendingConnection();

    if(this->filter(pClient->mWebSocket))
    {
#ifdef LOG
        Log::log(QString("%1: %2 connected.").arg(mWebSocketServer->serverName(), getPeerID(pClient->mWebSocket)), mConfig->mLogFile, mConfig->mLogUse);
#endif
        pClient->mWebSocket->setParent(this);
        connect(pClient->mWebSocket, &QWebSocket::textMessageReceived, this, &Server::processMessage);
        connect(pClient->mWebSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);
        mClients << pClient;
    }
    else
    {
#ifdef LOG
        Log::log(QString("%1: %2 aborted (black-list).").arg(mWebSocketServer->serverName(), getPeerID(pClient->mWebSocket)), mConfig->mLogFile, mConfig->mLogUse);
#endif
        delete pClient;
    }
#ifdef LOG
        Log::log(QString("%1: mClients.size() = %2.").arg(mWebSocketServer->serverName(), QString::number(mClients.size())), mConfig->mLogFile, mConfig->mLogUse);
#endif
}


/**
@brief  Private slot: Process message.
@param  MsgIn - pointer to received message.
@return None.
*/
void Server::processMessage(const QString &MsgIn)
{
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());

    if(!mClients.isEmpty())
    {
        Client *pClient = nullptr;

        for(int i=0; i<mClients.size(); i++)
        {
            pClient = mClients.at(i);
            if(pClient != nullptr)
            {
                 if(pClient->mWebSocket != pSender)
                 {
                     pClient->mWebSocket->sendTextMessage(MsgIn);
                 }
                 else
                 {
                     pClient->refreshStamp();
                 }
            }
        }
    }
}


/**
@brief  Private slot: Socket disconnect.
@param  None.
@return None.
*/
void Server::socketDisconnected()
{
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    Client *pClient = this->getClient(pSender);
    this->disconnectClient(pClient);
}


/**
@brief  Private slot: Client's life-timer is timeout.
@param  None.
@return None.
*/
void Server::clientLifeTimeout()
{
    if(!mClients.isEmpty())
    {
        QDateTime CurrentStamp = QDateTime::currentDateTime();
        QDateTime ClientStamp;
        Client *pClient = nullptr;

        for(int i=0; i<mClients.size(); i++)
        {
            pClient = mClients.at(i);

            if(pClient != nullptr)
            {
                ClientStamp = pClient->mStamp.addSecs(mConfig->mCliLifeTime);
                if(ClientStamp <= CurrentStamp)
                {
#ifdef LOG
                    Log::log(QString("%1: ClientStamp.(%2) <= CurrentStamp.(%3)").arg(mWebSocketServer->serverName(), ClientStamp.toString(QString("yyyy-MM-dd hh:mm:ss")), CurrentStamp.toString(QString("yyyy-MM-dd hh:mm:ss"))), mConfig->mLogFile, mConfig->mLogUse);
#endif
                    this->disconnectClient(pClient);
                }
            }
        }
    }
}
