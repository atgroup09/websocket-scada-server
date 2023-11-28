/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  This file is part of the HMI server component.
*
*  The code in this page is free software: you can
*  redistribute it and/or modify it under the terms of the GNU
*  General Public License (GNU GPL) as published by the Free Software
*  Foundation, either version 3 of the License, or (at your option)
*  any later version.  The code is distributed WITHOUT ANY WARRANTY;
*  without even the implied warranty of MERCHANTABILITY or FITNESS
*  FOR A PARTICULAR PURPOSE.  See the GNU GPL for more details.
*
*  As additional permission under GNU GPL version 3 section 7, you
*  may distribute non-source (e.g., minimized or compacted) forms of
*  that code without the copy of the GNU GPL normally required by
*  section 4, provided you include this license notice and a URL
*  through which recipients can access the Corresponding Source.
*/

#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QChar>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDateTime>


/**
@brief Event settings.
*/
class Event : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit Event(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Event();


    /**
    Public constants
    */

    /**
    @brief Fields
    */
    static const QString FIELD__ALG;
    static const QString FIELD__MIN;
    static const QString FIELD__MAX;
    static const QString FIELD__REF;
    static const QString FIELD__SIGN;
    static const QString FIELD__EX;
    static const QString FIELD__ERR;
    static const QString FIELD__STAMP;
    static const QString FIELD__STAMP_CONF;

    /**
    @brief Algorithms
    */
    static const QString ALG__OUTLIM;       //"OutLim"
    static const QString ALG__REF_EQ;       //"=Ref"
    static const QString ALG__REF_GREATER;  //">Ref"
    static const QString ALG__REF_LESS;     //"<Ref"


    /**
    Public options
    */

    /**
    @brief Algorithm.
    */
    QString mAlg;

    /**
    @brief Minimum.
    @details for "Alg":"OutLim"
    */
    int mMin;

    /**
    @brief Maximum.
    @details for "Alg":"OutLim"
    */
    int mMax;

    /**
    @brief Reference / Setpoint.
    @details for "Alg":"=Ref|>Ref|<Ref"
    */
    int mRef;

    /**
    @brief Exception code.
    */
    qint16 mEx;

    /**
    @brief Error code.
    */
    qint16 mErr;

    /**
    @brief Sign code.
    */
    qint16 mSign;

    /**
    @brief Raw-value.
    */
    quint16 mValue;


    /**
    Public methods
    */

    /**
    @brief  Set values by default.
    @param  None.
    @return None.
    */
    void byDefault();

    /**
    @brief  Load values from JsonObject.
    @param  DataIn - data.
    @return True if value of options are correct, otherwise - False.
    */
    bool fromJson(const QJsonObject &DataIn);

    /**
    @brief  Pack data to String buffer.
    @param  StringIn - link to string buffer.
    @return None.
    */
    void toString(QString &StringIn);

    /**
    @brief  Check option "Alg".
    @param  None.
    @return True if value of option "Alg" is correct, otherwise - False.
    */
    bool isCorrectAlg();

    /**
    @brief  Check options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool isCorrect();

    /**
    @brief  Refresh stamp.
    @param  None.
    @return New stamp.
    */
    QDateTime refreshStamp();

    /**
    @brief  Get value of stamp.
    @param  None.
    @return Value of stamp.
    */
    QDateTime getStamp();

    /**
    @brief  Reset event.
    @param  None.
    @return None.
    @details
    */
    void reset();

    /**
    @brief  Confirm event.
    @param  None.
    @return None.
    */
    void confirm();

    /**
    @brief  Check readness of event.
    @param  None.
    @return True if event is ready, otherwise - False.
    @details If mStamp != mStampConfirmed
    */
    bool isReady();

    /**
    @brief  Check confirmation of event.
    @param  None.
    @return True if event is confirmed, otherwise - False.
    @details If mStamp == mStampConfirmed
    */
    bool isConfirmed();


private:

    /**
    Private options
    */

    /**
    @brief Stamp.
    */
    QDateTime mStamp;
    QDateTime mStampConfirmed;
};

#endif // EVENT_H
