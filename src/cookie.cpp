// Copyright (C) 2014 - 2017 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QSettings>

#include <time.h>

#include "cookie.h"
#include "globaldeclarations.h"

Cookie::Cookie(HttpGet* parent) 
  : HttpGet(parent)
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

Cookie::~Cookie() 
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void Cookie::get(QString redirect_uri) 
{
    QString url = redirect_uri + "&fun=new";
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    HttpGet::get(url);
}

void Cookie::getV2(QString redirect_uri)
{
    QString url = redirect_uri.replace(WX_SERVER_HOST, WX_V2_SERVER_HOST) + "&fun=old";
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    HttpGet::get(url);
}

QString Cookie::getTicket()
{
    QSettings cookie(QWXDIR + "/" + COOKIE_FILENAME, QSettings::NativeFormat);
#ifndef NDEBUG
    qDebug() << cookie.allKeys();
#endif
    return cookie.value("webwx_data_ticket").toString();
}

QString Cookie::getUin()
{
    QSettings cookie(QWXDIR + "/" + COOKIE_FILENAME, QSettings::NativeFormat);
    return cookie.value("wxuin").toString();
}

QString Cookie::getSid()
{
    QSettings cookie(QWXDIR + "/" + COOKIE_FILENAME, QSettings::NativeFormat);
    return cookie.value("wxsid").toString();
}

bool Cookie::isV2() 
{
    QSettings cookie(QWXDIR + "/" + COOKIE_FILENAME, QSettings::NativeFormat);
    return cookie.value("v2").toBool();
}

bool Cookie::exists()
{
    return QFile::exists(QWXDIR + "/" + COOKIE_FILENAME);
}

void Cookie::finished(QNetworkReply* reply) 
{
    QString replyStr(reply->readAll());
    QString uinStr = "";
    QString sidStr = "";
    QString ticketStr = "";
    QString expires = "";
    QString domain = "";
    QString path = "";
    QString webwxuvid = ""; 
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
    qDebug() << "DEBUG:" << replyStr;
#endif
    QString cookiePath = QWXDIR + "/" + COOKIE_FILENAME;
    QDir qwxDir(QWXDIR);
    if (!qwxDir.exists(cookiePath)) qwxDir.mkdir(QWXDIR);
    QFile file(cookiePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qWarning() << "ERROR: fail to save cookie!";
        return;
    }
    QTextStream out(&file);
    QLocale locale = QLocale(QLocale::C, QLocale::AnyCountry);
    Q_FOREACH (const QNetworkCookie cookie, HttpGet::cookies()) {
        if (HttpGet::cookies().size() > 1) {
            out << QString(cookie.name()) << "=" << QString(cookie.value()) 
                << "; expires=" << locale.toString(cookie.expirationDate(), 
                   "ddd, dd-MMM-yyyy hh:mm:ss") + " GMT" 
                << "; domain=" << cookie.domain() 
                << "; path=" << cookie.path() << endl;
        }
        
        if (QString(cookie.name()) == "webwxuvid") 
            webwxuvid = QString(cookie.value());

        expires = locale.toString(cookie.expirationDate(), 
                                  "ddd, dd-MMM-yyyy hh:mm:ss") + " GMT";
        domain = cookie.domain();
        path = cookie.path();
        if (cookie.name() == "wxuin") 
            uinStr = QString(cookie.value());
        else if (cookie.name() == "wxsid") 
            sidStr = QString(cookie.value());
        else if (cookie.name() == "webwx_data_ticket") 
            ticketStr = QString(cookie.value());
    }
    // TODO: so if webwx V1 fail to get uin and sid, switch to V2
    if (uinStr == "" || sidStr == "" || ticketStr == "") {
        QString href = "window.location.href=";
        int index = replyStr.indexOf(href);
        if (index == -1) {
            qWarning() << "V2 still use XML format for cookie?";
            return;
        } else {
            out << "v2=true";
            Q_EMIT switchToV2();
        }
    } else { 
        Q_EMIT infoChanged(uinStr, sidStr, ticketStr);
    }
    file.close();
}
