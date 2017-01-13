// Copyright (C) 2014 - 2017 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <QJsonDocument>                                                           
#include <QJsonObject>

#include "xiaodoubi.h"
#include "globaldeclarations.h"

XiaoDouBi::XiaoDouBi(HttpGet* parent) 
  : HttpGet(parent)
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

XiaoDouBi::~XiaoDouBi() 
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void XiaoDouBi::get(QString word) 
{ 
    QString url = XIAODOUBI_URL + word;
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    HttpGet::get(url); 
}

void XiaoDouBi::finished(QNetworkReply* reply) 
{
    Q_EMIT contentChanged(QString(reply->readAll()));
}
