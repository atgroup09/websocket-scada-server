/* Copyright (C) 2019 ATgroup09
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

#include "arh.h"


/**
@brief Fields
*/
const QString Archive::FIELD__HOST           = "Host";
const QString Archive::FIELD__PORT           = "Port";
const QString Archive::FIELD__USER           = "User";
const QString Archive::FIELD__PASSWD         = "Passwd";
const QString Archive::FIELD__DB             = "Db";
const QString Archive::FIELD__PROFILE        = "Profile";
const QString Archive::FIELD__USE_LOG        = "UseLog";
const QString Archive::FIELD__USE_LOG_EVENT  = "UseLogEvent";
const QString Archive::FIELD__FILE_LOG       = "Log";
const QString Archive::FIELD__FILE_LOG_EVENT = "LogEvent";
const QString Archive::FIELD__MODE           = "Mode";

/**
@brief Named profiles
*/
const QString Archive::PROFILE__1MIN         = "1min";
const QString Archive::PROFILE__5MIN         = "5min";
const QString Archive::PROFILE__15MIN        = "15min";
const QString Archive::PROFILE__30MIN        = "30min";
const QString Archive::PROFILE__HOUR         = "hour";
const QString Archive::PROFILE__EVENT        = "event";

/**
@brief Values by default
*/
const QString Archive::DEFAUL__HOST          = "localhost";


/**
@brief  Constructor.
@param  None.
@return None.
*/
Archive::Archive(QList<Network *> *ListNetworksIn, quint8 ModeIn, QObject *parent) : QObject(parent)
{
    mHost         = DEFAUL__HOST;
    mPort         = DEFAUL__PORT;
    mUser         = QString("");
    mPasswd       = QString("");
    mDb           = QString("");
    mProfile      = QString("");
    mUseLog       = false;
    mFileLog      = QString("");
    mMode         = ((ModeIn == MODE__EVENT) ? MODE__EVENT : MODE__PERIODIC);
    mListNetworks = ListNetworksIn;

    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &Archive::save);
    connect(this, &Archive::sigCompleted, this, &Archive::start);
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Archive::~Archive()
{
    this->stopTimer();
    delete mTimer;
}


/**
@brief  Pack values to String buffer.
@param  StringIn - link to string buffer.
@return None.
*/
void Archive::toString(QString &StringIn)
{
    QString Correct    = QString(" (is correct)");
    QString NotCorrect = QString(" (is not correct)");

    StringIn+= QString("[ ARCHIVE ]\r\n");

    StringIn+= QString(" - ");
    StringIn+= Archive::FIELD__MODE;
    StringIn+= QString(" = ");
    StringIn+= ((mMode == MODE__EVENT) ? QString("event") : QString("periodic"));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__HOST;
    StringIn+= QString(" = ");
    StringIn+= mHost;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PORT;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mPort);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__USER;
    StringIn+= QString(" = ");
    StringIn+= mUser;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PASSWD;
    StringIn+= QString(" = ");
    StringIn+= mPasswd;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__DB;
    StringIn+= QString(" = ");
    StringIn+= mDb;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__PROFILE;
    StringIn+= QString(" = ");
    StringIn+= mProfile;
    StringIn+= ((this->isCorrectProfile()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__USE_LOG;
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mUseLog) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__FILE_LOG;
    StringIn+= QString(" = ");
    StringIn+= mFileLog;
    StringIn+= QString("\r\n");
}


/**
@brief  Read configuration from a file.
@param  FileIn - path to a file.
@return true if success, otherwise - false.
*/
bool Archive::readFileConfig(const QString &FileIn)
{
    QString Err;
    QJsonDocument Doc = Json::readFile(FileIn, Err);
    if(!Err.isEmpty()) Log::log(Err, mFileLog, mUseLog);

    return (this->parseDataConfig(Doc));
}


/**
@brief  Parse configuration.
@param  DocIn - link to JsonDocument.
@return true if success, otherwise - false.
*/
bool Archive::parseDataConfig(const QJsonDocument &DocIn)
{
    if(DocIn.isObject())
    {
        QJsonObject Obj = DocIn.object();

        if(!Obj.isEmpty())
        {
            mHost      = Obj.value(FIELD__HOST).toString("");
            mPort      = static_cast<quint32>(Obj.value(FIELD__PORT).toInt(0));
            mUser      = Obj.value(FIELD__USER).toString(QString(""));
            mPasswd    = Obj.value(FIELD__PASSWD).toString(QString(""));
            mDb        = Obj.value(FIELD__DB).toString(QString(""));
            mProfile   = Obj.value(FIELD__PROFILE).toString(QString(""));
            mFileLog   = ((mMode == MODE__EVENT) ? Obj.value(FIELD__FILE_LOG_EVENT).toString(QString("")) : Obj.value(FIELD__FILE_LOG).toString(QString("")));

            int Boo = ((mMode == MODE__EVENT) ? Obj.value(FIELD__USE_LOG_EVENT).toInt(0) : Obj.value(FIELD__USE_LOG).toInt(0));
            mUseLog = ((Boo) ? true : false);

            if(mUseLog)
            {
                QString LogBuff = QString();
                this->toString(LogBuff);
                Log::log(LogBuff, mFileLog, mUseLog);
            }

            return (true);
        }
    }

    return (false);
}


/**
@brief  Check option "Profile".
@param  ProfileIn - profile.
@return True if value of the option is correct, otherwise - False.
*/
bool Archive::isCorrectProfile(const QString &ProfileIn)
{
    return ((ProfileIn == PROFILE__1MIN || ProfileIn == PROFILE__5MIN || ProfileIn == PROFILE__15MIN || ProfileIn == PROFILE__30MIN || ProfileIn == PROFILE__HOUR) ? true : false);
}


/**
@brief  Check option "Profile".
@param  None.
@return True if value of the option is correct, otherwise - False.
*/
bool Archive::isCorrectProfile()
{
    return (isCorrectProfile(mProfile));
}


/**
@brief  Check options by type.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Archive::isCorrect()
{
    return (this->isCorrectProfile());
}


/**
@brief  Get timed profile.
@param  ProfileIn - named profile.
@return msec or 0.
*/
int Archive::getTimedProfile(const QString &ProfileIn)
{
    if(ProfileIn == PROFILE__1MIN)
    {
        return (PROFILE__1MIN_MSEC);
    }
    else if(ProfileIn == PROFILE__5MIN)
    {
        return (PROFILE__5MIN_MSEC);
    }
    else if(ProfileIn == PROFILE__15MIN)
    {
        return (PROFILE__15MIN_MSEC);
    }
    else if(ProfileIn == PROFILE__30MIN)
    {
        return (PROFILE__30MIN_MSEC);
    }
    else if(ProfileIn == PROFILE__HOUR)
    {
        return (PROFILE__HOUR_MSEC);
    }
    else if(ProfileIn == PROFILE__EVENT)
    {
        return (PROFILE__EVENT_MSEC);
    }

    return (static_cast<int>(0));
}


/**
@brief  Start.
@param  None.
@return None.
@details Start timer and start signal sigStarted.
*/
void Archive::start()
{
    this->stopTimer();

    int Msec = this->getTimedProfile(mProfile);

    if(Msec)
    {
        Log::log(QString("Archive::start(%1 msec)").arg(QString::number(Msec)), mFileLog, mUseLog);

        mTimer->setInterval(Msec);
        mTimer->setSingleShot(true);
        mTimer->start();

        emit sigStarted();
    }
    else
    {
        emit sigError();
    }
}


/**
@brief  Stop.
@param  None.
@return None.
@details Stop timer and send signal sigStopped.
*/
void Archive::stop()
{
    Log::log(QString("Archive::stop()"), mFileLog, mUseLog);

    this->stopTimer();
    emit sigStopped();
}


/**
@brief  Stop timer.
@param  none.
@return none.
*/
void Archive::stopTimer()
{
    if(mTimer->isActive()) mTimer->stop();
}


/**
@brief  Pack device data into SQL.
@param  NetIn - pointer to Network;
@param  DevIdxIn - index of device in the list of devices (0...NetIn->sizeListDevices()-1);
@param  StringIn - link to string buffer
@return true if OK, otherwise - false.
*/
bool Archive::toSql(Network *NetIn, const int DevIdxIn, QString &StringIn)
{
    bool Res = false;

    if(this->isCorrectProfile() && NetIn != nullptr)
    {
        //* list of strings of one query
        QList<QString> ListStrings;
        bool EventMode = ((mMode == MODE__EVENT) ? true : false);
        NetIn->getDeviceSql(mProfile, EventMode, DevIdxIn, ListStrings);

        if(!ListStrings.isEmpty())
        {
            //merge strings to one SQL and append to input string buffer
            Device::mergeSql(ListStrings, StringIn);
            Res = true;
        }
    }

    return (Res);
}


/**
@brief  Save data into a storage.
@param  None.
@return None.
@details Save data into a storage and send signal sigCompleted.
*/
void Archive::save()
{
    if(mListNetworks != nullptr)
    {
        Log::log(QString("Archive::save()"), mFileLog, mUseLog);

        if(!mListNetworks->isEmpty())
        {
            Network *Net = nullptr;
            QString StoreFile(""), StoreTable(""), Query("");
            QList<QString> ListDbData;
            int nNets = mListNetworks->size();
            int nDevs = 0;
            int i, j;

            for(i=0; i<nNets; i++)
            {
                Net = mListNetworks->at(i);
                if(Net)
                {
                    nDevs = Net->sizeListDevices();

                    for(j=0; j<nDevs; j++)
                    {
                        StoreFile  = Net->getDeviceArhFile(j);
                        StoreTable = Net->getDeviceArhTable(j);

                        if(!StoreFile.isEmpty() || !StoreTable.isEmpty())
                        {
                            Query = QString("");
                            this->toSql(Net, j, Query);

                            Log::log(Query, mFileLog, mUseLog, false, false);
                            Log::log(QString("\r\n"), mFileLog, mUseLog, false, false);

                            if(!StoreFile.isEmpty())
                            {
                                this->saveToFile(StoreFile, Query);
                            }
                            else
                            {
                                ListDbData.append(Query);
                            }
                        }
                    }
                }
            }

            if(!ListDbData.isEmpty()) this->saveToDb(ListDbData);
        }
        else
        {
            Log::log(QString("ListNetworks is empty!"), mFileLog, mUseLog, false);
        }
    }

    emit sigCompleted();
}


/**
@brief  Save data into a File.
@param  FileIn - path to file;
@param  DataIn - data.
@return true if OK, otherwise - false.
*/
bool Archive::saveToFile(const QString &FileIn, const QString &DataIn)
{
    bool Res = false;

    if(!FileIn.isEmpty() && !DataIn.isEmpty())
    {
        Log::log(QString("Archive::saveToFile(%1)").arg(FileIn), mFileLog, mUseLog);
        Log::log(DataIn, FileIn, true, false, false);
        Res = true;
    }

    return (Res);
}


/**
@brief  Save data into a DB.
@param  ListDataIn - list of data (queries).
@return true if OK, otherwise - false.
*/
bool Archive::saveToDb(const QList<QString> &ListDataIn)
{
    bool Res = false;

    if(!mHost.isEmpty() && mPort > 0 && !mUser.isEmpty() && !mDb.isEmpty())
    {
        Log::log(QString("Archive::saveToDb()"), mFileLog, mUseLog);

        if(!ListDataIn.isEmpty())
        {
            HelperMySQL *MySqlCli = new HelperMySQL();

            MySqlCli->mHost   = mHost;
            MySqlCli->mPort   = mPort;
            MySqlCli->mUser   = mUser;
            MySqlCli->mPasswd = mPasswd;
            MySqlCli->mDB     = mDb;

            if(MySqlCli->connect())
            {
                Log::log(QString("Connection with MySQL DB is established: %1@%2:%3/%4").arg(MySqlCli->mUser, MySqlCli->mHost, QString::number(MySqlCli->mPort), MySqlCli->mDB), mFileLog, mUseLog);

                for(int i=0; i<ListDataIn.size(); i++)
                {
                    Res = MySqlCli->sendQuery(ListDataIn.at(i));

                    if(Res)
                    {
                        Log::log(QString("The query sent successfully!"), mFileLog, mUseLog);
                    }
                    else
                    {
                        Log::log(QString("Error send query (%1)! %2)").arg(QString::number(MySqlCli->getErrorNo()), MySqlCli->getError()), mFileLog, mUseLog);
                    }
                }

                MySqlCli->disconnect();
                Log::log(QString("Connection with MySQL DB is closed."), mFileLog, mUseLog);
            }
            else
            {
                Log::log(QString("Error connecting to DB (%1)! %2)").arg(QString::number(MySqlCli->getErrorNo()), MySqlCli->getError()), mFileLog, mUseLog);
            }

            delete MySqlCli;
        }
    }

    return (Res);
}
