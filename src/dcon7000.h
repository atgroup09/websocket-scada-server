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

#ifndef DCON7000_H
#define DCON7000_H

#include <iostream>
#include <QObject>
#include <QString>
#include <QByteArray>
#include "bit.h"


/**
@brief DCON data protocol helper for modules ICP DAS of i7000 series.
*/
class DCON7000 : public QObject
{
    Q_OBJECT

public:

    /**
    @brief  Constructor.
    @param  None.
    @return None.
    */
    explicit DCON7000(QObject *parent = nullptr);

    /**
    @brief  Destructor.
    @param  None.
    @return None.
    */
    virtual ~DCON7000();


public:

    /**
    Protected options
    */

    /**
    @brief Commands
    @detailed 6 - read digital input/output status
    */
    static const char COMMAND_6;


    /**
    Protected methods
    */

    /**
    @brief    Calculate checksum.
    @param    DataIn - characters of command(or response) string except the character return(cr).
    @return   Checksum.
    @details  1. Calculate ASCII sum of all characters of command(or response) string except the character return(cr).
              2. Mask the sum of string with 0x0ff.
    */
    static quint8 chk(const QByteArray &DataIn);

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
    static bool cmd(const char &LeadingIn, const quint8 &AddressIn, const QByteArray &DataIn, const bool UseChkIn, QByteArray &ToIn);

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
    static bool cmdAA6(const quint8 &AddressIn, const bool UseChkIn, QByteArray &ToIn);

    /**
    @brief    Create command "$AA6" without checksum.
    @param    AddressIn - address of module (decimal)
    @param    ToIn      - link to output buffer
    @return   True if command is created, otherwise - false.
    */
    static bool cmdAA6(const quint8 &AddressIn, QByteArray &ToIn);

    /**
    @brief    Parse valid response on command "$AA6".
    @param    RespIn - link to response
    @param    DataIn - link to buffer where data will be saved
    @return   True if valid response, otherwise - false.
    @details
              Valid response: !(Data)[CHK](cr)
                              ! 0000  00   \r
    */
    static bool respAA6(const QByteArray &RespIn, QByteArray &DataIn);

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
    static quint16 respAA6_7041(const QByteArray &RespIn);
};

#endif // DCON7000_H
