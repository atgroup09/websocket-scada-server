/* Copyright (C) 2019-2023 ATgroup09
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

#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

#include "log.h"
#include "json.h"
#include "network.h"


/**
@brief Config.
*/
class Config : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit Config(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Config();


    /**
    Public constants
    */

    /**
    @brief Fields
    */
    static const QString FIELD__ID;
    static const QString FIELD__PORT;
    static const QString FIELD__CONN_MAX;
    static const QString FIELD__CONN_PER_CLI;
    static const QString FIELD__CONN_LIFE_TIME;
    static const QString FIELD__SURVEY_DELAY;
    static const QString FIELD__FIRST_SURVEY_NOW;
    static const QString FIELD__RANDOM;
    static const QString FIELD__USE_LOG;
    static const QString FIELD__USE_WS;
    static const QString FIELD__USE_WS_CLI;
    static const QString FIELD__USE_WS_BLACK;
    static const QString FIELD__USE_ARH;
    static const QString FIELD__USE_EVENT;
    static const QString FIELD__WS_BLACK;
    static const QString FIELD__WS_CLI;
    static const QString FIELD__ROLES;
    static const QString FIELD__ARH;
    static const QString FIELD__NETWORKS;
    static const QString FIELD__LOG;

    //** roles
    static const QString FIELD__IP;
    static const QString FIELD__ROLE;

    //** TimeStamp of JSON-object that will be send to WS-clients
    static const QString FIELD__STAMP;

    static const QString FIELD__SRV_ID;
    static const QString FIELD__NET_ID;
    static const QString FIELD__DEV_ID;
    static const QString FIELD__DATA;

    /**
    @brief Limites
    */
    static const quint8  CONN_MAX           = 250;
    static const qint32  CONN_LIFE_TIME_OFF = -1;
    static const qint32  CONN_LIFE_TIME_MIN = 0;
    static const qint32  CONN_LIFE_TIME_MAX = 65000;
    static const quint32 SURVEY_DELAY_MIN   = 300;

    /**
    @brief Client roles
    */
    static const quint8  ROLE__VIEWER;
    static const QString ROLE__VIEWER_STR;
    static const quint8  ROLE__MANAGER;
    static const QString ROLE__MANAGER_STR;


    /**
    Public options
    */

    /**
    @brief Server ID.
    */
    QString mID;

    /**
    @brief Port number.
    */
    quint16 mPort;

    /**
    @brief Max number of connections.
    */
    quint8 mConnMax;

    /**
    @brief Max number of connections per one client.
    */
    quint8 mConnPerCli;

    /**
    @brief Life-time of one connection (seconds).
    @detailed -1 for unlimited life-time
    */
    qint32 mConnLifeTime;

    /**
    @brief The Delay between surveys (msec).
    */
    quint32 mSurveyDelay;

    /**
    @brief Start first survey before delay (immediately).
    */
    bool mFirstSurveyNow;

    /**
    @brief Use random data.
    */
    bool mRandom;

    /**
    @brief Use log.
    */
    bool mUseLog;

    /**
    @brief Use WebSocket-server.
    */
    bool mUseWs;

    /**
    @brief Use List of predefined WebSocket-clients.
    */
    bool mUseWsCli;

    /**
    @brief Use List of forbidden WebSocket-clients.
    */
    bool mUseWsBlack;

    /**
    @brief Use periodic archive.
    */
    bool mUseArh;

    /**
    @brief Use event archive.
    */
    bool mUseEvent;

    /**
    @brief Path to a file that contains list of forbidden WebSocket-clients (JSON).
    */
    QString mFileWsBlack;

    /**
    @brief Path to a file that contains list of predefined WebSocket-clients (JSON).
    */
    QString mFileWsCli;

    /**
    @brief Path to a file that contains list of roles of allowed clients (JSON).
    */
    QString mFileRoles;

    /**
    @brief Path to a file that contains settings of a storage (JSON).
    */
    QString mFileArh;

    /**
    @brief Path to a file that contains list of networks (JSON).
    */
    QString mFileNetworks;

    /**
    @brief Path to a logout-file.
    */
    QString mFileLog;

    /**
    @brief Path to a loout-file (server.exe --log ...).
    */
    QString mFileLogArg;

    /**
    @brief List of parsed forbidden WebSocket-clients.
    @detailed "HostOrIP:Port"
              ...
              * Port 0 - all ports
    */
    QMap<QString, quint16> mListWsBlack;

    /**
    @brief List of predefined WebSocket-clients.
    @detailed "HostOrIP:Port"
              ...
    */
    QList<QString> mListWsCli;

    /**
    @brief List of parsed roles of allowed clients.
    @detailed "HostOrIP":"Role"
              ...
    */
    QMap<QString, quint8> mListRoles;

    /**
    @brief List of networks.
    */
    QList<Network *> mListNetworks;


    /**
    Public methods
    */

    /**
    @brief  Set values by default.
    @param  None.
    @return None.
    */
    void byDefault();

    /**
    @brief  Load values from JsonObject.
    @param  DataIn - data.
    @return True if value of options are correct, otherwise - False.
    */
    bool fromJson(const QJsonObject &DataIn);

    /**
    @brief  Pack values to String buffer.
    @param  StringIn - link to string buffer.
    @return None.
    */
    void toString(QString &StringIn);

    /**
    @brief  Pack data to JSON buffer.
    @param  ObjIn - link to JsonObject.
    @return None.
    @detailed { "ID":mID, "Stamp":"DateTime of last survey", "Networks":[ {...}, ... ] }
    */
    void toJson(QJsonObject &ObjIn);

    /**
    @brief  Pack values to JSON string.
    @param  StringIn - link to string buffer
    @return None.
    */
    void toJsonString(QString &StringIn);

    /**
    @brief  Check option "Port".
    @param  None.
    @return True if value of option "Port" is correct, otherwise - False.
    */
    bool isCorrectPort();

    /**
    @brief  Check options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool isCorrect();

    /**
    @brief  Normilize options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool normilize();

    /**
    @brief  Read config from a file.
    @param  FileIn - path to a file.
    @return True if value of options are correct, otherwise - False.
    */
    bool read(const QString &FileIn);

    /**
    @brief  Read list of forbidden WebSocket-clients from a file.
    @param  FileIn - path to a file.
    @return The number of forbidden clients.
    */
    quint16 readFileWsBlack(const QString &FileIn);

    /**
    @brief  Read list of predefined WebSocket-clients from a file.
    @param  FileIn - path to a file.
    @return The number of predefined clients.
    */
    quint16 readFileWsCli(const QString &FileIn);

    /**
    @brief  Read list of roles from file.
    @param  FileIn - path to a file.
    @return The number of roles.
    */
    quint16 readFileRoles(const QString &FileIn);

    /**
    @brief  Read list of networks from file.
    @param  FileIn - path to a file.
    @return The number of networks.
    */
    quint16 readFileNetworks(const QString &FileIn);

    /**
    @brief  Clear list of networks.
    @param  None.
    @return None.
    */
    void clearListNetworks();

    /**
    @brief  Get size of list of networks.
    @param  None.
    @return Size of list of networks.
    */
    quint16 sizeListNetworks();

    /**
    @brief  Start randomized surey.
    @param  None.
    @return True if the survey has completed, otherwise - False.
    */
    bool randomize();

    /**
    @brief  Start surey.
    @param  None.
    @return True if the survey has completed, otherwise - False.
    */
    bool survey();

    /**
    @brief  Write data.
    @param  MsgIn - data message.
    @return None.
    */
    bool write(const QString &MsgIn);


private:

    /**
    Private options
    */


    /**
    Private methods
    */

    /**
    @brief  Parse data of forbidden WebSocket-clients.
    @param  DataIn - data.
    @return The number of forbidden clients.
    */
    quint16 parseDataWsBlack(const QJsonArray &DataIn);

    /**
    @brief  Parse data of predefined WebSocket-clients.
    @param  DataIn - data.
    @return The number of predefined clients.
    */
    quint16 parseDataWsCli(const QJsonArray &DataIn);

    /**
    @brief  Read data of roles.
    @param  DataIn - data.
    @return The number of roles.
    */
    quint16 parseDataRoles(const QJsonArray &DataIn);

    /**
    @brief  Parse data of list of networks.
    @param  DocIn - link to JsonDocument.
    @return The number of networks have parsed.
    */
    quint16 parseDataNetworks(const QJsonDocument &DocIn);

    /**
    @brief  Start survey.
    @param  RandomIn - true if the survey is randomized.
    @return True if the survey has completed, otherwise - False.
    */
    bool survey(const bool RandomIn);
};

#endif // CONFIG_H
