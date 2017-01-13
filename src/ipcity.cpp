// Copyright (C) 2014 - 2017 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <QJsonDocument>                                                           
#include <QJsonObject>

#include "ipcity.h"
#include "globaldeclarations.h"

IpCity::IpCity(HttpGet* parent) 
  : HttpGet(parent)
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

IpCity::~IpCity() 
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void IpCity::get() { HttpGet::get(IPLOOKUP_URL); }

void IpCity::finished(QNetworkReply* reply) 
{
    QString replyStr = QString(reply->readAll());
    QJsonDocument doc = QJsonDocument::fromJson(replyStr.toUtf8());
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
    qDebug() << replyStr;
#endif
    if (!doc.isObject()) return;
    QJsonObject obj = doc.object();
    QString city = obj["city"].toString();
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << city;
#endif
    Q_EMIT cityChanged(city);
}
