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

#include "config.h"


/**
@brief Fields
*/
const QString Config::FIELD__ID                 = "ID";
const QString Config::FIELD__PORT               = "Port";
const QString Config::FIELD__CONN_MAX           = "ConnMax";
const QString Config::FIELD__CONN_PER_CLI       = "ConnPerCli";
const QString Config::FIELD__CONN_LIFE_TIME     = "ConnLifeTime";
const QString Config::FIELD__SURVEY_DELAY       = "SurveyDelay";
const QString Config::FIELD__FIRST_SURVEY_NOW   = "FirstSurveyNow";
const QString Config::FIELD__RANDOM             = "Random";
const QString Config::FIELD__USE_LOG            = "UseLog";
const QString Config::FIELD__USE_WS             = "UseWs";
const QString Config::FIELD__USE_WS_CLI         = "UseWsCli";
const QString Config::FIELD__USE_WS_BLACK       = "UseWsBlack";
const QString Config::FIELD__USE_ARH            = "UseArh";
const QString Config::FIELD__USE_EVENT          = "UseEvent";
const QString Config::FIELD__WS_BLACK           = "WsBlack";
const QString Config::FIELD__WS_CLI             = "WsCli";
const QString Config::FIELD__ROLES              = "Roles";
const QString Config::FIELD__ARH                = "Arh";
const QString Config::FIELD__NETWORKS           = "Networks";
const QString Config::FIELD__LOG                = "Log";

//** roles
const QString Config::FIELD__IP                 = "IP";
const QString Config::FIELD__ROLE               = "Role";

//** TimeStamp of JSON-object that will be send to WS-clients
const QString Config::FIELD__STAMP              = "Stamp";

const QString Config::FIELD__SRV_ID             = "SrvID";
const QString Config::FIELD__NET_ID             = "NetID";
const QString Config::FIELD__DEV_ID             = "DevID";
const QString Config::FIELD__DATA               = "Data";

/**
@brief Client roles
*/
const quint8  Config::ROLE__VIEWER               = 0;
const QString Config::ROLE__VIEWER_STR           = "viewer";
const quint8  Config::ROLE__MANAGER              = 1;
const QString Config::ROLE__MANAGER_STR          = "manager";


/**
@def Values by default.
*/
#define CONFIG_PORT_DEF     8100


/**
@brief  Constructor.
@param  None.
@return None.
*/
Config::Config(QObject *parent) : QObject(parent)
{
    mFileLogArg = QString("");
    this->byDefault();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Config::~Config()
{
    this->byDefault();
}


/**
@brief  Set values by default.
@param  None.
@return None.
*/
void Config::byDefault()
{
    mID             = QString("");
    mPort           = CONFIG_PORT_DEF;
    mConnMax        = 0;
    mConnPerCli     = 0;
    mConnLifeTime   = CONN_LIFE_TIME_OFF;
    mSurveyDelay    = SURVEY_DELAY_MIN;
    mFirstSurveyNow = false;
    mRandom         = false;
    mUseWs          = false;
    mUseWsCli       = false;
    mUseWsBlack     = false;
    mUseArh         = false;
    mUseEvent       = false;
    mFileWsBlack    = QString("");
    mFileWsCli      = QString("");
    mFileRoles      = QString("");
    mFileArh        = QString("");
    mFileNetworks   = QString("");

    if(mFileLogArg.isEmpty())
    {
        mFileLog = QString("");
        mUseLog  = false;
    }

    mListWsBlack.clear();
    mListWsCli.clear();
    mListRoles.clear();

    this->clearListNetworks();
}


/**
@brief  Load values from JsonObject.
@param  DataIn - data.
@return True if value of options are correct, otherwise - False.
*/
bool Config::fromJson(const QJsonObject &DataIn)
{
    this->byDefault();

    if(!DataIn.isEmpty())
    {
        mID           = DataIn.value(FIELD__ID).toString(QString(""));
        mPort         = static_cast<quint16>(DataIn.value(FIELD__PORT).toInt(CONFIG_PORT_DEF));
        mConnMax      = static_cast<quint8>(DataIn.value(FIELD__CONN_MAX).toInt(0));
        mConnPerCli   = static_cast<quint8>(DataIn.value(FIELD__CONN_PER_CLI).toInt(0));
        mConnLifeTime = static_cast<qint32>(DataIn.value(FIELD__CONN_LIFE_TIME).toInt(CONN_LIFE_TIME_OFF));
        mSurveyDelay  = static_cast<quint32>(DataIn.value(FIELD__SURVEY_DELAY).toInt(0));
        mFileWsBlack  = DataIn.value(FIELD__WS_BLACK).toString(QString(""));
        mFileWsCli    = DataIn.value(FIELD__WS_CLI).toString(QString(""));
        mFileRoles    = DataIn.value(FIELD__ROLES).toString(QString(""));
        mFileArh      = DataIn.value(FIELD__ARH).toString(QString(""));
        mFileNetworks = DataIn.value(FIELD__NETWORKS).toString(QString(""));

        int Boo = (DataIn.value(FIELD__FIRST_SURVEY_NOW).toInt(0));
        mFirstSurveyNow = ((Boo) ? true : false);

        Boo = (DataIn.value(FIELD__RANDOM).toInt(0));
        mRandom = ((Boo) ? true : false);

        Boo = (DataIn.value(FIELD__USE_WS).toInt(0));
        mUseWs = ((Boo) ? true : false);

        Boo = (DataIn.value(FIELD__USE_WS_CLI).toInt(0));
        mUseWsCli = ((Boo) ? true : false);

        Boo = (DataIn.value(FIELD__USE_WS_BLACK).toInt(0));
        mUseWsBlack = ((Boo) ? true : false);

        Boo = (DataIn.value(FIELD__USE_ARH).toInt(0));
        mUseArh = ((Boo) ? true : false);

        Boo = (DataIn.value(FIELD__USE_EVENT).toInt(0));
        mUseEvent = ((Boo) ? true : false);

        if(mFileLogArg.isEmpty())
        {
            mFileLog = DataIn.value(FIELD__LOG).toString(QString(""));

            Boo = (DataIn.value(FIELD__USE_LOG).toInt(0));
            mUseLog = ((Boo) ? true : false);
        }

        this->normilize();

        if(mUseWsBlack && !mFileWsBlack.isEmpty()) this->readFileWsBlack(mFileWsBlack);
        if(mUseWsCli && !mFileWsCli.isEmpty()) this->readFileWsCli(mFileWsCli);
        if(!mFileRoles.isEmpty())    this->readFileRoles(mFileRoles);
        if(!mFileNetworks.isEmpty()) this->readFileNetworks(mFileNetworks);

        if(mUseLog)
        {
            QString LogBuff = QString();
            this->toString(LogBuff);
            Log::log(LogBuff, mFileLog, mUseLog);
        }
    }

    return (this->isCorrect());
}


/**
@brief  Public method: Pack values to String buffer.
@param  StringIn - link to string buffer.
@return None.
*/
void Config::toString(QString &StringIn)
{
    QString Correct    = QString(" (is correct)");
    QString NotCorrect = QString(" (is not correct)");

    StringIn+= QString("[ CONFIG ]\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__ID;
    StringIn+= QString(" = ");
    StringIn+= mID;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__PORT;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mPort);
    StringIn+= ((this->isCorrectPort()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__CONN_MAX;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mConnMax);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__CONN_PER_CLI;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mConnPerCli);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__CONN_LIFE_TIME;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mConnLifeTime);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__SURVEY_DELAY;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mSurveyDelay);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__FIRST_SURVEY_NOW;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mFirstSurveyNow) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__RANDOM;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mRandom) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__USE_LOG;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mUseLog) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__USE_WS;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mUseWs) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__USE_WS_CLI;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mUseWsCli) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__USE_WS_BLACK;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mUseWsBlack) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__USE_ARH;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mUseArh) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__USE_EVENT;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mUseEvent) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__WS_BLACK;
    StringIn+= QString(" = ");
    StringIn+= mFileWsBlack;
    StringIn+= QString("\r\n");

    StringIn+= QString(" ListWsBlack.size() = %1").arg(QString::number(mListWsBlack.size()));
    StringIn+= QString("\r\n");

    QString Key;
    QList<QString> Keys;
    int i;

    Keys = mListWsBlack.keys();
    for(i=0; i<Keys.size(); i++)
    {
        Key = Keys.at(i);
        StringIn+= QString(" client[%1] = %2:%3").arg(QString::number(i), Key, QString::number(mListWsBlack.value(Key)));
        StringIn+= QString("\r\n");
    }

    StringIn+= QString(" - ");
    StringIn+= FIELD__WS_CLI;
    StringIn+= QString(" = ");
    StringIn+= mFileWsCli;
    StringIn+= QString("\r\n");

    StringIn+= QString(" ListWsCli.size() = %1").arg(QString::number(mListWsCli.size()));
    StringIn+= QString("\r\n");

    for(i=0; i<mListWsCli.size(); i++)
    {
        StringIn+= QString(" client[%1] = %2").arg(QString::number(i), mListWsCli.at(i));
        StringIn+= QString("\r\n");
    }

    StringIn+= QString(" - ");
    StringIn+= FIELD__ROLES;
    StringIn+= QString(" = ");
    StringIn+= mFileRoles;
    StringIn+= QString("\r\n");

    StringIn+= QString(" ListRoles.size() = %1").arg(QString::number(mListRoles.size()));
    StringIn+= QString("\r\n");

    Keys = mListRoles.keys();
    for(i=0; i<Keys.size(); i++)
    {
        Key = Keys.at(i);
        StringIn+= QString(" client[%1] = %2:").arg(QString::number(i), Key);
        StringIn+= ((mListRoles.value(Key) == ROLE__MANAGER) ? ROLE__MANAGER_STR : ROLE__VIEWER_STR);
        StringIn+= QString("\r\n");
    }

    StringIn+= QString(" - ");
    StringIn+= FIELD__NETWORKS;
    StringIn+= QString(" = ");
    StringIn+= mFileNetworks;
    StringIn+= QString("\r\n");

    StringIn+= QString(" ListNetworks.size() = %1").arg(QString::number(mListNetworks.size()));
    StringIn+= QString("\r\n");

    Network *Net;
    for(i=0; i<mListNetworks.size(); i++)
    {
        Net = mListNetworks.at(i);

        if(Net != nullptr)
        {
            StringIn+= QString(" network[%1]").arg(QString::number(i));
            StringIn+= QString("\r\n");
            Net->toString(StringIn);
        }
    }

    StringIn+= QString(" - ");
    StringIn+= FIELD__LOG;
    StringIn+= QString(" = ");
    StringIn+= mFileLog;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ARH;
    StringIn+= QString(" = ");
    StringIn+= mFileArh;
    StringIn+= QString("\r\n");
}


/**
@brief  Public method: Pack data to JSON buffer.
@param  ObjIn - link to JsonObject.
@return None.
@detailed { "ID":mID, "Stamp":"DateTime of last survey", "Networks":[ {...}, ... ] }
*/
void Config::toJson(QJsonObject &ObjIn)
{
    QJsonArray  ArrNets;
    QJsonObject ObjNet;
    Network    *Net = nullptr;

    for(int i=0; i<mListNetworks.size(); i++)
    {
        Net = mListNetworks.at(i);

        if(Net != nullptr)
        {
             ObjNet = QJsonObject();
             Net->toJson(ObjNet);
             ArrNets.append(ObjNet);
        }
    }

    ObjIn.insert(FIELD__ID, QJsonValue(mID));
    ObjIn.insert(FIELD__STAMP, QJsonValue((QDateTime::currentMSecsSinceEpoch()/1000)));
    ObjIn.insert(FIELD__NETWORKS, QJsonValue(ArrNets));
}


/**
@brief  Public method: Pack values to JSON string.
@param  StringIn - link to string buffer
@return None.
*/
void Config::toJsonString(QString &StringIn)
{
    QJsonDocument Doc;
    QJsonObject Obj;

    this->toJson(Obj);
    Doc.setObject(Obj);

    StringIn+= QString(Doc.toJson(QJsonDocument::Compact));
}


/**
@brief  Public method: Check option "Port".
@param  None.
@return True if value of option "Port" is correct, otherwise - False.
*/
bool Config::isCorrectPort()
{
    return ((mPort > 0) ? true : false);
}


/**
@brief  Public method: Check options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Config::isCorrect()
{
    return ((this->isCorrectPort()) ? true : false);
}


/**
@brief  Public method: Normilize options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Config::normilize()
{
    if(mConnMax > CONN_MAX)    mConnMax    = CONN_MAX;
    if(mConnPerCli > CONN_MAX) mConnPerCli = CONN_MAX;
    if(mConnLifeTime < CONN_LIFE_TIME_OFF) mConnLifeTime = CONN_LIFE_TIME_OFF;
    if(mConnLifeTime > CONN_LIFE_TIME_MAX) mConnLifeTime = CONN_LIFE_TIME_MAX;
    if(mSurveyDelay < SURVEY_DELAY_MIN)    mSurveyDelay  = SURVEY_DELAY_MIN;

    return (this->isCorrect());
}


/**
@brief  Public method: Read config from a file.
@param  FileIn - path to a file.
@return True if value of options are correct, otherwise - False.
*/
bool Config::read(const QString &FileIn)
{
    QString Err;
    QJsonDocument Doc = Json::readFile(FileIn, Err);
    if(!Err.isEmpty()) Log::log(Err, mFileLog, mUseLog);

    QJsonObject Obj;
    if(Doc.isObject()) Obj = Doc.object();

    return (this->fromJson(Obj));
}


/**
@brief  Read list of forbidden WebSocket-clients from a file.
@param  FileIn - path to a file.
@return The number of forbidden clients.
*/
quint16 Config::readFileWsBlack(const QString &FileIn)
{
    Log::log(QString("Config::readFileWsBlack(%1)").arg(FileIn), mFileLog, mUseLog);

    QString Err;
    QJsonDocument Doc = Json::readFile(FileIn, Err);
    if(!Err.isEmpty()) Log::log(Err, mFileLog, mUseLog, false);

    QJsonArray Arr;
    if(Doc.isArray()) Arr = Doc.array();

    return (this->parseDataWsBlack(Arr));
}


/**
@brief  Parse data of forbidden WebSocket-clients.
@param  DataIn - data.
@return The number of forbidden clients.
*/
quint16 Config::parseDataWsBlack(const QJsonArray &DataIn)
{
    if(!DataIn.isEmpty())
    {
        QJsonValue Val;
        QStringList StrParts;
        QString Key;
        bool Ok;

        for(int i=0; i<DataIn.size(); i++)
        {
            Val = DataIn.at(i);

            if(Val.isString())
            {
                StrParts = Val.toString(QString()).split(':');

                if(StrParts.size())
                {
                    Key = StrParts.at(0);
                    mListWsBlack[Key] = ((StrParts.size() > 1) ? static_cast<quint16>(StrParts.at(1).toInt(&Ok)) : static_cast<quint16>(0));
                }
            }
        }
    }

    return (static_cast<quint16>(mListWsBlack.size()));
}


/**
@brief  Read list of predefined WebSocket-clients from a file.
@param  FileIn - path to a file.
@return The number of predefined clients.
*/
quint16 Config::readFileWsCli(const QString &FileIn)
{
    Log::log(QString("Config::readFileWsCli(%1)").arg(FileIn), mFileLog, mUseLog);

    QString Err;
    QJsonDocument Doc = Json::readFile(FileIn, Err);
    if(!Err.isEmpty()) Log::log(Err, mFileLog, mUseLog, false);

    QJsonArray Arr;
    if(Doc.isArray()) Arr = Doc.array();

    return (this->parseDataWsCli(Arr));
}


/**
@brief  Parse data of predefined WebSocket-clients.
@param  DataIn - data.
@return The number of predefined clients.
*/
quint16 Config::parseDataWsCli(const QJsonArray &DataIn)
{
    if(!DataIn.isEmpty())
    {
        QJsonValue Val;
        QString Str;

        for(int i=0; i<DataIn.size(); i++)
        {
            Val = DataIn.at(i);

            if(Val.isString())
            {
                Str = Val.toString(QString());
                if(!Str.isEmpty()) mListWsCli.append(Str);
            }
        }
    }

    return (static_cast<quint16>(mListWsCli.size()));
}


/**
@brief  Read list of roles from file.
@param  FileIn - path to a file.
@return The number of roles.
*/
quint16 Config::readFileRoles(const QString &FileIn)
{
    Log::log(QString("Config::readFileRoles(%1)").arg(FileIn), mFileLog, mUseLog);

    QString Err;
    QJsonDocument Doc = Json::readFile(FileIn, Err);
    if(!Err.isEmpty()) Log::log(Err, mFileLog, mUseLog, false);

    QJsonArray Arr;
    if(Doc.isArray()) Arr = Doc.array();

    return (this->parseDataRoles(Arr));
}


/**
@brief  Parse data of roles.
@param  DataIn - data.
@return The number of roles.
*/
quint16 Config::parseDataRoles(const QJsonArray &DataIn)
{
    if(!DataIn.isEmpty())
    {
        QJsonObject Obj;
        QString IP;
        QString Role;

        for(int i=0; i<DataIn.size(); i++)
        {
            Obj = DataIn.at(i).toObject();

            if(!Obj.isEmpty())
            {
                IP = Obj.value(FIELD__IP).toString(QString());
                if(!IP.isEmpty())
                {
                    Role = Obj.value(FIELD__ROLE).toString(QString());
                    if(Role == ROLE__MANAGER_STR) mListRoles[IP] = ROLE__MANAGER;
                }
            }
        }
    }

    return (static_cast<quint16>(mListRoles.size()));
}


/**
@brief  Read list of networks from file.
@param  FileIn - path to a file.
@return The number of networks.
*/
quint16 Config::readFileNetworks(const QString &FileIn)
{
    Log::log(QString("Config::readFileNetworks(%1)").arg(FileIn), mFileLog, mUseLog);

    QString Err;
    QJsonDocument Doc = Json::readFile(FileIn, Err);
    if(!Err.isEmpty()) Log::log(Err, mFileLog, mUseLog, false);

    return (this->parseDataNetworks(Doc));
}


/**
@brief Parse data of list of networks.
@param  DocIn - link to JsonDocument.
@return The number of networks have parsed.
*/
quint16 Config::parseDataNetworks(const QJsonDocument &DocIn)
{
    if(DocIn.isArray())
    {
        QJsonArray Arr = DocIn.array();
        QJsonObject Obj;
        Network *Net;

        for(int i=0; i<Arr.size(); i++)
        {
            Obj = Arr.at(i).toObject();

            if(!Obj.isEmpty())
            {
                Net = new Network();
                Net->mFileLog = mFileLog;
                Net->mUseLog  = mUseLog;
                Net->fromJson(Obj);

                mListNetworks.append(Net);
            }
        }
    }

    return (this->sizeListNetworks());
}


/**
@brief  Public method: Clear list of networks.
@param  None.
@return None.
*/
void Config::clearListNetworks()
{
    if(!mListNetworks.isEmpty())
    {
        Network *Net;

        while(mListNetworks.size())
        {
            Net = mListNetworks.takeLast();
            if(Net != nullptr) delete Net;
        }
    }
}


/**
@brief  Public method: Get size of list of networks.
@param  None.
@return Size of list of networks.
*/
quint16 Config::sizeListNetworks()
{
    return (static_cast<quint16>(mListNetworks.size()));
}


/**
@brief  Public method: Start randomized surey.
@param  None.
@return True if the survey has completed, otherwise - False.
*/
bool Config::randomize()
{
    return (this->survey(true));
}


/**
@brief  Public method: Start surey.
@param  None.
@return True if the survey has completed, otherwise - False.
*/
bool Config::survey()
{
    return (this->survey(mRandom));
}


/**
@brief  Private method: Start survey.
@param  RandomIn - true if the survey is randomized.
@return True if the survey has completed, otherwise - False.
*/
bool Config::survey(const bool RandomIn)
{
    Log::log(QString("Config::survey(%1)").arg(((RandomIn) ? QString("randomized") : QString("real"))), mFileLog, mUseLog);

    if(this->isCorrect())
    {
        Log::log(QString("ListNetworks.size() = %1\r\n").arg(QString::number(mListNetworks.size())), mFileLog, mUseLog, false);
        Network *Net = nullptr;

        for(int i=0; i<mListNetworks.size(); i++)
        {
            Net = mListNetworks.at(i);

            if(Net)
            {
                if(Net->mAllow)
                {
                    if(!RandomIn)
                    {
                        Log::log(QString("network[%1].survey()").arg(QString::number(i)), mFileLog, mUseLog, false);
                        Net->survey();
                    }
                    else
                    {
                        Log::log(QString("network[%1].randomize()").arg(QString::number(i)), mFileLog, mUseLog, false);
                        Net->randomize();
                    }
                }
            }
        }

        return (true);
    }
    else
    {
        Log::log(QString("The configuration is incorrect!"), mFileLog, mUseLog, false);
    }

    return (false);
}


/**
@brief  Write data.
@param  MsgIn - data message.
@return None.
@return True if the data has written, otherwise - False.
@detailed MsgIn = { SrvID:Config.ID, NetID:Config.Net[n].ID, DevID:Config.Net[n].Dev[d].ID, Data:{ RegVar:RegValue, ... }, Stamp:CurrentTimeStamp }
*/
bool Config::write(const QString &MsgIn)
{
    Log::log(QString("Config::write(%1)").arg(MsgIn), mFileLog, mUseLog);

    if(!MsgIn.isEmpty())
    {
        QJsonDocument Doc = QJsonDocument::fromJson(MsgIn.toUtf8());

        if(Doc.isObject())
        {
            QJsonObject Obj = Doc.object();
            if(!Obj.isEmpty())
            {
                QString SrvID = Obj.value(FIELD__SRV_ID).toString(QString(""));
                if(SrvID == mID)
                {
                    Log::log(QString("SrvID(%1)==Config.mID(%2)").arg(SrvID, mID), mFileLog, mUseLog, false);
                    quint16     NetID = static_cast<quint16>(Obj.value(FIELD__NET_ID).toInt(0));
                    quint16     DevID = static_cast<quint16>(Obj.value(FIELD__DEV_ID).toInt(0));
                    QJsonObject Data  = Obj.value(FIELD__DATA).toObject();
                    Network *Net;

                    for(int i=0; i<mListNetworks.size(); i++)
                    {
                        Net = mListNetworks.at(i);
                        if(Net)
                        {
                            if(Net->mAllow && Net->mID == NetID)
                            {
                                Net->write(DevID, Data);
                                return (true);
                            }
                        }
                    }
                }
            }
        }
    }

    return (false);
}
