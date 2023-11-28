#ifndef ARGS_H
#define ARGS_H

#include <QObject>
#include <QString>
#include <QHash>


/**
@brief      Args parser.
*/
class Args : public QObject
{
    Q_OBJECT

public:

    explicit Args(QObject *parent = nullptr);
    ~Args();

    /**
    Public methods
    */

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
    static void parse(int argc, char *argv[], QHash<QString, QString> &result);

};

#endif // ARGS_H
