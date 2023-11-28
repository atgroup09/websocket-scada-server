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

#include "register.h"

/**
@brief Fields
*/
const QString Register::FIELD__ID          = "ID";
const QString Register::FIELD__ADDR        = "Addr";
const QString Register::FIELD__CLASS       = "Class";
const QString Register::FIELD__TYPE        = "Type";
const QString Register::FIELD__OFFSET      = "Offset";
const QString Register::FIELD__ROUND       = "Round";
const QString Register::FIELD__VAR         = "Var";
const QString Register::FIELD__ALLOW       = "Allow";
const QString Register::FIELD__VALUE       = "Value";
const QString Register::FIELD__EVENTS      = "Events";
const QString Register::FIELD__TARGETS     = "Targets";
const QString Register::FIELD__ALG         = "Alg";
const QString Register::FIELD__STAMP       = "Stamp";
const QString Register::FIELD__SIGN        = "Sign";
const QString Register::FIELD__EX          = "Ex";
const QString Register::FIELD__ERR         = "Err";

//** random
const QString Register::FIELD__RAND_MIN    = "RandMin";
const QString Register::FIELD__RAND_MAX    = "RandMax";

//** device
const QString Register::FIELD__DEV_ID      = "DevID";
const QString Register::FIELD__DEV_CLASS   = "DevClass";

/**
@brief Classes of a register.
*/
const QString Register::CLASS__CONST       = "const";
const QString Register::CLASS__RANDOM      = "random";
const QString Register::CLASS__CALC        = "calc";
const QString Register::CLASS__COIL        = "coil";
const QString Register::CLASS__DISC        = "disc";
const QString Register::CLASS__IN          = "in";
const QString Register::CLASS__INPT        = "inpt";
const QString Register::CLASS__HOLDING     = "hold";
const QString Register::CLASS__6           = "6";

/**
@brief Data types of a register.
*/
const QString Register::TYPE__BOOL         = "bool";
const QString Register::TYPE__WORD         = "word";
const QString Register::TYPE__INT          = "int";
const QString Register::TYPE__FLOAT        = "float";
const QString Register::TYPE__RAW          = "raw";

/**
@brief Calculation agorithms
*/
const QString Register::CALC_ALG__MERGE32     = "merge32";
const QString Register::CALC_ALG__DWORD       = "dword";
const QString Register::CALC_ALG__FLOAT       = "float";
const QString Register::CALC_ALG__BITSTODWORD = "bitstodword";


/**
@brief Permission codes.
*/
const QChar Register::ALLOW_CODE__READ     = 'r';
const QChar Register::ALLOW_CODE__WRITE    = 'w';
const QChar Register::ALLOW_CODE__HMI      = 'h';
const QChar Register::ALLOW_CODE__ARH      = 'a';
const QChar Register::ALLOW_CODE__RAND     = 'd';

#ifdef SQL_PROC_TRM
/**
@brief Predefined values of mVar
@detals "Obj" for archiving `obj_status`,`prog`,`step`
*/
const QString Register::VAR__TRM_OBJ       = "Obj";
#endif


/**
@brief  Constructor.
@param  None.
@return None.
*/
Register::Register(QObject *parent) : QObject(parent)
{
    mDevID    = 0;
    mDevClass = QString("");
    mUseLog   = false;
    mFileLog  = QString("");

    this->byDefault();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Register::~Register()
{
    mListTargetsToCalc.clear();
    mListValuesToCalc.clear();
    this->clearListEvents();
}


/**
@brief  Set values by default.
@param  None.
@return None.
*/
void Register::byDefault()
{
    mID            = 0;
    mAddr          = 0;
    mClass         = CLASS__CONST;
    mType          = TYPE__RAW;
    mOffset        = OFFSET_MIN;
    mRound         = ROUND_UNDEF;
    mVar           = QString("");
    mAllowMask     = QString("");
    mAllowRead     = false;
    mAllowWrite    = false;
    mAllowHmi      = false;
    mAllowArh      = false;
    mAllowRand     = false;
    mValue         = 0;
    mFileEvents    = QString("");
    mAlg           = QString("");
    mRandMin       = 0;
    mRandMax       = 0;
    mExLast        = -1;
    mErrLast       = 0;
    mSignLast      = 0;

    mListTargetsToCalc.clear();
    mListValuesToCalc.clear();
    this->clearListEvents();
    this->refreshStamp();
}


/**
@brief  Load values from JsonObject.
@param  DataIn - data.
@return True if value of options are correct, otherwise - False.
*/
bool Register::fromJson(const QJsonObject &DataIn)
{
    this->byDefault();

    if(!DataIn.isEmpty())
    {
        mID         = static_cast<quint16>(DataIn.value(FIELD__ID).toInt(0));
        mAddr       = static_cast<quint16>(DataIn.value(FIELD__ADDR).toInt(0));
        mClass      = DataIn.value(FIELD__CLASS).toString(CLASS__CONST);
        mType       = DataIn.value(FIELD__TYPE).toString(TYPE__RAW);
        mOffset     = static_cast<qint8>(DataIn.value(FIELD__OFFSET).toInt(OFFSET_MIN));
        mRound      = static_cast<qint8>(DataIn.value(FIELD__ROUND).toInt(ROUND_UNDEF));
        mVar        = DataIn.value(FIELD__VAR).toString(QString(""));
        mAllowMask  = DataIn.value(FIELD__ALLOW).toString(QString(""));
        mValue      = static_cast<quint16>(DataIn.value(FIELD__VALUE).toInt(0));
        mFileEvents = DataIn.value(FIELD__EVENTS).toString(QString(""));
        mAlg        = DataIn.value(FIELD__ALG).toString(QString(""));
        mAllowRead  = mAllowMask.contains(ALLOW_CODE__READ);
        mAllowWrite = mAllowMask.contains(ALLOW_CODE__WRITE);
        mAllowHmi   = mAllowMask.contains(ALLOW_CODE__HMI);
        mAllowArh   = mAllowMask.contains(ALLOW_CODE__ARH);
        mAllowRand  = mAllowMask.contains(ALLOW_CODE__RAND);
        mRandMin    = static_cast<quint16>(DataIn.value(FIELD__RAND_MIN).toInt(0));
        mRandMax    = static_cast<quint16>(DataIn.value(FIELD__RAND_MAX).toInt(0));

        QJsonArray Targets = DataIn.value(FIELD__TARGETS).toArray();

        this->normilize();

        if(mAllowRead || mAllowWrite || mAllowHmi || mAllowArh || mAllowRand)
        {
            if(!mFileEvents.isEmpty()) this->readFileEvents(mFileEvents);
            if(!Targets.isEmpty()) this->parseDataTargets(Targets);
        }
    }

    return (this->isCorrect());
}


/**
@brief  Pack data to String buffer.
@param  StringIn - link to string buffer.
@return None.
*/
void Register::toString(QString &StringIn)
{
    QString Correct    = QString(" (is correct)");
    QString NotCorrect = QString(" (is not correct)");

    StringIn+= QString(" - ");
    StringIn+= FIELD__DEV_ID;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mDevID);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__DEV_CLASS;
    StringIn+= QString(" = ");
    StringIn+= mDevClass;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ID;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mID);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ADDR;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mAddr);
    StringIn+= ((this->isCorrectAddr()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__CLASS;
    StringIn+= QString(" = ");
    StringIn+= mClass;
    StringIn+= ((this->isCorrectClass()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__TYPE;
    StringIn+= QString(" = ");
    StringIn+= mType;
    StringIn+= ((this->isCorrectType()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__OFFSET;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mOffset);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ROUND;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mRound);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__VAR;
    StringIn+= QString(" = ");
    StringIn+= mVar;
    StringIn+= ((this->isCorrectVar()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ALLOW;
    StringIn+= QString(" = ");
    StringIn+= mAllowMask;
    StringIn+= QString("\r\n");

    StringIn+= QString(" -- ");
    StringIn+= QString(ALLOW_CODE__READ);
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllowRead) ? ALLOW : DENY));
    StringIn+= QString("\r\n");

    StringIn+= QString(" -- ");
    StringIn+= QString(ALLOW_CODE__WRITE);
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllowWrite) ? ALLOW : DENY));
    StringIn+= QString("\r\n");

    StringIn+= QString(" -- ");
    StringIn+= QString(ALLOW_CODE__HMI);
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllowHmi) ? ALLOW : DENY));
    StringIn+= QString("\r\n");

    StringIn+= QString(" -- ");
    StringIn+= QString(ALLOW_CODE__ARH);
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllowArh) ? ALLOW : DENY));
    StringIn+= QString("\r\n");

    StringIn+= QString(" -- ");
    StringIn+= QString(ALLOW_CODE__RAND);
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllowRand) ? ALLOW : DENY));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__RAND_MIN;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mRandMin);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__RAND_MAX;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mRandMax);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__VALUE;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mValue);
    StringIn+= QString("\r\n");

    QString Buff;
    packStampISO(mStamp, Buff);
    StringIn+= QString(" - ");
    StringIn+= FIELD__STAMP;
    StringIn+= QString(" = ");
    StringIn+= Buff;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__EX;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mExLast);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ERR;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mErrLast);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__SIGN;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mSignLast);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ALG;
    StringIn+= QString(" = ");
    StringIn+= mAlg;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__EVENTS;
    StringIn+= QString(" = ");
    StringIn+= mFileEvents;
    StringIn+= QString("\r\n");

    StringIn+= QString(" ListEvents.size() = %1").arg(QString::number(mListEvents.size()));
    StringIn+= QString("\r\n");

    int i = 0;

    Event *Ev = nullptr;
    for(i=0; i<mListEvents.size(); i++)
    {
        Ev = mListEvents.at(i);

        if(Ev)
        {
            StringIn+= QString(" event[%1]").arg(QString::number(i));
            StringIn+= QString("\r\n");
            Ev->toString(StringIn);
        }
    }

    if(mClass == CLASS__CALC)
    {
        StringIn+= QString(" ListTargetsToCalc.size() = %1").arg(QString::number(mListTargetsToCalc.size()));
        StringIn+= QString("\r\n");

        for(i=0; i<mListTargetsToCalc.size(); i++)
        {
            StringIn+= QString(" target[%1] = %2\r\n").arg(QString::number(i), QString::number(mListTargetsToCalc.at(i)));
        }
    }
}


/**
@brief  Pack data to JSON buffer.
@param  ObjIn - link to JsonObject.
@return None.
@detailed { ..., Register.mVar:FormattedValue }
*/
void Register::toJson(QJsonObject &ObjIn)
{
    if(mAllowHmi && !mVar.isEmpty())
    {
        if(ObjIn.contains(mVar)) ObjIn.remove(mVar);

        QJsonValue Val;
        this->packFormattedValue(Val);
        ObjIn.insert(mVar, Val);
    }
}


/**
@brief (static) Pack value of the register into String-buffer in SQL-format (only value).
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
bool Register::toSqlValue(const QDateTime &StampIn, const QString &ProfileIn, const quint16 DevIdIn, const quint16 RegIdIn, const QString ValueIn, const int ExIn, const int ErrIn, const int SignIn, QString &StringIn)
{
    QString Profile = ((!ProfileIn.isEmpty()) ? QString("'%1'").arg(ProfileIn) : QString("NULL"));
    QString DevID   = ((DevIdIn) ? QString("%1").arg(QString::number(DevIdIn)) : QString("NULL"));
    QString RegID   = ((RegIdIn) ? QString("%1").arg(QString::number(RegIdIn)) : QString("NULL"));
    QString Value   = ((!ValueIn.isEmpty()) ? QString("%1").arg(ValueIn) : QString("NULL"));

    QString Stamp;
    packStampISO(StampIn, Stamp);
#ifdef SQL_PROC_TRM
    (void)SignIn;
    //(`stamp`,`profile`,`channel_id`,`value`,`exception`,`errcode`
    StringIn+= QString("('%1',%2,%3,%4,%5,%6").arg(Stamp, Profile, RegID, Value, QString::number(ExIn), QString::number(ErrIn));
#else
    //(`stamp`,`profile`,`device_id`,`register_id`,`value`,`ex`,`err`,`sign`)
    StringIn+= QString("('%1',%2,%3,%4,%5,%6,%7,%8)").arg(Stamp, Profile, DevID, RegID, Value, QString::number(ExIn), QString::number(ErrIn), QString::number(SignIn));
#endif
    return (true);
}


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
    for mAllowArh = true
*/
bool Register::toSqlCurrentValue(const QString &ProfileIn, QString &StringIn)
{
    if(mAllowArh)
    {
        QString Value;
        this->packFormattedValue(Value);
        toSqlValue(mStamp, ProfileIn, mDevID, mID, Value, mExLast, mErrLast, mSignLast, StringIn);
#ifdef SQL_PROC_TRM
        //,`obj_status`,`prog`,`step`)
        QString Status = ((mVar == VAR__TRM_OBJ && mListValuesToCalc.size() >= 3) ? QString::number(*mListValuesToCalc.at(0)) : QString("NULL"));
        QString Prog   = ((mVar == VAR__TRM_OBJ && mListValuesToCalc.size() >= 3) ? QString::number(*mListValuesToCalc.at(1)) : QString("NULL"));
        QString Step   = ((mVar == VAR__TRM_OBJ && mListValuesToCalc.size() >= 3) ? QString::number(*mListValuesToCalc.at(2)) : QString("NULL"));
        StringIn+= QString(",%1,%2,%3)").arg(Status, Prog, Step);
#endif
        return (true);
    }

    return (false);
}


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
int Register::toSqlCurrentValue(const QString &ProfileIn, QList<QString> &ListStringsIn)
{
    int Res = 0;

    QString Str("");
    this->toSqlCurrentValue(ProfileIn, Str);

    if(!Str.isEmpty())
    {
        ListStringsIn.append(Str);
        Res++;
    }

    return (Res);
}


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
int Register::toSqlEventValue(const QString &ProfileIn, QList<QString> &ListStringsIn)
{
    int Res = 0;

    if(mAllowArh)
    {
        int EvSize = mListEvents.size();

        if(EvSize)
        {
            Event *Ev = nullptr;
            QString Str, Value;

            for(int i=0; i<EvSize; i++)
            {
                Ev = mListEvents.at(i);

                if(Ev)
                {
                    if(Ev->isReady())
                    {
                        Value.clear();
                        packFormattedValue(Ev->mValue, mType, mOffset, mRound, Value);

                        Str = QString("");
                        toSqlValue(Ev->getStamp(), ProfileIn, mDevID, mID, Value, Ev->mEx, Ev->mErr, Ev->mSign, Str);
#ifdef SQL_PROC_TRM
                        //,`obj_status`,`prog`,`step`)
                        QString Status = ((mVar == VAR__TRM_OBJ && mListValuesToCalc.size() >= 3) ? QString::number(*mListValuesToCalc.at(0)) : QString("NULL"));
                        QString Prog   = ((mVar == VAR__TRM_OBJ && mListValuesToCalc.size() >= 3) ? QString::number(*mListValuesToCalc.at(1)) : QString("NULL"));
                        QString Step   = ((mVar == VAR__TRM_OBJ && mListValuesToCalc.size() >= 3) ? QString::number(*mListValuesToCalc.at(2)) : QString("NULL"));
                        Str+= QString(",%1,%2,%3)").arg(Status, Prog, Step);
#endif
                        if(!Str.isEmpty())
                        {
                            ListStringsIn.append(Str);
                            Res++;
                        }
                    }
                }
            }
        }
    }

    return (Res);
}


/**
@brief (static) Unpack Raw-value (uint16) into Int.
@param  RawValueIn - raw-value.
@return Unpacked value of type Int.
@detailed Will be unpack sign; offset as is.
*/
int Register::toInt(const quint16 RawValueIn)
{
    return ((RawValueIn > WORD_UNSIGN_MAX) ? (static_cast<int>(RawValueIn) - WORD_SIGN) : static_cast<int>(RawValueIn));
}


/**
@brief  Decode Offset.
@param  In - Float (coded by offset);
@param  OffsetIn - offset;
@return Decoded Float.
@detailed Result = CodedIn/OffsetIn.
*/
float Register::decodeOffset(float In, qint8 OffsetIn)
{
    qint8 Offset = ((OffsetIn >= OFFSET_MIN) ? OffsetIn : 1);
    return (In/(static_cast<float>(Offset)));
}

/**
@brief  Decode Offset.
@param  In - Int (coded by offset);
@param  OffsetIn - offset;
@return Decoded Float.
@detailed Result = CodedIn/OffsetIn.
*/
float Register::decodeOffset(int In, qint8 OffsetIn)
{
    return (decodeOffset((static_cast<float>(In)), OffsetIn));
}

/**
@brief  Code Offset.
@param  In - Float;
@param  OffsetIn - offset;
@return Coded Float.
@detailed Result = In*OffsetIn.
*/
float Register::codeOffset(float In, qint8 OffsetIn)
{
    qint8 Offset = ((OffsetIn >= OFFSET_MIN) ? OffsetIn : 1);
    return (In*(static_cast<float>(Offset)));
}

/**
@brief  Round.
@param  In - Float;
@param  RoundIn - round;
@return Rounded Float.
@detailed Result = round(In, RoundIn).
*/
float Register::setRound(float In, qint8 RoundIn)
{
    qint8 Round = RoundIn;
    if(Round < ROUND_MIN) Round = ROUND_MIN;
    if(Round > ROUND_MAX) Round = ROUND_MAX;
    return (QString::number(static_cast<double>(In), 'f', Round).toFloat());
}


/**
@brief (static) Unpack Raw-value (uint16) into Float.
@param  RawValueIn - raw-value;
@param  OffsetIn - offset;
@param  RoundIn - round.
@return Unpacked value of type Float.
@detailed Will be unpack sign and offset, set round.
*/
float Register::toFloat(const quint16 RawValueIn, const qint8 OffsetIn, const qint8 RoundIn)
{
/*
    qint8 Offset = ((OffsetIn >= OFFSET_MIN) ? OffsetIn : 1);
    int SignInt  = toInt(RawValueIn);
    double Value = ((static_cast<double>(SignInt))/(static_cast<double>(Offset)));

    qint8 Round = RoundIn;
    if(Round < ROUND_MIN) Round = ROUND_MIN;
    if(Round > ROUND_MAX) Round = ROUND_MAX;

    return (QString::number(static_cast<double>(Value), 'f', Round).toFloat());
*/
    int SignInt   = toInt(RawValueIn);
    float Decoded = decodeOffset(SignInt, OffsetIn);
    return (setRound(Decoded, RoundIn));
}


/**
@brief (static) Unpack Raw-value (uint16) into Boolean.
@param  RawValueIn - raw-value.
@return Unpacked value of type Boolean.
*/
bool Register::toBool(const quint16 RawValueIn)
{
    return ((RawValueIn > 0) ? true : false);
}


/**
@brief  Merge two Raw-values (uint16) from a list into DWord.
@param  ListRawValuesIn - list of raw-values.
@return Merged DWord.
*/
quint32 Register::mergeDword(const QList<quint16 *> *ListRawValuesIn)
{
    DWORD_uwt Buff;
    Buff.data = 0;

    if(ListRawValuesIn)
    {
        quint16 *Word0 = ((ListRawValuesIn->size() > 0) ? ListRawValuesIn->at(0) : nullptr);
        quint16 *Word1 = ((ListRawValuesIn->size() > 1) ? ListRawValuesIn->at(1) : nullptr);
        Buff.words[0]  = ((Word0 != nullptr) ? *Word0 : 0);
        Buff.words[1]  = ((Word1 != nullptr) ? *Word1 : 0);
    }

    return (Buff.data);
}


/**
@brief  Merge two Raw-values (uint16) from a list into Float.
@param  ListRawValuesIn - list of raw-values.
@return Merged Float.
*/
float Register::mergeFloat(const QList<quint16 *> *ListRawValuesIn)
{
    FLOAT_uwt Buff;
    Buff.data = 0.0;

    if(ListRawValuesIn)
    {
        quint16 *Word0 = ((ListRawValuesIn->size() > 0) ? ListRawValuesIn->at(0) : nullptr);
        quint16 *Word1 = ((ListRawValuesIn->size() > 1) ? ListRawValuesIn->at(1) : nullptr);
        Buff.words[0]  = ((Word0 != nullptr) ? *Word0 : 0);
        Buff.words[1]  = ((Word1 != nullptr) ? *Word1 : 0);
    }

    return (Buff.data);
}


/**
@brief  Merge bits into DWord.
@param  ListRawValuesIn - list of raw-values.
@return Merged DWord.
*/
quint32 Register::mergeBitsToDword(const QList<quint16 *> *ListRawValuesIn)
{
    quint32 Res = 0;

    if(ListRawValuesIn)
    {
        quint16 *Word = nullptr;
        int Sz = ListRawValuesIn->size();
        if(Sz > 32) Sz = 32;

        for(int i=0; i<Sz; i++)
        {
            Word = ListRawValuesIn->at(i);
            if(Word != nullptr)
            {
                Res = BYTE_SET_BIT(Res, *Word, i);
            }
        }
    }

    return (Res);
}


/**
@brief  Check option "Addr".
@param  None.
@return True if value of option "Addr" is correct, otherwise - False.
*/
bool Register::isCorrectAddr()
{
    return (true);
}


/**
@brief  Check option "Class".
@param  None.
@return True if value of option "Class" is correct, otherwise - False.
*/
bool Register::isCorrectClass()
{
    return ((mClass == CLASS__CONST || mClass == CLASS__RANDOM || mClass == CLASS__CALC || mClass == CLASS__COIL  || mClass == CLASS__DISC || mClass == CLASS__IN || mClass == CLASS__INPT || mClass == CLASS__HOLDING || mClass == CLASS__6) ? true : false);
}


/**
@brief  Check option "Type".
@param  None.
@return True if value of option "Type" is correct, otherwise - False.
*/
bool Register::isCorrectType()
{
    return ((mType == TYPE__BOOL || mType == TYPE__WORD || mType == TYPE__INT || mType == TYPE__FLOAT || mType == TYPE__RAW) ? true : false);
}


/**
@brief  Check option "Var".
@param  None.
@return True if value of option "Var" is correct, otherwise - False.
*/
bool Register::isCorrectVar()
{
    if(mVar.isEmpty())
    {
        if(!mAllowHmi) return (true);
    }
    else
    {
        return (true);
    }

    return (false);
}


/**
@brief  Check options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Register::isCorrect()
{
    return ((this->isCorrectAddr() && this->isCorrectClass() && this->isCorrectType() && this->isCorrectVar()) ? true : false);
}


/**
@brief  Check readness of events.
@param  None.
@return True if one from events is ready, otherwise - False.
*/
bool Register::isEventsReady()
{
    int EvSize = mListEvents.size();

    if(EvSize)
    {
        Event *Ev = nullptr;
        for(int i=0; i<EvSize; i++)
        {
            Ev = mListEvents.at(i);
            if(Ev)
            {
                if(Ev->isReady()) return (true);
            }
        }
    }

    return (false);
}


/**
@brief  Normilize options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Register::normilize()
{
    if(mOffset < OFFSET_MIN) mOffset = OFFSET_MIN;
    if(mRound < ROUND_MIN)   mRound  = ROUND_MIN;
    if(mRound > ROUND_MAX)   mRound  = ROUND_MAX;
    if(!this->isCorrectType()) mType = TYPE__RAW;

    return (this->isCorrect());
}


/**
@brief  Read list of event settings from a file.
@param  FileIn - path to a file.
@return The number of event settings have read.
*/
quint16 Register::readFileEvents(const QString &FileIn)
{
    Log::log(QString("Register::readFileEvents(%1)").arg(FileIn), mFileLog, mUseLog);

    QString Err;
    QJsonDocument Doc = Json::readFile(FileIn, Err);
    if(!Err.isEmpty()) Log::log(Err, mFileLog, mUseLog, false);

    this->parseDataEvents(Doc);

    return (this->sizeListEvents());
}


/**
@brief  Parse data of list event settings.
@param  DocIn - link to JsonDocument.
@return The number of event settings have parsed.
*/
quint16 Register::parseDataEvents(const QJsonDocument &DocIn)
{
    if(DocIn.isArray())
    {
        QJsonArray Arr = DocIn.array();
        QJsonObject Obj;
        Event *Ev = nullptr;

        for(int i=0; i<Arr.size(); i++)
        {
            Obj = Arr.at(i).toObject();

            if(!Obj.isEmpty())
            {
                Ev = new Event();
                Ev->fromJson(Obj);

                mListEvents.append(Ev);
            }
        }
    }

    return (this->sizeListEvents());
}


/**
@brief  Parse data for list of ID of target registers.
@param  ArrIn - link to JsonArray.
@return The number of targets have parsed.
*/
quint16 Register::parseDataTargets(const QJsonArray &ArrIn)
{
    if(!ArrIn.isEmpty())
    {
        quint16 ID = 0;

        for(int i=0; i<ArrIn.size(); i++)
        {
            ID = static_cast<quint16>(ArrIn.at(i).toInt(0));
            if(ID) mListTargetsToCalc.append(ID);
        }
    }

    return (static_cast<quint16>(mListTargetsToCalc.size()));
}


/**
@brief  Clear list of event settings.
@param  None.
@return None.
*/
void Register::clearListEvents()
{
    if(!mListEvents.isEmpty())
    {
        Event *Ev = nullptr;

        while(mListEvents.size())
        {
            Ev = mListEvents.takeLast();
            if(Ev) delete Ev;
        }
    }
}


/**
@brief  Get size of list of event settings.
@param  None.
@return Size of list of event settings.
*/
quint16 Register::sizeListEvents()
{
    return (static_cast<quint16>(mListEvents.size()));
}


/**
@brief  Check size of list of event settings.
@param  None.
@return True if list of event settings is empty, otherwise - False.
*/
bool Register::isListEventsEmpty()
{
    return ((this->sizeListEvents() == 0) ? true : false);
}


/**
@brief  Set value.
@param  ValueIn - value.
@return Packed value.
*/
quint16 Register::setValue(float ValueIn)
{
    mValue = 0;

    if(this->normilize())
    {
        float Value = QString::number(static_cast<double>(ValueIn), 'f', mRound).toFloat();
        mValue      = static_cast<quint16>(Value*static_cast<float>(mOffset));
    }

    return (mValue);
}


/**
@brief  Refresh stamp.
@param  None.
@return New stamp.
*/
QDateTime Register::refreshStamp()
{
    mStamp = QDateTime::currentDateTime();
    return (mStamp);
}


/**
@brief  Get value of stamp.
@param  None.
@return Value of stamp.
*/
QDateTime Register::getStamp()
{
    return (mStamp);
}


/**
@brief (static) Pack value of stamp into string buffer in ISO-format.
@param  StampIn - stamp;
@param  BuffIn - link to string buffer.
@return None.
*/
void Register::packStampISO(const QDateTime &StampIn, QString &BuffIn)
{
    BuffIn = StampIn.toString(QString("yyyy-MM-dd hh:mm:ss"));
}


/**
@brief  Pack value of stamp into string buffer in ISO-format.
@param  BuffIn - link to string buffer.
@return None.
*/
void Register::packStampISO(QString &BuffIn)
{
    packStampISO(mStamp, BuffIn);
}


/**
@brief (static) Pack formatted value into QJsonValue.
@param  RawValueIn - raw-value;
@param  TypeIn - output data type;
@param  OffsetIn - offset;
@param  RoundIn - round;
@param  BuffIn - link to buffer.
@return None.
*/
void Register::packFormattedValue(const quint16 RawValueIn, const QString TypeIn, const qint8 OffsetIn, const qint8 RoundIn, QJsonValue &BuffIn)
{
    if(TypeIn == TYPE__BOOL)
    {
        BuffIn = QJsonValue(toBool(RawValueIn));
    }
    else if(TypeIn == TYPE__FLOAT)
    {
        BuffIn = QJsonValue(static_cast<double>(toFloat(RawValueIn, OffsetIn, RoundIn)));
    }
    else if(TypeIn == TYPE__INT)
    {
        BuffIn = QJsonValue(toInt(RawValueIn));
    }
    else
    {
        BuffIn = QJsonValue(RawValueIn);
    }
}


/**
@brief  Pack calculated value into QJsonValue.
@param  ListRawValuesIn - list of raw-values;
@param  AlgIn - calculation algorithm;
@param  RoundIn - round (for float);
@param  BuffIn - link to buffer.
@return None.
*/
void Register::packCalculatedValue(const QList<quint16 *> ListRawValuesIn, const QString AlgIn, const qint8 RoundIn, QJsonValue &BuffIn)
{
    QJsonValue Res = QJsonValue(0);

    if(AlgIn == CALC_ALG__MERGE32 || AlgIn == CALC_ALG__DWORD)
    {
        (void)RoundIn;
        Res = QJsonValue(static_cast<qint64>(mergeDword(&ListRawValuesIn)));
    }
    else if(AlgIn == CALC_ALG__FLOAT)
    {
        Res = QJsonValue(static_cast<double>(setRound(mergeFloat(&ListRawValuesIn), RoundIn)));
    }
    else if(AlgIn == CALC_ALG__BITSTODWORD)
    {
        (void)RoundIn;
        Res = QJsonValue(static_cast<qint64>(mergeBitsToDword(&ListRawValuesIn)));
    }

    BuffIn = Res;
}


/**
@brief  Pack formatted value into QJsonValue.
@param  BuffIn - link to buffer.
@return None.
*/
void Register::packFormattedValue(QJsonValue &BuffIn)
{
    if(mClass == CLASS__CALC)
    {
        packCalculatedValue(mListValuesToCalc, mAlg, mRound, BuffIn);
        this->refreshStamp();
    }
    else
    {
        packFormattedValue(mValue, mType, mOffset, mRound, BuffIn);
    }
}


/**
@brief (static) Pack formatted value into QString.
@param  RawValueIn - raw-value;
@param  TypeIn - output data type
@param  OffsetIn - offset;
@param  RoundIn - round;
@param  BuffIn - link to buffer.
@return None.
*/
void Register::packFormattedValue(const quint16 RawValueIn, const QString TypeIn, const qint8 OffsetIn, const qint8 RoundIn, QString &BuffIn)
{
    if(TypeIn == TYPE__BOOL)
    {
        BuffIn = ((toBool(RawValueIn)) ? QString("true") : QString("false"));
    }
    else if(TypeIn == TYPE__FLOAT)
    {
        qint8 Round = RoundIn;
        if(Round < ROUND_MIN) Round = ROUND_MIN;
        if(Round > ROUND_MAX) Round = ROUND_MAX;
        BuffIn = QString::number(static_cast<double>(toFloat(RawValueIn, OffsetIn, Round)), 'f', Round);
    }
    else if(TypeIn == TYPE__INT)
    {
        BuffIn = QString::number(toInt(RawValueIn));
    }
    else
    {
        BuffIn = QString::number(RawValueIn);
    }
}


/**
@brief  Pack calculated value into QString.
@param  ListRawValuesIn - list of raw-values;
@param  AlgIn - calculation algorithm;
@param  RoundIn - round (for float);
@param  BuffIn - link to buffer.
@return None.
*/
void Register::packCalculatedValue(const QList<quint16 *> ListRawValuesIn, const QString AlgIn, const qint8 RoundIn, QString &BuffIn)
{
    QJsonValue Value;
    packCalculatedValue(ListRawValuesIn, AlgIn, RoundIn, Value);

    if(Value.isDouble())
    {
        BuffIn = QString::number(Value.toDouble(0), 'f', RoundIn);
    }
    else
    {
        BuffIn = QString::number(Value.toInt(0));
    }
}


/**
@brief  Pack formatted value into QString.
@param  BuffIn - link to buffer.
@return None.
*/
void Register::packFormattedValue(QString &BuffIn)
{
    if(mClass == CLASS__CALC)
    {
        packCalculatedValue(mListValuesToCalc, mAlg, mRound, BuffIn);
        this->refreshStamp();
    }
    else
    {
        packFormattedValue(mValue, mType, mOffset, mRound, BuffIn);
    }
}


/**
@brief  Get randomized value.
@param  None.
@return Randomized value.
*/
int Register::randomize()
{
    if(mAllowRand) mValue = (qrand() % ((mRandMax + 1) - mRandMin) + mRandMin);
    return (mValue);
}
