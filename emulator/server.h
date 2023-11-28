#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QTimer>
#include <QtWebSockets>

#include "global.h"
#include "log.h"
#include "config.h"
#include "client.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QString)


/**
@brief WebSocket Server
*/
class Server : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  ConfigFileIn - path to config-file;
            LogFileIn - path to log-file or empty QString.
    @return None.
    */
    explicit Server(QString ConfigFileIn, QString LogFileIn, QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Server();


signals:

    /**
    Signals
    */


private:

    /**
    Private options
    */

    /**
    @brief Config
    */
    Config *mConfig;

    /**
    @brief WebSocketServer
    */
    QWebSocketServer *mWebSocketServer;

    /**
    @brief Clients
    */
    QList<Client *> mClients;

    /**
    @brief Client life-timer.
    */
    QTimer *mCliLifeTimer;


    /**
    Private methods
    */

    /**
    @brief  Private method: Filter by black-list and CliMax.
    @param  WebSocketIn - connected client (WebSocket).
    @return true - allowed, false - banned.
    */
    bool filter(QWebSocket *WebSocketIn);

    /**
    @brief  Private method: Disconnect client.
    @param  ClientIn - connected client.
    @return None.
    */
    void disconnectClient(Client *ClientIn);

    /**
    @brief  Private method: Get client by WebSocket pointer.
    @param  WebSocketIn - connected client (WebSocket).
    @return Pointer to Client or NULL.
    */
    Client *getClient(QWebSocket *WebSocketIn);


private slots:

    /**
    Private slots
    */

    /**
    @brief  Private slot: New connetion.
    @param  None.
    @return None.
    */
    void onNewConnection();

    /**
    @brief  Private slot: Process message.
    @param  MsgIn - pointer to received message.
    @return None.
    */
    void processMessage(const QString &MsgIn);

    /**
    @brief  Private slot: Socket disconnect.
    @param  None.
    @return None.
    */
    void socketDisconnected();

    /**
    @brief  Private slot: Client's life-timer is timeout.
    @param  None.
    @return None.
    */
    void clientLifeTimeout();
};

#endif // SERVER_H
