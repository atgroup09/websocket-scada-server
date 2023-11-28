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

#ifndef ARH_H
#define ARH_H

#include <iostream>
#include <QObject>
#include <QList>
#include <QTimer>

#include "log.h"
#include "json.h"
#include "mysql-cli.h"
#include "network.h"


/**
@brief Archive.
*/
class Archive : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit Archive(QList<Network *> *ListNetworksIn, quint8 ModeIn, QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Archive();


    /**
    Public constants
    */

    /**
    @brief Fields
    */
    static const QString FIELD__HOST;
    static const QString FIELD__PORT;
    static const QString FIELD__USER;
    static const QString FIELD__PASSWD;
    static const QString FIELD__DB;
    static const QString FIELD__PROFILE;
    static const QString FIELD__USE_LOG;
    static const QString FIELD__USE_LOG_EVENT;
    static const QString FIELD__FILE_LOG;
    static const QString FIELD__FILE_LOG_EVENT;
    static const QString FIELD__MODE;

    /**
    @brief Named profiles
    */
    static const QString PROFILE__1MIN;
    static const QString PROFILE__5MIN;
    static const QString PROFILE__15MIN;
    static const QString PROFILE__30MIN;
    static const QString PROFILE__HOUR;
    static const QString PROFILE__EVENT;

    /**
    @brief Timed profiles (msec)
    */
    static const int PROFILE__1MIN_MSEC  = 60000;
    static const int PROFILE__5MIN_MSEC  = 300000;
    static const int PROFILE__15MIN_MSEC = 900000;
    static const int PROFILE__30MIN_MSEC = 1800000;
    static const int PROFILE__HOUR_MSEC  = 3600000;
    static const int PROFILE__EVENT_MSEC = 300000;

    /**
    @brief Values by default
    */
    static const QString DEFAUL__HOST;
    static const quint32 DEFAUL__PORT = 3306;

    /**
    @brief Modes
    */
    static const quint8 MODE__PERIODIC = 0;
    static const quint8 MODE__EVENT    = 1;


    /**
    Public options
    */

    /**
    @brief Host name.
    @detailed "localhost" by default.
    */
    QString mHost;

    /**
    @brief Port number.
    @detailed 3306 by default.
    */
    quint32 mPort;

    /**
    @brief User name.
    @detailed "" by default.
    */
    QString mUser;

    /**
    @brief User password.
    @detailed "" by default.
    */
    QString mPasswd;

    /**
    @brief Name of selected database.
    */
    QString mDb;

    /**
    @brief Name of profile.
    */
    QString mProfile;

    /**
    @brief Path to log-file.
    */
    QString mFileLog;

    /**
    @brief Allow to write of log.
    */
    bool mUseLog;


    /**
    Public methods
    */

    /**
    @brief  Pack values to String buffer.
    @param  StringIn - link to string buffer.
    @return None.
    */
    void toString(QString &StringIn);

    /**
    @brief  Read configuration from a file.
    @param  FileIn - path to a file.
    @return true if success, otherwise - false.
    */
    bool readFileConfig(const QString &FileIn);

    /**
    @brief  Check option "Profile".
    @param  ProfileIn - profile.
    @return True if value of the option is correct, otherwise - False.
    */
    static bool isCorrectProfile(const QString &ProfileIn);

    /**
    @brief  Check option "Profile".
    @param  None.
    @return True if value of the option is correct, otherwise - False.
    */
    bool isCorrectProfile();

    /**
    @brief  Check options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool isCorrect();

    /**
    @brief  Get timed profile.
    @param  ProfileIn - named profile.
    @return msec or 0.
    */
    int getTimedProfile(const QString &ProfileIn);


public slots:

    /**
    @brief  Start.
    @param  None.
    @return None.
    @details Start timer and start signal sigStarted or sigError.
    */
    void start();

    /**
    @brief  Stop.
    @param  None.
    @return None.
    @details Stop timer and send signal sigStopped.
    */
    void stop();

    /**
    @brief  Save data into a storage.
    @param  None.
    @return None.
    @details Save data into a storage and send signal sigCompleted.
    */
    void save();


private:

    /**
    Private options
    */

    /**
    @brief Mode
    @details MODE__PERIODIC by default
    */
    quint8 mMode;

    /**
    @brief Timer.
    */
    QTimer *mTimer;

    /**
    @brief Link to list of networks.
    */
    QList<Network *> *mListNetworks;


    /**
    Private methods
    */

    /**
    @brief  Parse configuration.
    @param  DocIn - link to JsonDocument.
    @return true if success, otherwise - false.
    */
    bool parseDataConfig(const QJsonDocument &DocIn);

    /**
    @brief  Pack device data into SQL.
    @param  NetIn - pointer to Network;
    @param  DevIdxIn - index of device in the list of devices (0...NetIn->sizeListDevices()-1);
    @param  StringIn - link to string buffer
    @return true if OK, otherwise - false.
    */
    bool toSql(Network *NetIn, const int DevIdxIn, QString &StringIn);

    /**
    @brief  Save data into a File.
    @param  FileIn - path to file;
    @param  DataIn - data.
    @return true if OK, otherwise - false.
    */
    bool saveToFile(const QString &FileIn, const QString &DataIn);

    /**
    @brief  Save data into a DB.
    @param  ListDataIn - list of data (queries).
    @return true if OK, otherwise - false.
    */
    bool saveToDb(const QList<QString> &ListDataIn);

    /**
    @brief  Stop timer.
    @param  none.
    @return none.
    */
    void stopTimer();


signals:

    /**
    @brief  Started.
    @param  None.
    @return None.
    */
    void sigStarted();

    /**
    @brief  Stopped.
    @param  None.
    @return None.
    */
    void sigStopped();

    /**
    @brief  Error.
    @param  None.
    @return None.
    */
    void sigError();

    /**
    @brief  The archiving has completed successfully.
    @param  None.
    @return None.
    */
    void sigCompleted();
};

#endif // ARH_H
