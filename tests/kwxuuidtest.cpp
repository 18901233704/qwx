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

#include "kwxuuidtest.h"

QwxUUIDTest::QwxUUIDTest(QObject *parent)
    : QObject(parent), 
      m_qwxuuidPtr(new QwxUUID)
{
    connect(m_qwxuuidPtr.data(), &QwxUUID::autologin, 
            [this](QString uin, QString sid, QString ticket, bool isV2){
        qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << "try to autologin" 
                 << uin << sid << ticket << isV2;
    });
}

QwxUUIDTest::~QwxUUIDTest()
{
}

int main(int argc, char *argv[])
{
    QScopedPointer<QwxUUIDTest> test(new QwxUUIDTest);
    return 0;
}

#include "moc_kwxuuidtest.cpp"
