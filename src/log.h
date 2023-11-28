/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  Log functions.
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

#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QFile>


/**
@brief      Log.
@detailed   Send Log-messages to STD or File.
*/
class Log : public QObject
{
    Q_OBJECT

public:

    explicit Log(QObject *parent = nullptr);
    ~Log();

    /**
    Public methods
    */

    /**
    @brief      Log.
    @param      LogMessage_in - Log-message;
                LogFile_in - path to Log-file;
                LogUse_in - on/off Log;
                StampIn - on/off TimeStamp;
                EndLrIn - on/off \r\n in end.
    @return     None.
    @detailed   If LogFile_in is NULL or Empty, then used std::cout.
    */
    static void log(const QString &LogMessage_in, const QString &LogFile_in, bool LogUse_in, bool StampUseIn, bool EndLrIn);

    /**
    @brief      Log.
    @param      LogMessage_in - Log-message;
                LogFile_in - path to Log-file;
                LogUse_in - on/off Log;
                StampIn - on/off TimeStamp.
    @return     None.
    @detailed   If LogFile_in is NULL or Empty, then used std::cout.
    */
    static void log(const QString &LogMessage_in, const QString &LogFile_in, bool LogUse_in, bool StampUseIn);

    /**
    @brief      Log.
    @param      LogMessage_in - Log-message;
                LogFile_in - path to Log-file;
                LogUse_in - on/off Log.
    @return     None.
    @detailed   If LogFile_in is NULL or Empty, then used std::cout.
    */
    static void log(const QString &LogMessage_in, const QString &LogFile_in, bool LogUse_in);

    /**
    @brief      Log.
    @param      LogMessage_in - Log-message;
                LogFile_in - path to Log-file.
    @return     None.
    @detailed   If LogFile_in is NULL or Empty, then used std::cout.
    */
    static void log(const QString &LogMessage_in, const QString &LogFile_in);

    /**
    @brief      Log.
    @param      LogMessage_in - Log-message.
    @return     None.
    @detailed   Used std::cout.
    */
    static void log(const QString &LogMessage_in);

};

#endif // LOG_H
