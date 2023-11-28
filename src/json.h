/* Copyright (C) 2019 ATgroup09
*  Contact: atgroup09@gmail.com
*
*  This file is part of the independent library.
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

#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <QObject>
#include <QFile>
#include <QJsonDocument>


/**
@brief    JSON.
@detailed extended functions.
*/
class Json : public QObject
{
    Q_OBJECT

public:

    explicit Json(QObject *parent = nullptr);
    ~Json();


    /**
    Public methods
    */

    /**
    @brief  Public Method: Read JSON-file.
    @param  FileIn - path to file;
    @param  ErrIn  - errors of parser.
    @return QJsonDocument.
    */
    static QJsonDocument readFile(const QString &FileIn, QString &ErrIn);

    /**
    @brief  Public Method: Read JSON-file.
    @param  FileIn - path to file.
    @return QJsonDocument.
    */
    static QJsonDocument readFile(const QString &FileIn);
};

#endif // JSON_H
