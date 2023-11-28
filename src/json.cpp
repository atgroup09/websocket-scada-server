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

#include "json.h"


/**
@brief  Constructor.
@param  None.
@return None.
*/
Json::Json(QObject *parent) : QObject(parent)
{

}


/**
@brief  Destructor.
@param  None.
@return None.
*/
Json::~Json()
{

}


/**
@brief  Public Method: Read JSON-file.
@param  FileIn - path to file;
@param  ErrIn  - parse error.
@return QJsonDocument.
*/
QJsonDocument Json::readFile(const QString &FileIn, QString &ErrIn)
{
    QJsonDocument Doc;

    if(!FileIn.isEmpty())
    {
        QFile File(FileIn);

        if(File.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QJsonParseError Err;
            QString Data = File.readAll();

            Doc = QJsonDocument::fromJson(Data.toUtf8(), &Err);

            if(Err.error != QJsonParseError::NoError)
            {
                ErrIn+= QString("Error parse json-file %1! %2").arg(FileIn, Err.errorString());
            }

            File.close();
        }
        else
        {
            ErrIn+= QString("Error open json-file %1!").arg(FileIn);
        }
    }

    return (Doc);
}


/**
@brief  Public Method: Read JSON-file.
@param  FileIn - path to file.
@return QJsonDocument.
*/
QJsonDocument Json::readFile(const QString &FileIn)
{
    QString Err;
    return (Json::readFile(FileIn, Err));
}
