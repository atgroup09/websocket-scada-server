#ifndef HELPERMYSQL_H
#define HELPERMYSQL_H

#include <QObject>
#include <QString>
#include <sys/types.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <winsock2.h>
#include <windows.h>
#endif

#include "lib/mysql/include/mysql.h"


/**
@brief   MySQL DB Helper
*/
class HelperMySQL : public QObject
{
    Q_OBJECT

public:

    explicit HelperMySQL(QObject *parent = nullptr);
    ~HelperMySQL();

    /**
    Public constants
    */

    /**
    @brief      Constant: Class error codes
    */
    static const quint32 ERROR__INIT;

    /**
    @brief      Constant: Host names
    */
    static const QString HOST__LOCALHOST;

    /**
    @brief      Constant: Charsets
    */
    static const QString CHARSET__UTF8;


    /**
    Public options
    */

    /**
    @brief      Option: Host name
    @detailed   empty value is "localhost"
    */
    QString mHost;

    /**
    @brief      Option: User name
    */
    QString mUser;

    /**
    @brief      Option: User password
    @detailed   empty value or string
    */
    QString mPasswd;

    /**
    @brief      Option: The name of selected database
    @detailed   empty value of string
    */
    QString mDB;

    /**
    @brief      Option: Port number
    @detailed   3306 by default
    */
    quint32 mPort;

    /**
    @brief      Option: Used Character encoding
    @detailed   "utf8" by default
    */
    QString mCharacterSet;


    /**
    Public methods
    */

    /**
    @brief      Method: Check init. status.
    @param      None.
    @return     True if initialization is performed (ERROR__INIT), otherwise - false.
    */
    bool isInited();

    /**
    @brief      Method: Check connection.
    @param      None.
    @return     True if connection was established, otherwise - false.
    */
    bool isConnected();

    /**
    @brief      Method: Get Error code.
    @detailed   if no errors, then returns 0
    @param      None.
    @return     Error code.
    */
    quint32 getErrorNo();

    /**
    @brief      Method: Get Error message.
    @detailed   if no errors, then returns empty string
    @param      None.
    @return     Error code.
    */
    QString getError();

    /**
    @brief      Method: Get Resultset.
    @detailed   Resultset is NULL:
                    - for INSERT, UPDATE, DELETE, DROP queries (it is normally);
                    - for SELECT, SHOW, DESCRIBE, EXPLAIN queries if Error.
                Use mysql_free_result() for free memory!
    @param      None.
    @return     Pointer to Stored resultset or NULL.
    */
    MYSQL_RES *getResultset();


signals:

    /**
    Signals
    */

    /**
    @brief      Signal: Initialization is performed.
    @param      None.
    @return     None.
    */
    void sigInited();

    /**
    @brief      Signal: Connection was established successfully.
    @param      None.
    @return     None.
    */
    void sigConnected();

    /**
    @brief      Signal: Connection was closed successfully.
    @param      None.
    @return     None.
    */
    void sigDisconnected();

    /**
    @brief      Signal: Response to Ping.
    @param      None.
    @return     None.
    */
    void sigPong();

    /**
    @brief      Signal: DB selected.
    @param      None.
    @return     None.
    */
    void sigDBselected();

    /**
    @brief      Signal: Option was set successfully.
    @param      None.
    @return     None.
    */
    void sigOptionSet();

    /**
    @brief      Signal: Query was sent successfully.
    @param      None.
    @return     None.
    */
    void sigQuerySent();

    /**
    @brief      Signal: Error.
    @param      ErrorNoIn - error code;
                ErrorStrIn - error string.
    @return     None.
    */
    void sigError(quint32 ErrorNoIn, QString ErrorStrIn);


public slots:

    /**
    Public slots
    */

    /**
    @brief      Public slot: Init.
    @param      None.
    @return     True if initialization is performed, otherwise - false.
    */
    bool init();

    /**
    @brief      Public slot: Connect to server.
    @param      None.
    @return     True if connection was established successfully, otherwise - false.
    */
    bool connect();

    /**
    @brief      Public slot: Close connection with server.
    @param      None.
    @return     None.
    */
    void disconnect();

    /**
    @brief      Public slot: Test the connection.
    @param      None.
    @return     True if connection is established, otherwise - false.
    */
    bool ping();

    /**
    @brief      Public slot: Select the database.
    @param      DBNameIn - the name of database to select.
    @return     True if database was selected successfully, otherwise - false.
    */
    bool selectDB(const QString &DBNameIn);

    /**
    @brief      Public slot: Select the database.
    @param      None.
    @return     True if database was selected successfully, otherwise - false.
    */
    bool selectDB();

    /**
    @brief      Public slot: Set option.
    @param      OptionNameIn  - the option name;
                OptionValueIn - the option value.
    @return     True if the option was set successfully, otherwise - false.
    */
    bool setOption(mysql_option OptionNameIn, const QString &OptionValueIn);

    /**
    @brief      Public slot: Send Query.
    @param      QueryIn - SQL-query.
    @return     True if query was sent successfully, otherwise - false.
    */
    bool sendQuery(const QString &QueryIn);


private:

    /**
    Private options
    */

    /**
    @brief      Option: Connection descriptor
    */
    MYSQL *mMySQL;

    /**
    @brief      Option: Init. status
    */
    bool mInited;

    /**
    @brief      Option: Connect. status
    */
    bool mConnected;

    /**
    @brief      Option: Locker timeout (UNIX_TIMESTAMP)
    @detailed   lock work the class by date and time
                0 for disable the locker
    */
    quint32 mLocker;
};

#endif // HELPERMYSQL_H
