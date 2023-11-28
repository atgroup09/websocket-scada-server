/* Copyright (C) 2019 ATgroup09
*  Copyright (C) 2013  Pavel Bobkov  (www.chipenable.ru)
*  Contact: atgroup09@gmail.com
*
*  Bit functions.
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

#include "bit.h"


/**
@brief      copy bits.
@param      NumIn  - source number;
            StartBitIn - start bit position (1 ... 32);
            NumBitsIn - the number of bits (1 ... 32).
@return     number. (0 by default)
*/
quint32 copyBits(const quint32 NumIn, const quint8 StartBitIn, const quint8 NumBitsIn)
{
    quint32 Result = 0;

    if(StartBitIn <= 32)
    {
        quint8 NumBits = (((StartBitIn+NumBitsIn) <= 32) ? NumBitsIn : 32-StartBitIn);

        for(quint8 i=StartBitIn-1; i<=NumBits-1; i++)
        {
            if(BIT_IS_SET(NumIn, i)) SET_BIT(Result, i);
        }
    }

    return (Result);
}


/**
@brief      copy bits.
@param      NumIn  - source number;
            StartBitIn - start bit position (1 ... 32);
            NumBitsIn - the number of bits (1 ... 32).
@return     number. (0 by default)
*/
quint32 copyBits32(const quint32 NumIn, const quint8 StartBitIn, const quint8 NumBitsIn)
{
    return (copyBits(NumIn, StartBitIn, NumBitsIn));
}


/**
@brief      copy bits.
@param      NumIn  - source number;
            StartBitIn - start bit position (1 ... 16);
            NumBitsIn - the number of bits (1 ... 16).
@return     number. (0 by default)
*/
quint16 copyBits16(const quint16 NumIn, const quint8 StartBitIn, const quint8 NumBitsIn)
{
    return ((StartBitIn <= 16) ? static_cast<quint16>(copyBits(static_cast<quint32>(NumIn), StartBitIn, NumBitsIn)) : static_cast<quint16>(0));
}


/**
@brief      copy bits.
@param      NumIn  - source number;
            StartBitIn - start bit position (1 ... 8);
            NumBitsIn - the number of bits (1 ... 8).
@return     number. (0 by default)
*/
quint8 copyBits8(const quint8 NumIn, const quint8 StartBitIn, const quint8 NumBitsIn)
{
    return ((StartBitIn <= 8) ? static_cast<quint8>(copyBits(static_cast<quint32>(NumIn), StartBitIn, NumBitsIn)) : static_cast<quint8>(0));
}


/**
@brief      print bits.
@param      NumIn - number;
            NumBitsIn - the number of bits in NumIn (1 ... 32).
@return     none.
*/
void printBits(const quint32 NumIn, const quint8 NumBitsIn)
{
    quint32 i, b, nBits = 0;

    if(NumBitsIn > 0)
    {
        nBits = ((NumBitsIn < 32) ? NumBitsIn : 32);
    }

    for(i=nBits; i>0; i--)
    {
        b = GET_BIT_VALUE(NumIn, (i-1));
        std::cout << b;
    }
}


/**
@brief      print bits.
@param      NumIn - number.
@return     none.
*/
void print32Bits(const quint32 NumIn)
{
    printBits(NumIn, static_cast<quint8>(32));
}


/**
@brief      print bits.
@param      NumIn - number.
@return     none.
*/
void print16Bits(const quint16 NumIn)
{
    printBits(static_cast<quint32>(NumIn), static_cast<quint8>(16));
}


/**
@brief      print bits.
@param      NumIn - number.
@return     none.
*/
void print8Bits(const quint8 NumIn)
{
    printBits(static_cast<quint32>(NumIn), static_cast<quint8>(8));
}


/**
@brief      print DWORD by bytes.
@param      NumIn - number;
            BaseIn - base (DEC_BASE by default)
@return     none.
*/
void printDwordByBytes(const quint32 NumIn, const quint8 BaseIn)
{
    quint8 Byte;
    int Base = ((BaseIn == HEX_BASE) ? HEX_BASE : DEC_BASE);

    Byte = BYTE0(NumIn);
    std::cout << QString::number(Byte, Base).toStdString();
    std::cout << " ";

    Byte = BYTE1(NumIn);
    std::cout << QString::number(Byte, Base).toStdString();
    std::cout << " ";

    Byte = BYTE2(NumIn);
    std::cout << QString::number(Byte, Base).toStdString();
    std::cout << " ";

    Byte = BYTE3(NumIn);
    std::cout << QString::number(Byte, Base).toStdString();
}
