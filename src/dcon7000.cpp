/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  DCON data protocol helper
*  for modules ICP DAS of i7000 series.
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

#include "dcon7000.h"


/**
@brief    Commands
@detailed
          6 - read digital input/output status
*/
const char DCON7000::COMMAND_6 = '6';


/**
@brief    Constructor.
@param    None.
@return   None.
*/
DCON7000::DCON7000(QObject *parent) : QObject(parent)
{

}


/**
@brief    Destructor.
@param    None.
@return   None.
*/
DCON7000::~DCON7000()
{

}


/**
@brief    Calculate checksum.
@param    DataIn - characters of command(or response) string except the character return(cr).
@return   Checksum.
@details
          1. Calculate ASCII sum of all characters of command(or response) string except the character return(cr).
          2. Mask the sum of string with 0x0ff.
*/
quint8 DCON7000::chk(const QByteArray &DataIn)
{
    int Size = DataIn.size();
    int ChCode, Crc = 0;

    for(int i=0; i<Size; i++)
    {
        ChCode = static_cast<int>(DataIn[i]);
        Crc   += ChCode;

        //if(i) std::cout << "+";
        //std::cout << QString("%1h").arg(QString::number(ChCode, 16).toUpper()).toStdString();
    }
    //std::cout << " = " << QString("%1h").arg(QString::number(Crc, 16).toUpper()).toStdString() << std::endl;

    Crc &= 0x0ff;
    //std::cout << " = " << QString("%1h").arg(QString::number(Crc, 16).toUpper()).toStdString() << std::endl;

    return (static_cast<quint8>(Crc));
}


/**
@brief    Create command.
@param    LeadingIn - leading delimiter character
@param    AddressIn - address of module (decimal)
@param    DataIn    - data (not empty)
@param    UseChkIn  - True if checksum enabled, otherwise - False
@param    ToIn      - link to output buffer
@return   True if command is created, otherwise - false.
@details
          Format of the result: (Leading)(Address)(Data)[CHK](cr)

          where, (Leading) - 1-character delimeter
                 (Address) - 2-character address of device
                 [CHK]     - 2-character checksum
                 (cr)      - end-of-command character (0x0D)
*/
bool DCON7000::cmd(const char &LeadingIn, const quint8 &AddressIn, const QByteArray &DataIn, const bool UseChkIn, QByteArray &ToIn)
{
    if(!DataIn.isEmpty())
    {
        ToIn.append(LeadingIn);

        if(AddressIn < 10) ToIn.append('0');
        ToIn.append(QString::number(AddressIn, 16).toUpper());

        ToIn.append(DataIn);

        if(UseChkIn)
        {
            quint8 Crc = chk(ToIn);
            ToIn.append(QString::number(BIT4(Crc, 0), 16).toUpper());
            ToIn.append(QString::number(BIT4(Crc, 1), 16).toUpper());
        }

        ToIn.append('\r');

        return (true);
    }

    return (false);
}


/**
@brief    Create command "$AA6".
@param    AddressIn - address of module (decimal)
@param    UseChkIn  - True if checksum enabled, otherwise - False
@param    ToIn      - link to output buffer
@return   True if command is created, otherwise - false.
@details
          Read Digital I/O Status

          where, $  - delimiter character
                 AA - address of reading module
                 6  - command for read digital input/output status
*/
bool DCON7000::cmdAA6(const quint8 &AddressIn, const bool UseChkIn, QByteArray &ToIn)
{
    QByteArray Data;
    Data.append(COMMAND_6);

    return (cmd('$', AddressIn, Data, UseChkIn, ToIn));
}


/**
@brief    Create command "$AA6" without checksum.
@param    AddressIn - address of module (decimal)
@param    ToIn      - link to output buffer
@return   True if command is created, otherwise - false.
*/
bool DCON7000::cmdAA6(const quint8 &AddressIn, QByteArray &ToIn)
{
    return (cmdAA6(AddressIn, false, ToIn));
}

/**
@brief    Parse valid response on command "$AA6".
@param    RespIn - link to response
@param    DataIn - link to buffer where data will be saved
@return   True if valid response, otherwise - false.
@details
          Valid response: !(Data)[CHK](cr)
                          ! 0000  00   \r
*/
bool DCON7000::respAA6(const QByteArray &RespIn, QByteArray &DataIn)
{
    if(!RespIn.isEmpty())
    {
        if(RespIn.at(0) == '!')
        {
            int MinSize = 8;
            int Size    = RespIn.size();

            if(Size >= MinSize)
            {
                QByteArray Data = RespIn.mid(1, 4);
                DataIn.append(Data);

                return (true);
            }
        }
    }

    return (false);
}

/**
@brief    Parse valid response on command "$AA6" for module i7041/42.
@param    RespIn - link to response
@return   Word.
@details
          Valid response: !(Data)[CHK](cr)
                          ! 0000  00   \r

          where, (Data) is (First Data)(Second Data)
            (First Data): DI08..13: 00 to 3F
           (Second Data): DI00..07: 00 to FF
*/
quint16 DCON7000::respAA6_7041(const QByteArray &RespIn)
{
    quint16 Result = 0;

    QByteArray Data;

    if(respAA6(RespIn, Data))
    {
        bool ok;
        quint8 First  = static_cast<quint8>(Data.mid(0, 2).toInt(&ok, 16));
        quint8 Second = static_cast<quint8>(Data.mid(2, 2).toInt(&ok, 16));

        Result = static_cast<quint16>(MERGE_WORD(Second, First));
    }

    return (Result);
}
