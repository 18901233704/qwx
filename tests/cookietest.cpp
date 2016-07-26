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
#include "cookietest.h"

CookieTest::CookieTest(QObject *parent)
    : QObject(parent)
{
    m_cookie = new Cookie;
    qDebug() << m_cookie->getDataTicket();
}

CookieTest::~CookieTest()
{
    if (m_cookie) {
        delete m_cookie;
        m_cookie = nullptr;
    }
}

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("cookietest"));

    CookieTest *test = new CookieTest;
    return app.exec();
}

#include "moc_cookietest.cpp"
