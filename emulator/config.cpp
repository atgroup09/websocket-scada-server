#include "config.h"


/**
@brief Constants: Fields
*/
const QString Config::FIELD__ID              = "ID";
const QString Config::FIELD__PORT            = "Port";
const QString Config::FIELD__CLI_MAX         = "CliMax";
const QString Config::FIELD__CLI_LIFE_TIME   = "CliLifeTime";
const QString Config::FIELD__CLI_BLACK_LIST  = "CliBlackList";


/**
@brief Constants: Values by default
*/
const quint16 Config::DEF__PORT              = 8100;
const quint8  Config::DEF__CLI_MAX           = 32;
const quint16 Config::DEF__CLI_LIFE_TIME     = 3600;


/**
@brief Constants: Limites
*/
const quint8  Config::LIM__CLI_MIN           = 0;
const quint8  Config::LIM__CLI_MAX           = 250;
const quint16 Config::LIM__CLI_LIFE_TIME_MIN = 0;
const quint16 Config::LIM__CLI_LIFE_TIME_MAX = 65000;


/**
@brief  Constructor.
@param  None.
@return None.
*/
Config::Config(QObject *parent) : QObject(parent)
{
    mLogFile = QString("");
    mLogUse  = false;

    this->setDef();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Config::~Config()
{
    mBlackList.clear();
}


/**
@brief  Method: Read data from file.
@param  FileIn - path to file.
@return true - successful, otherwise - false.
*/
bool Config::readFile(const QString &FileIn)
{
    bool Res = false;

    this->setDef();

    //read config from file
    QJsonDocument Doc = this->readJsonDoc(FileIn);

    if(Doc.isObject())
    {
       QJsonObject DataObj = Doc.object();
       Res = this->parse(DataObj);

       if(Res)
       {
           QString BlackListFile = DataObj.value(FIELD__CLI_BLACK_LIST).toString(QString());

           //read black-list from file
           Doc = this->readJsonDoc(BlackListFile);
           if(Doc.isArray())
           {
               QJsonArray DataArr = Doc.array();
               this->parseBlackList(DataArr);
           }
       }
    }

#ifdef LOG
    QString LogBuff = QString();
    this->toString(LogBuff);
    Log::log(LogBuff, mLogFile, mLogUse);
#endif

    return (Res);
}


/**
@brief  Method: Set values by default.
@param  None.
@return None.
*/
void Config::setDef()
{
    mID          = QString("NONE");
    mPort        = Config::DEF__PORT;
    mCliMax      = Config::DEF__CLI_MAX;
    mCliLifeTime = Config::DEF__CLI_LIFE_TIME;
    mBlackList.clear();
}


/**
@brief  Method: Pack data to String.
@param  StringIn - link to buffer.
@return None.
*/
void Config::toString(QString &StringIn)
{
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
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__CLI_MAX;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mCliMax);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= Config::FIELD__CLI_LIFE_TIME;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mCliLifeTime);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - BlackList.size() = ");
    StringIn+= QString::number(mBlackList.size());
    StringIn+= QString("\r\n");

    QList<QString> Keys = mBlackList.keys();
    QString Key;

    for(int i=0; i<Keys.size(); i++)
    {
        Key = Keys.at(i);
        StringIn+= QString(" -- client[%1] = %2:%3").arg(QString::number(i), Key, QString::number(mBlackList.value(Key)));
        StringIn+= QString("\r\n");
    }
}


/**
@brief  Method: Read data from JSON-file.
@param  FileIn - path to file.
@return QJsonDocument.
*/
QJsonDocument Config::readJsonDoc(const QString &FileIn)
{
    QJsonDocument Res;

    if(FileIn != nullptr)
    {
        QFile File(FileIn);

        if(File.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QString Data = File.readAll();
            Res = QJsonDocument::fromJson(Data.toUtf8());
            File.close();
        }
        else
        {
#ifdef LOG
        Log::log(QString("Error open file '%1'!").arg(FileIn), mLogFile, mLogUse);
#endif
        }
    }
    else
    {
#ifdef LOG
        Log::log(QString("Error read file 'NULL'!"), mLogFile, mLogUse);
#endif
    }

    return (Res);
}


/**
@brief  Private method: Parse data that was read from file.
@param  DataIn - link to JsonObject.
@return true - success, otherwise - false.
*/
bool Config::parse(const QJsonObject &DataIn)
{
    if(!DataIn.isEmpty())
    {
        mID          = DataIn.value(FIELD__ID).toString(QString("NONE"));
        mPort        = static_cast<quint16>(DataIn.value(FIELD__PORT).toInt(Config::DEF__PORT));
        mCliMax      = static_cast<quint8>(DataIn.value(FIELD__CLI_MAX).toInt(Config::DEF__CLI_MAX));
        mCliLifeTime = static_cast<quint16>(DataIn.value(FIELD__CLI_LIFE_TIME).toInt(Config::DEF__CLI_LIFE_TIME));

        return (true);
    }

    return (false);
}


/**
@brief  Private method: Read black-list from JSON-file.
@param  DataIn - link to JsonArray.
@return true - successful, otherwise - false.
*/
bool Config::parseBlackList(const QJsonArray &DataIn)
{
    if(!DataIn.isEmpty())
    {
        QJsonValue Item;
        QStringList Parts;
        QString MapKey;
        bool Ok;

        for(int i=0; i<DataIn.size(); i++)
        {
            Item = DataIn.at(i);

            if(Item.isString())
            {
                Parts = Item.toString(QString()).split(':');

                if(Parts.size())
                {
                    MapKey = Parts.at(0);
                    mBlackList[MapKey] = ((Parts.size() > 1) ? static_cast<quint16>(Parts.at(1).toInt(&Ok)) : 0);
                }
            }
        }
    }

    return (false);
}
