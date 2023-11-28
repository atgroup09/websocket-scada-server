#ifndef SURVEY_H
#define SURVEY_H

#include <iostream>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <QHash>

#include "global.h"
#include "log.h"
#include "config.h"
#include "config-devices.h"
#include "mysql-cli.h"


/**
@brief Survey.
*/
class Survey : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit Survey(QObject *parent = 0);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Survey();


    /**
    Public options
    */

    /**
    @brief Path to Log-file.
    */
    QString mLogFile;

    /**
    @brief Log on/off.
    */
    bool mLogUse;


    /**
    Public methods
    */

    /**
    @brief  Read config from file (JSON).
    @param  FileIn - path to Config-file.
    @return true if Config was read, otherwise - false.
    */
    bool readConfigFromFile(const QString &FileIn);

    /**
    @brief  Read config of devices from file (JSON).
    @param  FileIn - path to DevMap-file.
    @return true if Map-list was read, otherwise - false.
    */
    bool readConfigDevFromFile(const QString &FileIn);

    /**
    @brief  Read config of devices from file (JSON).
    @param  None.
    @return true if Map-list was read, otherwise - false.
    */
    bool readConfigDevFromFile();

    /**
    @brief  Read data of devices.
    @param  None.
    @return The number of reading devices.
    */
    int readData();

    /**
    @brief    Read data of Device.
    @detailed supported classes of device: TRG
    @param    ModBusCliIn - link to configured helper of ModBus Client (connection must be established);
              DevIn - link to Dev.
    @return   The number of read registers.
    */
    //int readDevData(HelperModBusRTUClient *ModBusCliIn, Device *DevIn);
    int readDevData(Device *DevIn);

    /**
    @brief  Save data of devices into DB.
    @param  None.
    @return The number of saved devices.
    */
    int saveDataIntoDb();

    /**
    @brief    Save data of Device into DB.
    @param    MySqlCliIn - MySQL helper;
              DevIn      - link to Dev.
    @return   True if data saved successfully, otherwise - false..
    */
    bool saveDevDataIntoDb(HelperMySQL *MySqlCliIn, Device *DevIn);


signals:

    /**
    Signals
    */

    /**
    @brief  Survey started
    @param  None.
    @return None.
    */
    void sigStarted();

    /**
    @brief  Survey stopped
    @param  None.
    @return None.
    */
    void sigStopped();

    /**
    @brief  Shot completed
    @param  None.
    @return None.
    */
    void sigShot();

    /**
    @brief  Data of device channel saved in DB
    @param  None.
    @return None.
    */
    void sigDevDataSaved();


public slots:

    /**
    Public slots
    */

    /**
    @brief  Start survey process.
    @param  None.
    @return true if survey process was started, otherwise - false.
    */
    bool start();

    /**
    @brief  Stop survey process.
    @param  None.
    @return true if survey process was stopped, otherwise - false.
    */
    bool stop();


private:

    /**
    Private options
    */

    /**
    @brief Config.
    */
    Config *mConfig;

    /**
    @brief Config of devices.
    */
    ConfigDevices *mConfigDevs;

    /**
    @brief Survey timer.
    */
    QTimer *mSurveyTimer;


    /**
    Private methods
    */


private slots:

    /**
    Private slots
    */

    /**
    @brief  Shot.
    @param  None.
    @return None.
    */
    void shot();

    /**
    @brief  Start survey timer.
    @param  None.
    @return None.
    */
    void startTimer();

    /**
    @brief  Shot and start survey timer.
    @param  None.
    @return None.
    */
    void shotTimer();

};

#endif // SURVEY_H
