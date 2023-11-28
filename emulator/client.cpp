#include "client.h"


/**
@brief  Constructor.
@param  None.
@return None.
*/
Client::Client(QObject *parent) : QObject(parent)
{
    mWebSocket = nullptr;
    this->refreshStamp();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Client::~Client()
{
    if(mWebSocket != nullptr)
    {
        mWebSocket->close(QWebSocketProtocol::CloseCodeNormal);
        mWebSocket->deleteLater();
    }
}


/**
@brief  Method: Refresh Stamp.
@param  None.
@return None.
*/
void Client::refreshStamp()
{
    mStamp = QDateTime::currentDateTime();
}
