#include <QCoreApplication>

#include "global.h"
#include "args.h"
#include "server.h"


/**
    Input arguments:

    --config PathToConfigFile [ --log PathToLogFile --sname ServiceName --sdesc ServiceDescription ]

    [  ] - optional arguments
*/

int main(int argc, char *argv[])
{
    QHash<QString, QString> ParsedArgs;
    Args::parse(argc, argv, ParsedArgs);

    QCoreApplication a(argc, argv);

    QString ConfigFile = ParsedArgs.value(QString(ARG_KEY_CONFIG), QString(""));
    QString LogFile    = ParsedArgs.value(QString(ARG_KEY_LOG), QString(""));

    Server srv(ConfigFile, LogFile);

    return a.exec();
}
