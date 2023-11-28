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

#include "log.h"


/**
@brief      Constructor.
@param      None.
@return     None.
*/
Log::Log(QObject *parent) : QObject(parent)
{

}


/**
@brief      Destructor.
@param      None.
@return     None.
*/
Log::~Log()
{

}


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
void Log::log(const QString &LogMessage_in, const QString &LogFile_in, bool LogUse_in = false, bool StampUseIn = true, bool EndLrIn = true)
{
    if(LogUse_in)
    {
        QString Buff;

        if(StampUseIn)
        {
            Buff.append((QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            Buff.append("\r\n");
        }

        if(!LogMessage_in.isEmpty())
        {
            Buff.append(LogMessage_in);
            Buff.append("\r\n");
        }

        if(EndLrIn) Buff.append("\r\n");

        if(!Buff.isEmpty())
        {
            if(!LogFile_in.isEmpty())
            {
                QFile File(LogFile_in.toStdString().data());

                if(File.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
                {
                    File.atEnd();
                    File.write(Buff.toUtf8());
                    File.close();
                }
            }
            else
            {
                std::cout << Buff.toStdString();
            }
        }
    }
}


/**
@brief      Log.
@param      LogMessage_in - Log-message;
            LogFile_in - path to Log-file;
            LogUse_in - on/off Log;
            StampIn - on/off TimeStamp.
@return     None.
@detailed   If LogFile_in is NULL or Empty, then used std::cout.
*/
void Log::log(const QString &LogMessage_in, const QString &LogFile_in, bool LogUse_in = false, bool StampUseIn = true)
{
    Log::log(LogMessage_in, LogFile_in, LogUse_in, StampUseIn, true);
}


/**
@brief      Log.
@param      LogMessage_in - Log-message;
            LogFile_in - path to Log-file;
            LogUse_in - on/off Log.
@return     None.
@detailed   If LogFile_in is NULL or Empty, then used std::cout.
*/
void Log::log(const QString &LogMessage_in, const QString &LogFile_in, bool LogUse_in)
{
    Log::log(LogMessage_in, LogFile_in, LogUse_in, true, true);
}


/**
@brief      Method: log.
@detailed   If LogFile_in is NULL or Empty, then used std::cout.
@param      LogMessage_in - Log-message;
            LogFile_in - path to Log-file.
@return     None.
*/
void Log::log(const QString &LogMessage_in, const QString &LogFile_in)
{
    Log::log(LogMessage_in, LogFile_in, true, true, true);
}


/**
@brief      Method: log.
@detailed   Used std::cout.
@param      LogMessage_in - Log-message.
@return     None.
*/
void Log::log(const QString &LogMessage_in)
{
    Log::log(LogMessage_in, QString(""), true, true, true);
}
