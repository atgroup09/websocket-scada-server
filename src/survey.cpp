#include "survey.h"


/**
@brief  Constructor.
@param  None.
@return None.
*/
Survey::Survey(QObject *parent) : QObject(parent)
{
    mConfig      = new Config(this);
    mConfigDevs  = new ConfigDevices(this);
    mSurveyTimer = new QTimer(this);
    mLogFile     = QString("");
    mLogUse      = false;

    connect(mSurveyTimer, SIGNAL(timeout()), this, SLOT(shot()));
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Survey::~Survey()
{
    mSurveyTimer->stop();

    delete mSurveyTimer;
    delete mConfigDevs;
    delete mConfig;
}


/**
@brief  Read config from file (JSON).
@param  FileIn - path to Config-file.
@return true if Config was read, otherwise - false.
*/
bool Survey::readConfigFromFile(const QString &FileIn)
{
    mConfig->mLogFile = mLogFile;
    mConfig->mLogUse  = mLogUse;

    return (mConfig->readFromFile(FileIn));
}


/**
@brief  Read config of devices from file (JSON).
@param  FileIn - path to DevMap-file.
@return true if Map-list was read, otherwise - false.
*/
bool Survey::readConfigDevFromFile(const QString &FileIn)
{
    mConfigDevs->mLogFile = mLogFile;
    mConfigDevs->mLogUse  = mLogUse;

    return (mConfigDevs->readFromFile(FileIn));
}


/**
@brief  Read config of devices from file (JSON).
@param  None.
@return true if Map-list was read, otherwise - false.
*/
bool Survey::readConfigDevFromFile()
{
    mConfigDevs->mLogFile = mLogFile;
    mConfigDevs->mLogUse  = mLogUse;

    return (this->readConfigDevFromFile(mConfig->mDevMapFile));
}


/**
@brief  Read data of devices.
@param  None.
@return The number of reading devices.
*/
int Survey::readData()
{
    int Res = 0;

    if(!mConfigDevs->isEmpty())
    {
        Device *Dev;
        int ResDev = 0, j = 0;

        for(int i=0; i<mConfigDevs->size(); i++)
        {
            if((Dev = mConfigDevs->getDevice(i)) != NULL)
            {
                Dev->mLogUse = mLogUse;

                for(j=0; j<mConfig->mRepeat+1; j++)
                {
                    ResDev = this->readDevData(Dev);
#ifdef LOG
                    Log::log(QString("repeat %1 from %2").arg(QString::number(j), QString::number(mConfig->mRepeat+1)), mLogFile, mLogUse);
#endif
                    if(ResDev > 0)
                    {
                        Res++;
                        break;
                    }
                }
            }
        }
    }

    return (Res);
}


/**
@brief    Read data of Device.
@detailed supported classes of device: TRG
@param    ModBusCliIn - link to configured helper of ModBus Client (connection must be established);
          DevIn - link to Dev.
@return   The number of read registers.
*/
int Survey::readDevData(Device *DevIn)
{
    int Res = 0;

    if(DevIn != NULL)
    {
        if(DevIn->isAllow())
        {
#ifdef LOG
            QString LogBuff = QString("");
            DevIn->toString(LogBuff);
            Log::log(LogBuff, mLogFile, mLogUse);
#endif
            if((DevIn->mProtoComm == Device::PROTO_COMM__SERIAL || DevIn->mProtoComm == Device::PROTO_COMM__RS232 || DevIn->mProtoComm == Device::PROTO_COMM__RS485) && DevIn->mProtoData == Device::PROTO_DATA__MODBUS_RTU)
            {
                HelperModBusRTUClient *ModBusCli = new HelperModBusRTUClient();
                ModBusCli->mSerialPortNum      = HelperModBusRTUClient::convSerialNum((int)mConfig->mSerialPortNum);
                ModBusCli->mSerialPortSpd      = (int)mConfig->mSerialPortSpd;
                ModBusCli->mSerialPortPrty     = HelperModBusRTUClient::convSerialPrty(mConfig->mSerialPortPrty);
                ModBusCli->mSerialPortDataBits = (int)mConfig->mSerialPortDatabits;
                ModBusCli->mSerialPortStopBits = (int)mConfig->mSerialPortStopbits;
                ModBusCli->mSerialPortMode     = (int)mConfig->mSerialPortMode;
                ModBusCli->mDevBaseAddr        = DevIn->mBaseAddr;

                if(ModBusCli->connect())
                {
                    Res = DevIn->readRegsModBusRTU(ModBusCli);
#ifdef LOG
                    if(Res != HelperModBusRTUClient::ERROR_RES)
                    {
                        LogBuff.clear();
#ifdef DEBUG
                        DevIn->toStringChannels(LogBuff);
                        Log::log(LogBuff, mLogFile, mLogUse);
#else
                        Log::log(QString("Data read successfully!"), mLogFile, mLogUse);
#endif
                    }
                    else
                    {
                        Log::log(QString("Error of serial port '%1' (%2)! %3").arg(ModBusCli->mSerialPortNum, QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mLogFile, mLogUse);
                    }
#endif
                    ModBusCli->disconnect();
                }
                else
                {
#ifdef LOG
                    Log::log(QString("Error of connection to serial port '%1' (%2)! %3").arg(ModBusCli->mSerialPortNum, QString::number(ModBusCli->getErrorNo()), ModBusCli->getError()), mLogFile, mLogUse);
#endif
                }

                delete ModBusCli;
            }
            else
            {
#ifdef LOG
                Log::log(QString("The data protocol '%1 %2' is not supported!").arg(DevIn->mProtoComm, DevIn->mProtoData), mLogFile, mLogUse);
#endif
            }
        }
    }

    return (Res);
}


/**
@brief  Save data of devices into DB.
@param  None.
@return The number of saved devices.
*/
int Survey::saveDataIntoDb()
{
    int Res = 0;

    if(!mConfigDevs->isEmpty())
    {
        if(mConfig->isDBData())
        {
            HelperMySQL *MySQLCli = new HelperMySQL();

            MySQLCli->mHost   = mConfig->mDBhost;
            MySQLCli->mUser   = mConfig->mDBuser;
            MySQLCli->mPasswd = mConfig->mDBpasswd;
            MySQLCli->mDB     = mConfig->mDBdatabase;

            if(MySQLCli->connect())
            {
                Device *Dev;
                int ResDev = 0;

                for(int i=0; i<mConfigDevs->size(); i++)
                {
                    Dev = mConfigDevs->getDevice(i);
                    ResDev = this->saveDevDataIntoDb(MySQLCli, Dev);
                    if(ResDev > 0) Res++;
                }
            }
            else
            {
#ifdef LOG
                Log::log(QString("Error connecting to DB (%1)! %2)").arg(QString::number(MySQLCli->getErrorNo()), MySQLCli->getError()), mLogFile, mLogUse);
#endif
            }
            delete MySQLCli;
        }
        else
        {
#ifdef LOG
            Log::log(QString("The DB settings is invalid!"), mLogFile, mLogUse);
#endif
        }
    }

    return (Res);
}


/**
@brief    Save data of Device into DB.
@param    MySqlCliIn - MySQL helper (connection must be established!);
          DevIn      - link to Dev.
@return   True if data saved successfully, otherwise - false..
*/
bool Survey::saveDevDataIntoDb(HelperMySQL *MySqlCliIn, Device *DevIn)
{
    bool Res = false;

    if(MySqlCliIn != NULL && DevIn != NULL)
    {
        if(MySqlCliIn->isConnected() && DevIn->isAllow())
        {
            QString Queries[2];

            DevIn->toTrgSql(mConfig->mDBtableTrg, mConfig->mProfile, Queries[0]);
            DevIn->toTrmSql(mConfig->mDBtableTrm, mConfig->mProfile, Queries[1]);

            for(int i=0; i<2; i++)
            {
                if(!Queries[i].isEmpty())
                {
#ifdef LOG
#ifdef DEBUG
                    Log::log(QString("%1").arg(Queries[i]), mLogFile, mLogUse);
#endif
#endif
                    if((Res = MySqlCliIn->sendQuery(Queries[i])))
                    {
#ifdef LOG
                        Log::log(QString("The query was sent successfully!"), mLogFile, mLogUse);
#endif
                        emit sigDevDataSaved();
                    }
                    else
                    {
#ifdef LOG
                        Log::log(QString("MySQLCli Error! (%1: %2)").arg(Queries[i], QString::number(MySqlCliIn->getErrorNo()), MySqlCliIn->getError()), mLogFile, mLogUse);
#endif
                        break;
                    }
                }
            }
        }
    }

    return (Res);
}


/**
@brief  Start survey process.
@param  None.
@return true if survey process was started, otherwise - false.
*/
bool Survey::start()
{
#ifdef LOG
    QString LogBuff = QString("<SURVEY PROCESS />\r\n\r\n");

    mConfig->toString(LogBuff);
    Log::log(LogBuff, mLogFile, mLogUse);

    LogBuff.clear();
    mConfigDevs->toString(LogBuff);
    Log::log(LogBuff, mLogFile, mLogUse);
#endif

#ifdef SURVEY__SINGLE_SHOT
    this->shot();
    return (true);
#else
    if(mSurveyTimer->isActive())
    {
        mSurveyTimer->stop();
    }

    //single shot after 1000 msec and start timer next
    QTimer::singleShot(1000, this, SLOT(shotTimer()));

    emit sigStarted();

    return (mSurveyTimer->isActive());
#endif
}


/**
@brief  Stop survey process.
@param  None.
@return true if survey process was stopped, otherwise - false.
*/
bool Survey::stop()
{
#ifdef EMULATOR
    return (true);
#else
    if(mSurveyTimer->isActive())
    {
        mSurveyTimer->stop();
    }

    emit sigStopped();

    return (!mSurveyTimer->isActive());
#endif
}


/**
@brief  Shot.
@param  None.
@return None.
*/
void Survey::shot()
{
#ifdef LOG
    QString LogBuff = QString("<SHOT>");
    Log::log(LogBuff, mLogFile, mLogUse);
#endif

    if(mConfig->mAllow)
    {
        int ResRead = this->readData();

        if(ResRead > 0 && mConfig->mDBallow)
        {
            this->saveDataIntoDb();
        }

        emit sigShot();
    }

#ifdef LOG
    LogBuff = QString("</SHOT>\r\n");
    Log::log(LogBuff, mLogFile, mLogUse);
#endif
}


/**
@brief  Start survey timer.
@param  None.
@return None.
*/
void Survey::startTimer()
{
    if(mSurveyTimer->isActive())
    {
        mSurveyTimer->stop();
    }

    if(mConfig->mSurveyPeriod > 0)
    {
        mSurveyTimer->start(mConfig->mSurveyPeriod);
    }
}


/**
@brief  Shot and start survey timer.
@param  None.
@return None.
*/
void Survey::shotTimer()
{
    this->shot();
    this->startTimer();
}
