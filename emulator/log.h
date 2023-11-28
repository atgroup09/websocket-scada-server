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
    @brief      Method: log.
    @detailed   If LogFile_in is NULL or Empty, then used std::cout.
    @param      LogMessage_in - Log-message;
                LogFile_in - path to Log-file;
                LogUse_in - on/off Log.
    @return     None.
    */
    static void log(const QString &LogMessage_in, const QString &LogFile_in, bool LogUse_in);

    /**
    @brief      Method: log.
    @detailed   If LogFile_in is NULL or Empty, then used std::cout.
    @param      LogMessage_in - Log-message;
                LogFile_in - path to Log-file.
    @return     None.
    */
    static void log(const QString &LogMessage_in, const QString &LogFile_in);

    /**
    @brief      Method: log.
    @detailed   Used std::cout.
    @param      LogMessage_in - Log-message.
    @return     None.
    */
    static void log(const QString &LogMessage_in);

};

#endif // LOG_H
