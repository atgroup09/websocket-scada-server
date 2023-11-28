/* Copyright (C) 2019-2023 ATgroup09
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

#ifndef REGISTER_H
#define REGISTER_H

#include <QObject>
#include <QChar>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDateTime>

#include "bit.h"
#include "log.h"
#include "json.h"
#include "global.h"
#include "event.h"
#include "type.h"


/**
@brief Register.
*/
class Register : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit Register(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~Register();


    /**
    Public constants
    */

    /**
    @brief Fields
    */
    static const QString FIELD__ID;
    static const QString FIELD__ADDR;
    static const QString FIELD__CLASS;
    static const QString FIELD__TYPE;
    static const QString FIELD__OFFSET;
    static const QString FIELD__ROUND;
    static const QString FIELD__VAR;
    static const QString FIELD__ALLOW;
    static const QString FIELD__VALUE;
    static const QString FIELD__EVENTS;
    static const QString FIELD__TARGETS;
    static const QString FIELD__ALG;
    static const QString FIELD__STAMP;
    static const QString FIELD__SIGN;
    static const QString FIELD__EX;
    static const QString FIELD__ERR;

    //** random
    static const QString FIELD__RAND_MIN;
    static const QString FIELD__RAND_MAX;

    //** device
    static const QString FIELD__DEV_ID;
    static const QString FIELD__DEV_CLASS;

    /**
    @brief Classes (tables) of a register
    */
    static const QString CLASS__CONST;
    static const QString CLASS__RANDOM;
    static const QString CLASS__CALC;
    static const QString CLASS__COIL;
    static const QString CLASS__DISC;
    static const QString CLASS__IN;
    static const QString CLASS__INPT;
    static const QString CLASS__HOLDING;
    static const QString CLASS__6;

    /**
    @brief Data types of a register
    */
    static const QString TYPE__BOOL;
    static const QString TYPE__WORD;
    static const QString TYPE__INT;
    static const QString TYPE__FLOAT;
    static const QString TYPE__RAW;

    /**
    @brief Calculation agorithms
    */
    static const QString CALC_ALG__MERGE32;
    static const QString CALC_ALG__DWORD;
    static const QString CALC_ALG__FLOAT;
    static const QString CALC_ALG__BITSTODWORD;

    /**
    @brief Limites
    */
    static const qint8 OFFSET_MIN  = 1;
    static const qint8 ROUND_MIN   = 0;
    static const qint8 ROUND_MAX   = 10;
    static const qint8 ROUND_UNDEF = 2;

    /**
    @brief Permission codes
    */
    static const QChar ALLOW_CODE__READ;
    static const QChar ALLOW_CODE__WRITE;
    static const QChar ALLOW_CODE__HMI;
    static const QChar ALLOW_CODE__ARH;
    static const QChar ALLOW_CODE__RAND;
    static const quint8 DENY  = 0;
    static const quint8 ALLOW = 1;

    /**
    @brief Word settings
    */
    static const quint16 WORD_UNSIGN_MAX = 32767;
    static const int WORD_SIGN           = 65536;

#ifdef SQL_PROC_TRM
    /**
    @brief Predefined values of mVar
    @detals "Obj" for archiving `obj_status`,`prog`,`step`
            { "ID":1, "Addr":1, "Class":"in", "Type":"word", "Var":"Stage", "Allow":"r|h|d", "Value":1, "RandMin":1, "RandMax":10 }, --read from device
            { "ID":2, "Addr":2, "Class":"in", "Type":"word", "Var":"Prog", "Allow":"r|h|d", "Value":1, "RandMin":1, "RandMax":2 },   --read from device
            { "ID":3, "Addr":3, "Class":"in", "Type":"word", "Var":"Step", "Allow":"r|h|d", "Value":1, "RandMin":1, "RandMax":3 },   --read from device
            { "ID":4, "Addr":4, "Class":"calc", "Type":"int", "Var":"Obj", "Allow":"r|h|a", "Alg":"none", "Targets":[1, 2, 3] }      --archiving Stage, Prog, Step by links in "Targets"
    */
    static const QString VAR__TRM_OBJ;
#endif

    /**
    Public options
    */

    /**
    @brief ID.
    */
    quint16 mID;

    /**
    @brief Address.
    */
    quint16 mAddr;

    /**
    @brief Class.
    */
    QString mClass;

    /**
    @brief Data type.
    */
    QString mType;

    /**
    @brief Offset of value decimal pointer to right.
    */
    qint8 mOffset;

    /**
    @brief The number of digits after the decimal point that will be saved after rounding.
    */
    qint8 mRound;

    /**
    @brief Name (aliase).
    */
    QString mVar;

    /**
    @brief Mask of permission codes.
    */
    QString mAllowMask;

    /**
    @brief Permission to read data.
    */
    bool mAllowRead;

    /**
    @brief Permission to write data.
    */
    bool mAllowWrite;

    /**
    @brief Permission to send data to a HMI-client.
    */
    bool mAllowHmi;

    /**
    @brief Permission to save data into an archive.
    */
    bool mAllowArh;

    /**
    @brief Permission to randomize data.
    */
    bool mAllowRand;

    /**
    @brief Minimum of randomized value.
    */
    quint16 mRandMin;

    /**
    @brief Maximum of randomized value.
    */
    quint16 mRandMax;

    /**
    @brief Raw-value.
    */
    quint16 mValue;

    /**
    @brief Path to a file that contains list of event settings (JSON).
    */
    QString mFileEvents;

    /**
    @brief List of ID of target registers (from file), that will be used in calculating.
    @details [0] = TargetRegID;
             [1] = TargetRegID;
             ...
             [n] = TargetRegID;
    */
    QList<quint16> mListTargetsToCalc;

    /**
    @brief List of pointers to values of target registers (set manually), that will be used in calculating.
    @details [0] = &TargetRegValue;
             [1] = &TargetRegValue;
             ...
             [n] = &TargetRegValue;
    */
    QList<quint16 *> mListValuesToCalc;

    /**
    @brief Calculation algorithm.
    */
    QString mAlg;

    /**
    @brief Device ID.
    */
    quint16 mDevID;

    /**
    @brief Device class.
    */
    QString mDevClass;

    /**
    @brief Use log.
    */
    bool mUseLog;

    /**
    @brief Path to a file when will be write service data.
    */
    QString mFileLog;


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
    @brief  Pack data to JSON buffer.
    @param  ObjIn - link to JsonObject.
    @return None.
    @detailed { ..., Register.mVar:FormattedValue }
    */
    void toJson(QJsonObject &ObjIn);

    /**
    @brief  Pack value of the register into String-buffer in SQL-format (only value).
    @param  StampIn - datetime stamp;
    @param  ProfileIn - name of profile;
    @param  DevIdIn - device ID;
    @param  RegIdIn - name of profile;
    @param  ValueIn - formatted value;
    @param  ExIn - exception code;
    @param  ErrIn - error code;
    @param  SignIn - sign code;
    @param  StringIn - link to string buffer.
    @return true if OK, otherwise - false.
    @detailed
        Output SQL: "('{Stamp in ISO-format}', '{ProfileIn}', {DevIdIn}, {RegIdIn}, {ValueIn}, {mExLast}, {mErrLast}, {mSignLast})"
        if ProfileIn is empty, then will be used NULL
        if DevIdIn is 0, then will be used NULL
        if RegIdIn is 0, then will be used NULL
    */
    static bool toSqlValue(const QDateTime &StampIn, const QString &ProfileIn, const quint16 DevIdIn, const quint16 RegIdIn, const QString ValueIn, const int ExIn, const int ErrIn, const int SignIn, QString &StringIn);

    /**
    @brief  Pack current value of the register into String-buffer in SQL-format (only value).
    @param  ProfileIn - name of profile;
    @param  StringIn - link to string buffer.
    @return true if OK, otherwise - false.
    @detailed
        Output SQL: "('{mStamp in ISO-format}', '{ProfileIn}', {mDevID}, {mID}, {FormattedValue}, {mExLast}, {mErrLast}, {mSignLast})"
        if ProfileIn is empty, then will be used NULL
        if mDevID is 0, then will be used NULL
        if mID is 0, then will be used NULL
        Required: mAllowArh = true
    */
    bool toSqlCurrentValue(const QString &ProfileIn, QString &StringIn);

    /**
    @brief  Pack current value of the register into String-buffer in SQL-format (only value).
    @param  ProfileIn - name of profile;
    @param  ListStringsIn - link to list of string buffers.
    @return The number of packed values.
    @detailed
        Output SQL:
                [0] = "('{mStamp in ISO-format}', '{ProfileIn}', {mDevID}, {mID}, {FormattedValue}, {mExLast}, {mErrLast}, {mSignLast})"
        if ProfileIn is empty, then will be used NULL
        if mDevID is 0, then will be used NULL
        if mID is 0, then will be used NULL
        Required: mAllowArh = true
    */
    int toSqlCurrentValue(const QString &ProfileIn, QList<QString> &ListStringsIn);

    /**
    @brief  Pack event values of the register into String-buffer in SQL-format (only values).
    @param  ProfileIn - name of profile;
    @param  ListStringsIn - link to list of string buffers.
    @return The number of packed values.
    @detailed
        Output SQL:
                [0] = "('{Event[0].mStamp in ISO-format}', 'event', {mDevID}, {mID}, {Event[0].FormattedValue}, {Event[0].mEx}, {Event[0].mErr}, {Event[0].mSign})";
                [1] = "('{Event[1].mStamp in ISO-format}', 'event', {mDevID}, {mID}, {Event[1].FormattedValue}, {Event[1].mEx}, {Event[1].mErr}, {Event[1].mSign})";
                ...
                [n] = "('{Event[n].mStamp in ISO-format}', 'event', {mDevID}, {mID}, {Event[n].FormattedValue}, {Event[n].mEx}, {Event[n].mErr}, {Event[n].mSign})";
        if mDevID is 0, then will be used NULL
        if mID is 0, then will be used NULL
        Required: mAllowArh = true
    */
    int toSqlEventValue(const QString &ProfileIn, QList<QString> &ListStringsIn);

    /**
    @brief  Unpack Raw-value (uint16) into Int.
    @param  RawValueIn - raw-value.
    @return Unpacked value of type Int.
    @detailed Will be unpack sign; offset as is.
    */
    static int toInt(const quint16 RawValueIn);

    /**
    @brief  Decode Offset.
    @param  In - Float (coded by offset);
    @param  OffsetIn - offset;
    @return Decoded Float.
    @detailed Result = CodedIn/OffsetIn.
    */
    static float decodeOffset(float In, qint8 OffsetIn);

    /**
    @brief  Decode Offset.
    @param  In - Int (coded by offset);
    @param  OffsetIn - offset;
    @return Decoded Float.
    @detailed Result = CodedIn/OffsetIn.
    */
    static float decodeOffset(int In, qint8 OffsetIn);

    /**
    @brief  Code Offset.
    @param  In - Float;
    @param  OffsetIn - offset;
    @return Coded Float.
    @detailed Result = In*OffsetIn.
    */
    static float codeOffset(float In, qint8 OffsetIn);

    /**
    @brief  Round.
    @param  In - Float;
    @param  RoundIn - round;
    @return Rounded Float.
    @detailed Result = round(In, RoundIn).
    */
    static float setRound(float In, qint8 RoundIn);

    /**
    @brief  Unpack Raw-value (uint16) into Float.
    @param  RawValueIn - raw-value;
    @param  OffsetIn - offset;
    @param  RoundIn - round.
    @return Unpacked value of type Float.
    @detailed Will be unpack sign and offset, set round.
    */
    static float toFloat(const quint16 RawValueIn, const qint8 OffsetIn, const qint8 RoundIn);

    /**
    @brief  Unpack Raw-value (uint16) into Boolean.
    @param  RawValueIn - raw-value.
    @return Unpacked value of type Boolean.
    */
    static bool toBool(const quint16 RawValueIn);

    /**
    @brief  Merge two Raw-values (uint16) from a list into DWord.
    @param  ListRawValuesIn - list of raw-values.
    @return Merged DWord.
    */
    static quint32 mergeDword(const QList<quint16 *> *ListRawValuesIn);

    /**
    @brief  Merge two Raw-values (uint16) from a list into Float.
    @param  ListRawValuesIn - list of raw-values.
    @return Merged Float.
    */
    static float mergeFloat(const QList<quint16 *> *ListRawValuesIn);

    /**
    @brief  Merge bits into DWord.
    @param  ListRawValuesIn - list of raw-values.
    @return Merged DWord.
    */
    static quint32 mergeBitsToDword(const QList<quint16 *> *ListRawValuesIn);

    /**
    @brief  Check option "Addr".
    @param  None.
    @return True if value of option "Addr" is correct, otherwise - False.
    */
    bool isCorrectAddr();

    /**
    @brief  Check option "Class".
    @param  None.
    @return True if value of option "Class" is correct, otherwise - False.
    */
    bool isCorrectClass();

    /**
    @brief  Check option "Type".
    @param  None.
    @return True if value of option "Type" is correct, otherwise - False.
    */
    bool isCorrectType();

    /**
    @brief  Check option "Var".
    @param  None.
    @return True if value of option "Var" is correct, otherwise - False.
    */
    bool isCorrectVar();

    /**
    @brief  Check options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool isCorrect();

    /**
    @brief  Check readness of events.
    @param  None.
    @return True if one from events is ready, otherwise - False.
    */
    bool isEventsReady();

    /**
    @brief  Normilize options.
    @param  None.
    @return True if value of options are correct, otherwise - False.
    */
    bool normilize();

    /**
    @brief  Read list of event settings from a file.
    @param  FileIn - path to a file.
    @return The number of event settings have read.
    */
    quint16 readFileEvents(const QString &FileIn);

    /**
    @brief  Clear list of event settings.
    @param  None.
    @return None.
    */
    void clearListEvents();

    /**
    @brief  Get size of list of event settings.
    @param  None.
    @return Size of list of event settings.
    */
    quint16 sizeListEvents();

    /**
    @brief  Check size of list of event settings.
    @param  None.
    @return True if list of event settings is empty, otherwise - False.
    */
    bool isListEventsEmpty();

    /**
    @brief  Get randomized value.
    @param  None.
    @return Randomized value.
    */
    int randomize();

    /**
    @brief  Set value.
    @param  ValueIn - value.
    @return Packed value.
    */
    quint16 setValue(float ValueIn);

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
    @brief  Pack value of stamp into string buffer in ISO-format.
    @param  StampIn - stamp;
    @param  BuffIn - link to string buffer.
    @return None.
    */
    static void packStampISO(const QDateTime &StampIn, QString &BuffIn);

    /**
    @brief  Pack value of stamp into string buffer in ISO-format.
    @param  BuffIn - link to string buffer.
    @return None.
    */
    void packStampISO(QString &BuffIn);

    /**
    @brief  Pack formatted value into QJsonValue.
    @param  RawValueIn - raw-value;
    @param  TypeIn - output data type;
    @param  OffsetIn - offset;
    @param  RoundIn - round;
    @param  BuffIn - link to buffer.
    @return None.
    */
    static void packFormattedValue(const quint16 RawValueIn, const QString TypeIn, const qint8 OffsetIn, const qint8 RoundIn, QJsonValue &BuffIn);

    /**
    @brief  Pack calculated value into QJsonValue.
    @param  ListRawValuesIn - list of raw-values;
    @param  AlgIn - calculation algorithm;
    @param  RoundIn - round (for float);
    @param  BuffIn - link to buffer.
    @return None.
    */
    static void packCalculatedValue(const QList<quint16 *> ListRawValuesIn, const QString AlgIn, const qint8 RoundIn, QJsonValue &BuffIn);

    /**
    @brief  Pack formatted value into QJsonValue.
    @param  BuffIn - link to buffer.
    @return None.
    */
    void packFormattedValue(QJsonValue &BuffIn);

    /**
    @brief  Pack formatted value into QString.
    @param  RawValueIn - raw-value;
    @param  TypeIn - output data type
    @param  OffsetIn - offset;
    @param  RoundIn - round;
    @param  BuffIn - link to buffer.
    @return None.
    */
    static void packFormattedValue(const quint16 RawValueIn, const QString TypeIn, const qint8 OffsetIn, const qint8 RoundIn, QString &BuffIn);

    /**
    @brief  Pack calculated value into QString.
    @param  ListRawValuesIn - list of raw-values;
    @param  AlgIn - calculation algorithm;
    @param  RoundIn - round (for float);
    @param  BuffIn - link to buffer.
    @return None.
    */
    static void packCalculatedValue(const QList<quint16 *> ListRawValuesIn, const QString AlgIn, const qint8 RoundIn, QString &BuffIn);

    /**
    @brief  Pack formatted value into QString.
    @param  BuffIn - link to buffer.
    @return None.
    */
    void packFormattedValue(QString &BuffIn);

    /**
    @brief  Add value of target register to calculate.
    @param  IDIn - ID of target register;
    @param  ValueIn - pointer to raw-value.
    @return true if the value is set, otherwise - false.
    */
    bool addCalcTargets(quint16 IDIn, quint16 *ValueIn);


private:

    /**
    Private options
    */

    /**
    @brief Stamp.
    */
    QDateTime mStamp;

    /**
    @brief Last exception code.
    */
    int mExLast;

    /**
    @brief Last error code.
    */
    int mErrLast;

    /**
    @brief Last sign code.
    */
    int mSignLast;

    /**
    @brief List of event settings.
    */
    QList<Event *> mListEvents;


    /**
    Private methods
    */

    /**
    @brief  Parse data for list of event settings.
    @param  DocIn - link to JsonDocument.
    @return The number of event settings have parsed.
    */
    quint16 parseDataEvents(const QJsonDocument &DocIn);

    /**
    @brief  Parse data for list of ID of target registers.
    @param  ArrIn - link to JsonArray.
    @return The number of targets have parsed.
    */
    quint16 parseDataTargets(const QJsonArray &ArrIn);
};

#endif // REGISTER_H
