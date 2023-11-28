#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "global.h"
#include "log.h"


/**
    config.json (example):

    {           "ID": "WsChart",
              "Port": 8100,
            "CliMax": 30,
       "CliLifeTime": 60,
      "CliBlackList": "black.json"
    }

    black.json (example):

    ["192.168.1.220",
     "192.168.1.77:1234"
    ]
*/


/**
@brief    Config.
@detailed Read data from JSON-file and parse it.
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
    @brief Constants: Fields
    */
    static const QString FIELD__ID;
    static const QString FIELD__PORT;
    static const QString FIELD__CLI_MAX;
    static const QString FIELD__CLI_LIFE_TIME;
    static const QString FIELD__CLI_BLACK_LIST;

    /**
    @brief Constants: Values by default
    */
    static const quint16 DEF__PORT;
    static const quint8  DEF__CLI_MAX;
    static const quint16 DEF__CLI_LIFE_TIME;

    /**
    @brief Constants: Limites
    */
    static const quint8  LIM__CLI_MIN;
    static const quint8  LIM__CLI_MAX;
    static const quint16 LIM__CLI_LIFE_TIME_MIN;
    static const quint16 LIM__CLI_LIFE_TIME_MAX;


    /**
    Public options
    */

    /**
    @brief Option: Server ID
    */
    QString mID;

    /**
    @brief Option: Port number
    */
    quint16 mPort;

    /**
    @brief Option: Clients (max)
    */
    quint8 mCliMax;

    /**
    @brief Option: Life time of client (seconds)
    @detailed 0 seconds - unlimited life time
    */
    quint16 mCliLifeTime;

    /**
    @brief Option: Black-list
    @detailed ["Client-IP"] = Client-Port
              ...
              Client-Port 0 - All ports
    */
    QMap<QString, quint16> mBlackList;

    /**
    @brief Option: Log-file.
    */
    QString mLogFile;

    /**
    @brief Log on\off.
    */
    bool mLogUse;



    /**
    Public methods
    */

    /**
    @brief  Method: Read data from file.
    @param  FileIn - path to file.
    @return true - successful, otherwise - false.
    */
    bool readFile(const QString &FileIn);

    /**
    @brief  Method: Set values by default.
    @param  None.
    @return None.
    */
    void setDef();

    /**
    @brief  Method: Pack data to String.
    @param  StringIn - link to buffer.
    @return None.
    */
    void toString(QString &StringIn);


protected:


private:

    /**
    Private methods
    */

    /**
    @brief  Private Method: Read data from JSON-file.
    @param  FileIn - path to file.
    @return QJsonDocument.
    */
    QJsonDocument readJsonDoc(const QString &FileIn);

    /**
    @brief  Private method: Parse data that was read from file.
    @param  DataIn - link to JsonObject.
    @return true - success, otherwise - false.
    */
    bool parse(const QJsonObject &DataIn);

    /**
    @brief  Private method: Read black-list from JSON-file.
    @param  DataIn - link to JsonArray.
    @return true - successful, otherwise - false.
    */
    bool parseBlackList(const QJsonArray &DataIn);


signals:


public slots:


};

#endif // CONFIG_H
