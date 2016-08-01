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

#include "chatlogtest.h"

#include <QDebug>

ChatLogTest::ChatLogTest(QObject *parent)
    : QObject(parent), 
      m_chatLogPtr(new ChatLog)
{
    m_chatLogPtr->load("weixin");
    // QSharedPointer<ChatLogObject> can not be invoked by QML...
    Q_FOREACH (ChatLogObject *chatLogObjPtr, m_chatLogPtr->chatLogList()) {
        qDebug() << chatLogObjPtr->userName() << chatLogObjPtr->content();
    }
}

ChatLogTest::~ChatLogTest()
{
}

int main(int argc, char *argv[])
{
    QScopedPointer<ChatLogTest> test(new ChatLogTest);
    return 0;
}

#include "moc_chatlogtest.cpp"
