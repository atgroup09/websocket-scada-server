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

#ifndef REGS_GROUP_H
#define REGS_GROUP_H

#include "log.h"
#include "register.h"


/** Group of registers (by class and address)
*/


/**
@brief Group of registers.
*/
class RegsGroup : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit RegsGroup(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~RegsGroup();


    /**
    Public options
    */

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
    @brief  Pack data to String buffer.
    @param  StringIn - link to string buffer.
    @return None.
    */
    void toString(QString &StringIn);

    /**
    @brief  Pack data to JSON buffer.
    @param  ObjIn - link to JsonObject.
    @return None.
    @detailed { ..., Register.mVar:Register.mValue }
    */
    void toJson(QJsonObject &ObjIn);

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
    int toSqlValue(const QString &ProfileIn, bool EventsIn, QList<QString> &ListStringsIn);

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
    int toSqlCurrentValue(const QString &ProfileIn, QList<QString> &ListStringsIn);

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
    int toSqlEventValue(const QString &ProfileIn, QList<QString> &ListStringsIn);

    /**
    @brief  Check class of the group.
    @param  ClassIn - class of a register.
    @return True if a class is matched, otherwise - False.
    */
    bool isClass(QString ClassIn);

    /**
    @brief  Check allow to read.
    @param  None.
    @return True if a group or registers to read, otherwise - False.
    */
    bool isAllowToRead();

    /**
    @brief  Check allow to write.
    @param  None.
    @return True if a group or registers to write, otherwise - False.
    */
    bool isAllowToWrite();

    /**
    @brief  Check allow to HMI.
    @param  None.
    @return True if a group or registers to HMI, otherwise - False.
    */
    bool isAllowToHmi();

    /**
    @brief  Get class of the group.
    @param  None.
    @return Class of the group.
    */
    QString getClass();

    /**
    @brief  Get address of first register of the group.
    @param  None.
    @return Address of first register of the group.
    */
    quint16 getFirstAddr();

    /**
    @brief  Get address of last register of the group.
    @param  None.
    @return Address of last register of the group.
    */
    quint16 getLastAddr();

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
    bool isValid(const QString ClassIn, const quint16 RegAddrIn, const bool AllowReadIn, const bool AllowWriteIn, const bool AllowHmiIn);

    /**
    @brief  Validate a register to add into the group.
    @param  RegIn - a register.
    @return True if a register is valid (may be add into the group), otherwise - False.
    @detailed Group by: Class AND Addr AND AllowRead
    */
    bool isValid(Register *RegIn);

    /**
    @brief  Add valid register into the group.
    @param  RegIn - register.
    @return True if a register had been adding, otherwise - False.
    */
    bool add(Register *RegIn);

    /**
    @brief  Get register by the list iterator.
    @param  IterNumIn - an iterator number.
    @return Register or nullptr.
    */
    Register *get(quint16 IterNumIn);

    /**
    @brief  Get register by ID.
    @param  IDIn - ID of register.
    @return Register or nullptr.
    */
    Register *getByID(quint16 IDIn);

    /**
    @brief  Get value of register by ID.
    @param  IDIn - ID of register.
    @return Value of register or nullptr.
    */
    quint16 *getValueByID(quint16 IDIn);

    /**
    @brief  Get registers by ID.
    @param  ListIDsIn - list of ID of registers.
    @return List of registers.
    */
    QList<Register *> getByID(const QList<quint16> &ListIDsIn);

    /**
    @brief  Get values of registers by ID.
    @param  ListIDsIn - list of ID of registers.
    @return Values of registers.
    */
    QList<quint16 *> getValuesByID(const QList<quint16> &ListIDsIn);

    /**
    @brief  Get all registers of the group.
    @param  None.
    @return List of registers.
    */
    QList<Register *> getAll();

    /**
    @brief  Get Raw-values of the group registers.
    @param  ValuesIn - pointer to array for Raw-values;
    @param  LenIn    - quantity of returned values (<= group.size).
    @return The number of returned values.
    */
    quint16 getValues(quint8 *ValuesIn, quint16 LenIn);

    /**
    @brief  Get Raw-values of the group registers.
    @param  ValuesIn - pointer to array for Raw-values;
    @return The number of returned values.
    */
    quint16 getValues(quint8 *ValuesIn);

    /**
    @brief  Get Raw-values of registers.
    @param  ValuesIn - pointer to array for Raw-values;
    @param  LenIn    - quantity of returned values (<= group.size).
    @return The number of returned values.
    */
    quint16 getValues(quint16 *ValuesIn, quint16 LenIn);

    /**
    @brief  Get Raw-values of registers.
    @param  ValuesIn - pointer to array for Raw-values;
    @return The number of returned values.
    */
    quint16 getValues(quint16 *ValuesIn);

    /**
    @brief  Set Raw-values to registers.
    @param  ValuesIn - pointer to array of Raw-values;
    @param  LenIn    - size of array of values.
    @return The number of registers that had been changing.
    */
    quint16 setValues(quint16 *ValuesIn, quint16 LenIn);

    /**
    @brief  Set Raw-values to registers.
    @param  ValuesIn - pointer to array of Raw-values;
    @param  LenIn    - size of array of values.
    @return The number of registers that had been changing.
    */
    quint16 setValues(const quint8 *ValuesIn, const quint16 LenIn);

    /**
    @brief  Set Raw-values to registers.
    @param  ObjIn - link to JsonObject.
    @return The number of registers that had been changing.
    @detailed ObjIn = { Var:Value, ... }
    */
    quint16 setValues(QJsonObject &ObjIn);

    /**
    @brief  Set Raw-value.
    @param  VarIn - variable name.
    @param  ValueIn - value.
    @return True if value is set, otherwise - False.
    */
    bool setValue(QString &VarIn, quint16 ValueIn);

    /**
    @brief  Set Raw-value.
    @param  VarIn - variable name.
    @param  ValueIn - value.
    @return True if value is set, otherwise - False.
    */
    bool setValue(QString &VarIn, quint8 ValueIn);

    /**
    @brief  Get size of the group.
    @param  None.
    @return Size of the group.
    */
    quint16 size();

    /**
    @brief  Clear the group.
    @param  None.
    @return None.
    */
    void clear();

    /**
    @brief  Read randomized data.
    @param  None.
    @return The number of registers that had been reading.
    */
    quint16 randomize();


private:

    /**
    Private options
    */

    /**
    @brief Class of register.
    */
    QString mClass;

    /**
    @brief Allow to read of register.
    */
    bool mAllowRead;

    /**
    @brief Allow to write of register.
    */
    bool mAllowWrite;

    /**
    @brief Allow to HMI of register.
    */
    bool mAllowHmi;

    /**
    @brief List of registers.
    */
    QList<Register *> mListRegisters;
};

#endif // REGS_GROUP_H
