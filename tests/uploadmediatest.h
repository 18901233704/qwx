/* This file is part of the KDE desktop environment

   Copyright (C) 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef _UPLOADMEDIATEST_H_
#define _UPLOADMEDIATEST_H_

#include <QObject>

#include "uploadmedia.h"

class UploadMediaTest : public QObject
{
    Q_OBJECT

public:
    UploadMediaTest(QString filePath, QObject *parent = nullptr);
    ~UploadMediaTest();

private:
    UploadMedia *m_uploader = nullptr;
};

#endif
