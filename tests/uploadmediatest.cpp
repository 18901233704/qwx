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

#include "uploadmediatest.h"

static int m_mediaCount = 1;

UploadMediaTest::UploadMediaTest(QString filePath, QObject *parent)
    : QObject(parent), 
      m_uploaderPtr(new UploadMedia(filePath, m_mediaCount++, 
                  "ticket", "uin", "sid", "skey", "deviceId"))
{
}

UploadMediaTest::~UploadMediaTest()
{
}

int main(int argc, char *argv[])
{
    // FIXME: lead QTBUG-52988 https://bugreports.qt.io/browse/QTBUG-52988
    QScopedPointer<UploadMediaTest> test(new UploadMediaTest(argv[1] ? argv[1] : "/tmp/test.png"));
    return 0;
}

#include "moc_uploadmediatest.cpp"
