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
@brief      Method: log.
@detailed   If LogFile_in is NULL or Empty, then used std::cout.
@param      LogMessage_in - Log-message;
            LogFile_in - path to Log-file;
            LogUse_in - on/off Log.
@return     None.
*/
void Log::log(const QString &LogMessage_in, const QString &LogFile_in = NULL, bool LogUse_in = false)
{
    if(LogMessage_in != NULL && LogUse_in)
    {
        QString Buff;

        Buff.append((QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        Buff.append("\r\n");
        if(!LogMessage_in.isEmpty()) Buff.append(LogMessage_in);
        Buff.append("\r\n\r\n");

        QString LogFile = ((LogFile_in != NULL) ? LogFile_in : QString(""));

        if(!LogFile.isEmpty())
        {
            QFile File(LogFile.toStdString().data());

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


/**
@brief      Method: log.
@detailed   If LogFile_in is NULL or Empty, then used std::cout.
@param      LogMessage_in - Log-message;
            LogFile_in - path to Log-file.
@return     None.
*/
void Log::log(const QString &LogMessage_in, const QString &LogFile_in = NULL)
{
    Log::log(LogMessage_in, LogFile_in, true);
}


/**
@brief      Method: log.
@detailed   Used std::cout.
@param      LogMessage_in - Log-message.
@return     None.
*/
void Log::log(const QString &LogMessage_in)
{
    Log::log(LogMessage_in, NULL, true);
}
