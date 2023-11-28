#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDateTime>
#include <QtWebSockets>


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
    @brief Pointer to WebSocket.
    */
    QWebSocket *mWebSocket;

    /**
    @brief DateTime of last activity.
    */
    QDateTime mStamp;


    /**
    Public methods
    */

    /**
    @brief  Method: Refresh Stamp.
    @param  None.
    @return None.
    */
    void refreshStamp();
};

#endif // CLIENT_H
