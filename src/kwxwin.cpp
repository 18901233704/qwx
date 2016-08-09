/**
 * Copyright (C) 2009 Ben Cooksley <bcooksley@kde.org>
 * Copyright (C) 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "kwxwin.h"
#include "globaldeclarations.h"
#include "kwxuuid.h"
#include "scan.h"
#include "cookie.h"
#include "statreport.h"
#include "init.h"
#include "sync.h"
#include "contact.h"
#include "headimg.h"
#include "statusnotify.h"
#include "sendmsg.h"
#include "monitor.h"
#include "getmsg.h"
#include "circleimage.h"
#include "xiaodoubi.h"
#include "chatlog.h"
#include "emotion.h"
#include "logout.h"
#include "ipcity.h"
#include "weather.h"
#include "i18n.h"
#include "clipboard.h"
#if KF5PRISON_FOUND
#include "qrcodequick.h"
#endif

#include <KDeclarative/KDeclarative>

#include <QQmlEngine>
#include <QScopedPointer>
#include <QQuickWidget>

KwxWin::KwxWin(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    KDeclarative::KDeclarative kdeclarative;

    qmlRegisterType<QwxUUID>("cn.com.isoft.qwx", 1, 0, "QwxUUID");
    qmlRegisterType<Scan>("cn.com.isoft.qwx", 1, 0, "Scan");
    qmlRegisterType<Cookie>("cn.com.isoft.qwx", 1, 0, "Cookie");
    qmlRegisterType<StatReport>("cn.com.isoft.qwx", 1, 0, "StatReport");
    qmlRegisterType<Init>("cn.com.isoft.qwx", 1, 0, "Init");
    qmlRegisterType<Sync>("cn.com.isoft.qwx", 1, 0, "Sync");
    qmlRegisterType<Contact>("cn.com.isoft.qwx", 1, 0, "Contact");
    qmlRegisterType<HeadImg>("cn.com.isoft.qwx", 1, 0, "HeadImg");
    qmlRegisterType<StatusNotify>("cn.com.isoft.qwx", 1, 0, "StatusNotify");
    qmlRegisterType<SendMsg>("cn.com.isoft.qwx", 1, 0, "SendMsg");
    qmlRegisterType<Monitor>("cn.com.isoft.qwx", 1, 0, "Monitor");
    qmlRegisterType<GetMsg>("cn.com.isoft.qwx", 1, 0, "GetMsg");
    qmlRegisterType<CircleImage>("cn.com.isoft.qwx", 1, 0, "CircleImage");
    qmlRegisterType<XiaoDouBi>("cn.com.isoft.qwx", 1, 0, "XiaoDouBi");
    qmlRegisterType<ChatLog>("cn.com.isoft.qwx", 1, 0, "ChatLog");
    qmlRegisterType<Emotion>("cn.com.isoft.qwx", 1, 0, "Emotion");
    qmlRegisterType<LogOut>("cn.com.isoft.qwx", 1, 0, "LogOut");
    qmlRegisterType<IpCity>("cn.com.isoft.qwx", 1, 0, "IpCity");
    qmlRegisterType<Weather>("cn.com.isoft.qwx", 1, 0, "Weather");
    qmlRegisterType<Clipboard>("cn.com.isoft.qwx", 1, 0, "Clipboard");
#if KF5PRISON_FOUND
    qmlRegisterType<QRcodeQuick>("cn.com.isoft.qwx", 1, 0, "QRcodeQuick");
#endif

    m_widget = new QQuickWidget(QUrl("qrc:/qml/main.qml"), parent);
    // To make QML code translatable, KDeclarative provides the same i18n() 
    // calls described above. To enable parsing at runtime, you need to install 
    // a KDeclarative object in the QDeclarativeEngine:
    kdeclarative.setDeclarativeEngine(m_widget->engine());
    kdeclarative.setTranslationDomain(CODE_NAME);
    // Call this after setDeclarativeEngine to set the i18n global functions, 
    // the runtime platform, etc
    kdeclarative.setupBindings();
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << kdeclarative.translationDomain();
#endif
    // the view will automatically resize the root item to the size of the view
    // that means QML will automatically resize to the KXmlGuiWindow's.
    m_widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    setCentralWidget(m_widget);
    setupGUI();

    resize(480, 822);
}

KwxWin::~KwxWin() 
{
    delete m_widget;
    m_widget = Q_NULLPTR;
}
