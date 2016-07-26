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

#include "qtsingleapplication/QtSingleApplication"
#include "uploadmediatest.h"

static int m_mediaCount = 1;

UploadMediaTest::UploadMediaTest(QString filePath, QObject *parent)
    : QObject(parent)
{
    m_uploader = new UploadMedia(filePath, m_mediaCount++, "ticket");
}

UploadMediaTest::~UploadMediaTest()
{
    if (m_uploader) {
        delete m_uploader;
        m_uploader = nullptr;
    }
}

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("uploadmediatest"));

    UploadMediaTest *test = new UploadMediaTest(argv[1] ? argv[1] : "/tmp/test.png");
    return app.exec();
}

#include "moc_uploadmediatest.cpp"
