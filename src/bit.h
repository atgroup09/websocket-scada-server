/* Copyright (C) 2019 ATgroup09
*  Copyright (C) 2013  Pavel Bobkov  (www.chipenable.ru)
*
*  Contact: atgroup09@gmail.com
*
*  Bit functions
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


#ifndef BIT_H_
#define BIT_H_

#include <iostream>
#include <QtGlobal>
#include <QString>

//* PartIn == 0: first 4 bit of DWord
//* PartIn == 1: second 4 bit of DWord
//* ...
#define BIT4(DWordIn, PartIn)                                      ((DWordIn >> PartIn*4) & 0xF)

//* PartIn == 0: first 8 bit of DWord
//* PartIn == 1: second 8 bit of DWord
//* ...
#define BIT8(DWordIn, PartIn)                                      ((DWordIn >> PartIn*8) & 0xFF)

#define BYTE0(DWordIn)                                             ((DWordIn >> 0)  & 0xFF)
#define BYTE1(DWordIn)                                             ((DWordIn >> 8)  & 0xFF)
#define BYTE2(DWordIn)                                             ((DWordIn >> 16) & 0xFF)
#define BYTE3(DWordIn)                                             ((DWordIn >> 24) & 0xFF)

#define WORD0(DWordIn)                                             ((DWordIn >> 0)  & 0xFFFF)
#define WORD1(DWordIn)                                             ((DWordIn >> 16) & 0xFFFF)

//* Bit40In - first 4 bit of Byte
//* Bit41In - second 4 bit of Byte
#define MERGE_BYTE(Bit40In, Bit41In)                               (((0xFF & Bit41In) << 4) | Bit40In)

#define MERGE_WORD(Byte0In, Byte1In)                               (((0xFFFF & Byte1In) << 8) | Byte0In)
#define MERGE_DWORD(Byte0In, Byte1In, _byte2_in, _byte3_in)        (((0xFFFFFFFF & _byte3_in) << 24) | ((0xFFFFFFFF & _byte2_in) << 16) | ((0xFFFFFFFF & Byte1In) << 8) | Byte0In)
#define MERGE_DWORD_V2(Word0In, Word1In)                           (((0xFFFFFFFF & Word1In) << 16) | Word0In)

#define BIT0(BitIn)										           (0<<BitIn)
#define BIT1(BitIn)										           (1<<BitIn)

#define CLEAR_BITS(ValueIn)								           ValueIn = 0

#define CLEAR_BIT(ValueIn, BitIn)						           ValueIn &= (~(1<<(BitIn)))
#define SET_BIT(ValueIn, BitIn)							           ValueIn |= (1<<(BitIn))

//* DWordIn - DWORD, where WORD will be inserted
//* WordIn  - WORD
//* PosIn   - insert position (0, 1)
#define SET_WORD(DWordIn, WordIn, PosIn)                           DWordIn |= ((0xFFFFFFFF & WordIn) << PosIn*16)

#define GET_BIT_VALUE(ValueIn, BitIn)					           ((((1<<BitIn)&ValueIn)&(1<<BitIn))!=0 ? 1 : 0)

#define BIT_IS_CLEAR(ValueIn, BitIn)					           ((ValueIn&(1<<(BitIn)))==0)
#define BIT_IS_SET(ValueIn, BitIn)						           ((ValueIn&(1<<(BitIn)))!=0)

#define INVERT_BIT(ValueIn, BitIn)						           ValueIn ^= (1<<(BitIn))

#define BIT_TRUE												   0
#define BIT_FALSE												   1

#define BIT_LO											           0
#define BIT_HI										               1

#define HEX_BASE                                                   16
#define DEC_BASE                                                   10


/** @def    Set bit into byte.
 *  @param  ByteIn   - target byte.
 *  @param  BitValIn - bit value
 *  @arg      = 0
 *  @arg      = 1
 *  @param  BitPosIn - bit position:
 *  @arg      = 0 ... 7
 *  @return New byte.
 *  @details Ex.: Byte = BYTE_SET_BIT(Byte, 1, 0);
 *                Byte = BYTE_SET_BIT(Byte, 0, 7);
 */
#define BYTE_SET_BIT(ByteIn, BitValIn, BitPosIn)                   ((BitValIn == 0) ? (ByteIn & (~(1<<BitPosIn))) : (ByteIn | (1<<BitPosIn)))


/** @def    Set byte into word.
 *  @param  WordIn - target word.
 *  @param  ByteIn - byte.
 *  @param  PosIn  - byte position:
 *  @arg      = 0
 *  @arg      = 1
 *  @return New word.
 *  @details Ex.: Word = WORD_SET_BYTE(Word, Byte0, 0);
 *                Word = WORD_SET_BYTE(Word, Byte1, 1);
 */
#define WORD_SET_BYTE(WordIn, ByteIn, PosIn)                       ((PosIn == 0) ? ((WordIn & 0xFF00) | ByteIn) : ((WordIn & 0x00FF) | ((0xFFFF & ByteIn) << 8)))


/** @def    Test value on limit
 *  @param  MinIn - minimum value
 *  @param  ValIn - value
 *  @param  MaxIn - maximum value
 *  @return Result code:
 *  @arg      = 0 - the value is out of limit
 *  @arg      = 1 - the value is in limit
 *  @details Min <= Val <= Max
 *           Ex.: if(VAL_IN_LIMITS(MinIn, ValIn, MaxIn)) ...;
 */
#define VAL_IN_LIMITS(MinIn, ValIn, MaxIn)                         ((ValIn >= MinIn && ValIn <= MaxIn) ? 1 : 0)



/**
@brief      copy bits.
@param      NumIn  - source number;
            StartBitIn - start bit position;
            NumBitsIn - the number of bits (0 ... 31).
@return     number. (0 by default)
*/
quint32 copyBits(const quint32 NumIn, const quint8 StartBitIn, const quint8 NumBitsIn);


/**
@brief      copy bits.
@param      NumIn  - source number;
            StartBitIn - start bit position;
            NumBitsIn - the number of bits (0 ... 31).
@return     number. (0 by default)
*/
quint32 copyBits32(const quint32 NumIn, const quint8 StartBitIn, const quint8 NumBitsIn);


/**
@brief      copy bits.
@param      NumIn  - source number;
            StartBitIn - start bit position;
            NumBitsIn - the number of bits (0 ... 15).
@return     number. (0 by default)
*/
quint16 copyBits16(const quint16 NumIn, const quint8 StartBitIn, const quint8 NumBitsIn);


/**
@brief      copy bits.
@param      NumIn  - source number;
            StartBitIn - start bit position;
            NumBitsIn - the number of bits (0 ... 7).
@return     number. (0 by default)
*/
quint8 copyBits8(const quint8 NumIn, const quint8 StartBitIn, const quint8 NumBitsIn);


/**
@brief      print bits.
@param      NumIn - number;
            NumBitsIn - the number of bits in NumIn (0 ... 31).
@return     none.
*/
void printBits(const quint32 NumIn, const quint8 NumBitsIn);


/**
@brief      print bits.
@param      NumIn - number.
@return     none.
*/
void print32Bits(const quint32 NumIn);


/**
@brief      print bits.
@param      NumIn - number.
@return     none.
*/
void print16Bits(const quint16 NumIn);


/**
@brief      print bits.
@param      NumIn - number.
@return     none.
*/
void print8Bits(const quint8 NumIn);


/**
@brief      print DWORD by bytes.
@param      NumIn - number;
            BaseIn - base (DEC_BASE by default)
@return     none.
*/
void printDwordByBytes(const quint32 NumIn, const quint8 BaseIn);

#endif /* BIT_H_ */
