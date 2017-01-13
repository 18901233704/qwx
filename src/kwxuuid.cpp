// Copyright (C) 2014 - 2017 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <time.h>

#include <QTimer>

#include "kwxuuid.h"
#include "cookie.h"
#include "globaldeclarations.h"

QwxUUID::QwxUUID(HttpGet* parent)
  : HttpGet(parent)
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
    get();
}

QwxUUID::~QwxUUID()
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void QwxUUID::get()
{
    if (Cookie::exists()) {
        QTimer::singleShot(100, this, [this]{
            Q_EMIT autologin(Cookie::getUin(), Cookie::getSid(),
                             Cookie::getTicket(), Cookie::isV2());
        });
        return;
    }

    QString url = LOGIN_SERVER_HOST + "/jslogin?appid=wx782c26e4c19acffb"
        "&redirect_uri=" + WX_SERVER_HOST + WX_CGI_PATH + "webwxnewloginpage"
        "&fun=new&lang=zh_CN&_=" + QString::number(time(NULL));
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    HttpGet::get(url);
}

void QwxUUID::finished(QNetworkReply* reply)
{
    QString replyStr(reply->readAll());
    QString uuidStr = "";
    QString qruuidStr = "window.QRLogin.uuid = \"";
    int index = -1;

#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
    qDebug() << "DEBUG:" << replyStr;
#endif
    index = replyStr.indexOf(qruuidStr) + qruuidStr.size();
    if (index == -1) {
        qWarning() << "ERROR:" << __PRETTY_FUNCTION__ << "uuid not found!";
        Q_EMIT error();
        return;
    }
    uuidStr = replyStr.mid(index, replyStr.size() - index - QString("\";").size());
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << uuidStr;
#endif
    if (uuidStr == "") {
        Q_EMIT error();
        return;
    }
    Q_EMIT uuidChanged(uuidStr);
}
