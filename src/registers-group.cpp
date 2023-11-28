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

#include "registers-group.h"

/**
@brief  Constructor.
@param  None.
@return None.
*/
RegsGroup::RegsGroup(QObject *parent) : QObject(parent)
{
    mFileLog   = QString("");
    mUseLog    = false;
    mClass     = QString("");
    mAllowRead = false;
}


/**
@brief  Destructor.
@param  None.
@return None.
*/
RegsGroup::~RegsGroup()
{
    this->clear();
}


/**
@brief  Pack data to String buffer.
@param  StringIn - link to string buffer.
@return None.
*/
void RegsGroup::toString(QString &StringIn)
{
    StringIn+= QString("[ GROUP OF REGISTERS ]\r\n");

    StringIn+= QString(" - ");
    StringIn+= Register::FIELD__CLASS;
    StringIn+= QString(" = ");
    StringIn+= mClass;
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= QString(" - Allow to read");
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllowRead) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= QString(" - Allow to write");
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllowWrite) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - ");
    StringIn+= QString(" - Allow to HMI");
    StringIn+= QString(" = ");
    StringIn+= QString::number(((mAllowHmi) ? 1 : 0));
    StringIn+= QString("\r\n");

    StringIn+= QString(" - FirstAddr");
    StringIn+= QString(" = ");
    StringIn+= QString::number(this->getFirstAddr());
    StringIn+= QString("\r\n");

    StringIn+= QString(" - LastAddr");
    StringIn+= QString(" = ");
    StringIn+= QString::number(this->getLastAddr());
    StringIn+= QString("\r\n");

    StringIn+= QString(" ListRegisters.size() = %1").arg(QString::number(mListRegisters.size()));
    StringIn+= QString("\r\n");

    Register *Reg = nullptr;
    for(int i=0; i<mListRegisters.size(); i++)
    {
        Reg = mListRegisters.at(i);

        if(Reg)
        {
            StringIn+= QString(" register[%1]").arg(QString::number(i));
            StringIn+= QString("\r\n");
            Reg->toString(StringIn);
        }
    }
}


/**
@brief  Pack data to JSON buffer.
@param  ObjIn - link to JsonObject.
@return None.
@detailed { ..., Register.mVar:Register.mValue }
*/
void RegsGroup::toJson(QJsonObject &ObjIn)
{
    Register *Reg = nullptr;

    for(int i=0; i<mListRegisters.size(); i++)
    {
        Reg = mListRegisters.at(i);

        if(Reg)
        {
            Reg->toJson(ObjIn);
        }
    }
}


/**
@brief  Pack registers into String-buffer in SQL-format (only values).
@param  ProfileIn - name of profile;
@param  EventsIn - true for event values, false for current values;
@param  ListStringsIn - link to list of string buffers.
@return The number of packed values.
@detailed
    Output SQL:
            [0] = ('{Register[0].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[0].mID}, {Register[0].FormattedValue}, {Register[0].mExLast}, {Register[0].mErrLast}, {Register[0].mSignLast}),
            [1] = ('{Register[1].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[1].mID}, {Register[1].FormattedValue}, {Register[1].mExLast}, {Register[1].mErrLast}, {Register[1].mSignLast}),
             ...
            [n] = ('{Register[n].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[n].mID}, {Register[n].FormattedValue}, {Register[n].mExLast}, {Register[n].mErrLast}, {Register[n].mSignLast})
*/
int RegsGroup::toSqlValue(const QString &ProfileIn, bool EventsIn, QList<QString> &ListStringsIn)
{
    int Res = 0;
    Register *Reg = nullptr;

    for(int i=0; i<mListRegisters.size(); i++)
    {
        Reg = mListRegisters.at(i);
        if(Reg) Res+= ((EventsIn == true) ? Reg->toSqlEventValue(ProfileIn, ListStringsIn) : Reg->toSqlCurrentValue(ProfileIn, ListStringsIn));
    }

    return (Res);
}


/**
@brief  Pack current value of registers into String-buffer in SQL-format (only values).
@param  ProfileIn - name of profile;
@param  ListStringsIn - link to list of string buffers.
@return The number of packed values.
@detailed
    Output SQL:
            [0] = ('{Register[0].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[0].mID}, {Register[0].FormattedValue}, {Register[0].mExLast}, {Register[0].mErrLast}, {Register[0].mSignLast}),
            [1] = ('{Register[1].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[1].mID}, {Register[1].FormattedValue}, {Register[1].mExLast}, {Register[1].mErrLast}, {Register[1].mSignLast}),
             ...
            [n] = ('{Register[n].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[n].mID}, {Register[n].FormattedValue}, {Register[n].mExLast}, {Register[n].mErrLast}, {Register[n].mSignLast})
*/
int RegsGroup::toSqlCurrentValue(const QString &ProfileIn, QList<QString> &ListStringsIn)
{
    return (this->toSqlValue(ProfileIn, false, ListStringsIn));
}


/**
@brief  Pack event value of registers into String-buffer in SQL-format (only values).
@param  ProfileIn - name of profile;
@param  ListStringsIn - link to list of string buffers.
@return The number of packed values.
@detailed
    Output SQL:
            [0] = ('{Register[0].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[0].mID}, {Register[0].Event.FormattedValue}, {Register[0].Event.mEx}, {Register[0].Event.mErrLast}, {Register[0].Event.mSign}),
            [0] = ('{Register[1].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[1].mID}, {Register[1].Event.FormattedValue}, {Register[1].Event.mEx}, {Register[1].Event.mErrLast}, {Register[1].Event.mSign}),
             ...
            [n] = ('{Register[n].mStamp in ISO-format}', '{ProfileIn}', {mID}, {Register[n].mID}, {Register[n].Event.FormattedValue}, {Register[n].Event.mEx}, {Register[n].Event.mErr}, {Register[n].Event.mSign})
*/
int RegsGroup::toSqlEventValue(const QString &ProfileIn, QList<QString> &ListStringsIn)
{
    return (this->toSqlValue(ProfileIn, true, ListStringsIn));
}


/**
@brief  Check class of the group.
@param  ClassIn - class of a register.
@return True if a class is matched, otherwise - False.
*/
bool RegsGroup::isClass(QString ClassIn)
{
    return ((mClass == ClassIn) ? true : false);
}


/**
@brief  Check allow to read.
@param  None.
@return True if a group or registers to read, otherwise - False.
*/
bool RegsGroup::isAllowToRead()
{
    return ((mAllowRead == Register::ALLOW) ? true : false);
}


/**
@brief  Check allow to write.
@param  None.
@return True if a group or registers to write, otherwise - False.
*/
bool RegsGroup::isAllowToWrite()
{
    return ((mAllowWrite == Register::ALLOW) ? true : false);
}


/**
@brief  Check allow to HMI.
@param  None.
@return True if a group or registers to HMI, otherwise - False.
*/
bool RegsGroup::isAllowToHmi()
{
    return ((mAllowHmi == Register::ALLOW) ? true : false);
}


/**
@brief  Get class of the group.
@param  None.
@return Class of the group.
*/
QString RegsGroup::getClass()
{
    return (mClass);
}


/**
@brief  Get address of first register of the group.
@param  None.
@return Address of first register of the group.
*/
quint16 RegsGroup::getFirstAddr()
{
    if(!mListRegisters.isEmpty())
    {
        Register *Reg = mListRegisters.first();
        if(Reg) return (Reg->mAddr);
    }

    return (static_cast<quint16>(0));
}


/**
@brief  Get address of last register of the group.
@param  None.
@return Address of last register of the group.
*/
quint16 RegsGroup::getLastAddr()
{
    if(!mListRegisters.isEmpty())
    {
        Register *Reg = mListRegisters.last();
        if(Reg) return (Reg->mAddr);
    }

    return (static_cast<quint16>(0));
}


/**
@brief  Validating a register to add into the group.
@param  ClassIn     - class of a register;
@param  RegAddrIn   - address of a register;
@param AllowReadIn  - allow to read of a register.
@param AllowWriteIn - allow to write of a register.
@param AllowHmiIn   - allow to HMI of a register.
@return True if a register is valid (may be add into the group), otherwise - False.
@detailed Group by: Class AND Addr AND AllowRead AND AllowWrite AND AllowHmi
*/
bool RegsGroup::isValid(const QString ClassIn, const quint16 RegAddrIn, const bool AllowReadIn, const bool AllowWriteIn, const bool AllowHmiIn)
{
    if(mListRegisters.isEmpty()) return (true);

    if(this->isClass(ClassIn) && AllowReadIn == mAllowRead && AllowWriteIn == mAllowWrite && AllowHmiIn == mAllowHmi)
    {
        Register *LastReg = mListRegisters.last();

        if(LastReg)
        {
            if((LastReg->mAddr+1) == RegAddrIn) return (true);
        }
    }

    return (false);
}


/**
@brief  Validate a register to add into the group.
@param  RegIn - a register.
@return True if a register is valid (may be add into the group), otherwise - False.
@detailed Group by: Class AND Addr AND AllowRead
*/
bool RegsGroup::isValid(Register *RegIn)
{
    return ((RegIn) ? this->isValid(RegIn->mClass, RegIn->mAddr, RegIn->mAllowRead, RegIn->mAllowWrite, RegIn->mAllowHmi) : false);
}


/**
@brief  Add register into the group.
@param  RegIn - register.
@return True if a register had been adding, otherwise - False.
*/
bool RegsGroup::add(Register *RegIn)
{
    if(this->isValid(RegIn))
    {
        if(mListRegisters.isEmpty())
        {
            mClass      = RegIn->mClass;
            mAllowRead  = RegIn->mAllowRead;
            mAllowWrite = RegIn->mAllowWrite;
            mAllowHmi   = RegIn->mAllowHmi;
        }

        mListRegisters.append(RegIn);

        return (true);
    }

    return (false);
}


/**
@brief  Get register by the list iterator.
@param  IterNumIn - an iterator number (0 ... size()-1).
@return Register or nullptr.
*/
Register *RegsGroup::get(quint16 IterNumIn)
{
    return ((IterNumIn < this->size()) ? mListRegisters.at(IterNumIn) : nullptr);
}


/**
@brief  Get register by ID.
@param  IDIn - ID of register.
@return Register or nullptr.
*/
Register *RegsGroup::getByID(quint16 IDIn)
{
    Register *Reg = nullptr;
    int Len = mListRegisters.size();

    for(int i=0; i<Len; i++)
    {
        Reg = mListRegisters.at(i);
        if(Reg != nullptr)
        {
           if(Reg->mID == IDIn) return (Reg);
        }
    }

    return (nullptr);
}


/**
@brief  Get value of register by ID.
@param  IDIn - ID of register.
@return Value of register or nullptr.
*/
quint16 *RegsGroup::getValueByID(quint16 IDIn)
{
    Register *Reg = this->getByID(IDIn);
    return ((Reg != nullptr) ? &Reg->mValue : nullptr);
}


/**
@brief  Get registers by ID.
@param  ListIDsIn - ID of registers.
@return List of pointers to registers.
*/
QList<Register *> RegsGroup::getByID(const QList<quint16> &ListIDsIn)
{
    QList<Register *> ListRegs;

    if(!ListIDsIn.isEmpty())
    {
        Register *Reg = nullptr;
        for(int i=0; i<ListIDsIn.size(); i++)
        {
            Reg = this->getByID(ListIDsIn.at(i));
            if(Reg != nullptr) ListRegs.append(Reg);
        }
    }

    return (ListRegs);
}


/**
@brief  Get values of registers by ID.
@param  ListIDsIn - list of ID of registers.
@return Values of registers.
*/
QList<quint16 *> RegsGroup::getValuesByID(const QList<quint16> &ListIDsIn)
{
    QList<quint16 *> ListValues;
    QList<Register *> ListRegs = this->getByID(ListIDsIn);

    if(!ListRegs.isEmpty())
    {
        Register *Reg = nullptr;
        for(int i=0; i<ListRegs.size(); i++)
        {
            Reg = ListRegs.at(i);
            if(Reg != nullptr) ListValues.append(&Reg->mValue);
        }
    }

    return (ListValues);
}


/**
@brief  Get all registers of the group.
@param  None.
@return List of registers.
*/
QList<Register *> RegsGroup::getAll()
{
    return (mListRegisters);
}


/**
@brief  Get Raw-values of the group registers.
@param  ValuesIn - pointer to array for Raw-values;
@param  LenIn    - quantity of returned values (<= group.size).
@return The number of returned values.
*/
quint16 RegsGroup::getValues(quint8 *ValuesIn, quint16 LenIn)
{
    quint16 Num = 0;

    if(ValuesIn && LenIn > 0)
    {
        Register *Reg = nullptr;
        int Len       = ((LenIn <= mListRegisters.size()) ? LenIn : mListRegisters.size());

        for(int i=0; i<Len; i++)
        {
            Reg = mListRegisters.at(i);
            if(Reg)
            {
                ValuesIn[i] = static_cast<quint8>(Reg->mValue);
                Num++;
            }
        }
    }

    return (Num);
}

/**
@brief  Get Raw-values of the group registers.
@param  ValuesIn - pointer to array for Raw-values;
@return The number of returned values.
*/
quint16 RegsGroup::getValues(quint8 *ValuesIn)
{
    return (this->getValues(ValuesIn, mListRegisters.size()));
}

/**
@brief  Get Raw-values of registers.
@param  ValuesIn - pointer to array for Raw-values;
@param  LenIn    - quantity of returned values (<= group.size).
@return The number of returned values.
*/
quint16 RegsGroup::getValues(quint16 *ValuesIn, quint16 LenIn)
{
    quint16 Num = 0;

    if(ValuesIn && LenIn > 0)
    {
        Register *Reg = nullptr;
        int Len       = ((LenIn <= mListRegisters.size()) ? LenIn : mListRegisters.size());

        for(int i=0; i<Len; i++)
        {
            Reg = mListRegisters.at(i);
            if(Reg)
            {
                ValuesIn[i] = Reg->mValue;
                Num++;
            }
        }
    }

    return (Num);
}

/**
@brief  Get Raw-values of registers.
@param  ValuesIn - pointer to array for Raw-values;
@return The number of returned values.
*/
quint16 RegsGroup::getValues(quint16 *ValuesIn)
{
    return (this->getValues(ValuesIn, mListRegisters.size()));
}


/**
@brief  Set Raw-values to registers.
@param  ValuesIn - pointer to array of Raw-values;
@param  LenIn    - size of array of values.
@return The number of registers that had been changing.
*/
quint16 RegsGroup::setValues(quint16 *ValuesIn, quint16 LenIn)
{
    Log::log(QString("RegsGroup::setValues(Class=%1,FirstAddr=%2,Len=%3)").arg(mClass, QString::number(this->getFirstAddr()), QString::number(LenIn)), mFileLog, mUseLog);

    quint16 Num = 0;

    if(ValuesIn && LenIn > 0)
    {
        Register *Reg   = nullptr;
        int Len         = ((LenIn <= mListRegisters.size()) ? LenIn : mListRegisters.size());
        QString LogBuff = QString("");

        for(int i=0; i<Len; i++)
        {
            Reg = mListRegisters.at(i);

            if(Reg)
            {
                if(mUseLog) LogBuff+= ((i < (Len-1)) ? QString("%1 ").arg(QString::number(ValuesIn[i])) : QString("%1").arg(QString::number(ValuesIn[i])));
                Reg->mValue = ValuesIn[i];
                Reg->refreshStamp();
                Num++;
            }
        }

        Log::log(LogBuff, mFileLog, mUseLog, false);
    }

    return (Num);
}


/**
@brief  Set Raw-values to registers.
@param  ValuesIn - pointer to array of Raw-values;
@param  LenIn    - size of array of values.
@return The number of registers that had been changing.
*/
quint16 RegsGroup::setValues(const quint8 *ValuesIn, const quint16 LenIn)
{
    if(ValuesIn && LenIn > 0)
    {
        quint16 *Values = nullptr;
        Values = new quint16[LenIn];

        for(int i=0; i<LenIn; i++)
        {
            Values[i] = static_cast<quint16>(ValuesIn[i]);
        }

        this->setValues(Values, LenIn);
        delete Values;
    }

    return (0);
}

/**
@brief  Set Raw-values to registers.
@param  ObjIn - link to JsonObject.
@return The number of registers that had been changing.
@detailed ObjIn = { Var:Value, ... }
*/
quint16 RegsGroup::setValues(QJsonObject &ObjIn)
{
    quint16 Num = 0;

    if(!ObjIn.isEmpty())
    {
        QStringList Keys = ObjIn.keys();
        QString Key;

        for(int i=0; i<Keys.size(); i++)
        {
            Key = Keys.at(i);
            if(this->setValue(Key, static_cast<quint16>(ObjIn.value(Key).toInt(0))))
            {
                Num++;
            }
        }
    }

    return (Num);
}


/**
@brief  Set Raw-value.
@param  VarIn - variable name.
@param  ValueIn - value.
@return True if value is set, otherwise - False.
*/
bool RegsGroup::setValue(QString &VarIn, quint16 ValueIn)
{
    int Len = mListRegisters.size();
    Register *Reg;

    for(int i=0; i<Len; i++)
    {
        Reg = mListRegisters.at(i);
        if(Reg)
        {
            if(Reg->mVar == VarIn)
            {
                Reg->mValue = ValueIn;
                return (true);
            }
        }
    }
    return (false);
}

/**
@brief  Set Raw-value.
@param  VarIn - variable name.
@param  ValueIn - value.
@return True if value is set, otherwise - False.
*/
bool RegsGroup::setValue(QString &VarIn, quint8 ValueIn)
{
    return (this->setValue(VarIn, static_cast<quint8>(ValueIn)));
}


/**
@brief  Get size of the group.
@param  None.
@return Size of the group.
*/
quint16 RegsGroup::size()
{
    return (static_cast<quint16>(mListRegisters.size()));
}


/**
@brief  Clear the group.
@param  None.
@return None.
*/
void RegsGroup::clear()
{
    if(!mListRegisters.isEmpty())
    {
        Register *Reg = nullptr;

        while(mListRegisters.size())
        {
            Reg = mListRegisters.takeLast();
            if(Reg) delete Reg;
        }
    }
}


/**
@brief  Read randomized data.
@param  None.
@return The number of registers that had been reading.
*/
quint16 RegsGroup::randomize()
{
    quint16 Num = 0;
    Register *Reg = nullptr;

    for(int i=0; i<mListRegisters.size(); i++)
    {
        Reg = mListRegisters.at(i);

        if(Reg)
        {
            Reg->randomize();
            Num++;
        }
    }

    return (Num);
}
