#include "args.h"


/**
@brief      Constructor.
@param      None.
@return     None.
*/
Args::Args(QObject *parent) : QObject(parent)
{

}


/**
@brief      Destructor.
@param      None.
@return     None.
*/
Args::~Args()
{

}


/**
@brief      Method: Parse input arguments.
@detailed   application --argName argValue ...
            result: QHash["app"] == QString("application")
                    QHash["argName"] == QString("argValue")
                    ...
@param      argc - size of argv;
            argv - list of input arguments;
            result - link to buffer to save result.
@return     None.
*/
void Args::parse(int argc, char *argv[], QHash<QString, QString> &result)
{
    if(argc > 0 && argv != nullptr)
    {
        result["app"] = argv[0];

        if(argc > 1)
        {
            QString Key, Value;
            bool initValue = false;

            for(int i=1; i<argc; i++)
            {
                if(!initValue)
                {
                    Key = QString(argv[i]);
                    initValue = true;
                }
                else
                {
                    Value = QString(argv[i]);
                    result.insert(Key, Value);
                    initValue = false;
                }
            }
        }
    }
}
