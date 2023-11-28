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

#include "event.h"

/**
@brief Fields
*/
const QString Event::FIELD__ALG         = "Alg";
const QString Event::FIELD__MIN         = "Min";
const QString Event::FIELD__MAX         = "Max";
const QString Event::FIELD__REF         = "Ref";
const QString Event::FIELD__SIGN        = "Sign";
const QString Event::FIELD__EX          = "Ex";
const QString Event::FIELD__ERR         = "Err";
const QString Event::FIELD__STAMP       = "Stamp";
const QString Event::FIELD__STAMP_CONF  = "StampConfirmed";

/**
@brief Algorithms
*/
const QString Event::ALG__OUTLIM        = "OutLim";
const QString Event::ALG__REF_EQ        = "=Ref";
const QString Event::ALG__REF_GREATER   = ">Ref";
const QString Event::ALG__REF_LESS      = "<Ref";


/**
@brief  Constructor.
@param  None.
@return None.
*/
Event::Event(QObject *parent) : QObject(parent)
{
    this->byDefault();
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Event::~Event()
{

}


/**
@brief  Set values by default.
@param  None.
@return None.
*/
void Event::byDefault()
{
    mAlg  = QString("");
    mMin  = 0;
    mMax  = 0;
    mRef  = 0;
    mSign = 0;
    mEx   = -1;
    mErr  = 0;

    this->reset();
}


/**
@brief  Load values from JsonObject.
@param  DataIn - data.
@return True if value of options are correct, otherwise - False.
*/
bool Event::fromJson(const QJsonObject &DataIn)
{
    this->byDefault();

    if(!DataIn.isEmpty())
    {
        mAlg  = DataIn.value(FIELD__ALG).toString(QString(""));
        mMin  = DataIn.value(FIELD__MIN).toInt(0);
        mMax  = DataIn.value(FIELD__MAX).toInt(0);
        mRef  = DataIn.value(FIELD__REF).toInt(0);
        mSign = static_cast<qint16>(DataIn.value(FIELD__SIGN).toInt(0));
        mEx   = static_cast<qint16>(DataIn.value(FIELD__EX).toInt(0));
        mErr  = static_cast<qint16>(DataIn.value(FIELD__ERR).toInt(0));
    }

    return (this->isCorrect());
}


/**
@brief  Pack data to String buffer.
@param  StringIn - link to string buffer.
@return None.
*/
void Event::toString(QString &StringIn)
{
    QString Correct    = QString(" (is correct)");
    QString NotCorrect = QString(" (is not correct)");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ALG;
    StringIn+= QString(" = ");
    StringIn+= mAlg;
    StringIn+= ((this->isCorrectAlg()) ? Correct : NotCorrect);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__MIN;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mMin);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__MAX;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mMax);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__REF;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mRef);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__STAMP;
    StringIn+= QString(" = ");
    StringIn+= mStamp.toString(QString("yyyy-MM-dd hh:mm:ss"));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__STAMP_CONF;
    StringIn+= QString(" = ");
    StringIn+= mStampConfirmed.toString(QString("yyyy-MM-dd hh:mm:ss"));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__EX;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mEx);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__ERR;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mErr);
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= FIELD__SIGN;
    StringIn+= QString(" = ");
    StringIn+= QString::number(mSign);
    StringIn+= QString("\r\n");

    StringIn+= QString(" * the event %1").arg(((this->isReady()) ? QString("is ready") : QString("is not ready")));
    StringIn+= QString("\r\n");
}


/**
@brief  Check option "Alg".
@param  None.
@return True if value of option "Alg" is correct, otherwise - False.
*/
bool Event::isCorrectAlg()
{
    return ((mAlg == ALG__OUTLIM || mAlg == ALG__REF_EQ || mAlg == ALG__REF_GREATER || mAlg == ALG__REF_LESS) ? true : false);
}


/**
@brief  Check options.
@param  None.
@return True if value of options are correct, otherwise - False.
*/
bool Event::isCorrect()
{
    return ((this->isCorrectAlg()) ? true : false);
}


/**
@brief  Refresh stamp.
@param  None.
@return New stamp.
*/
QDateTime Event::refreshStamp()
{
    mStamp = QDateTime::currentDateTime();
    return (mStamp);
}


/**
@brief  Get value of stamp.
@param  None.
@return Value of stamp.
*/
QDateTime Event::getStamp()
{
    return (mStamp);
}


/**
@brief  Reset event.
@param  None.
@return None.
@details
*/
void Event::reset()
{
    this->refreshStamp();
    this->confirm();
}


/**
@brief  Confirm event.
@param  None.
@return None.
*/
void Event::confirm()
{
    mStampConfirmed = mStamp;
}


/**
@brief  Check readness of event.
@param  None.
@return True if event is ready, otherwise - False.
@details If mStamp != mStampConfirmed
*/
bool Event::isReady()
{
    return (mStamp != mStampConfirmed);
}


/**
@brief  Check confirmation of event.
@param  None.
@return True if event is confirmed, otherwise - False.
@details If mStamp == mStampConfirmed
*/
bool Event::isConfirmed()
{
    return (!this->isReady());
}
