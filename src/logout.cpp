// Copyright (C) 2015 - 2017 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "logout.h"
#include "globaldeclarations.h"

LogOut::LogOut(HttpGet* parent)
  : HttpGet(parent)
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
    get();
}

LogOut::~LogOut()
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void LogOut::m_get(QString host)
{
    QString url = host + WX_CGI_PATH + "webwxlogout";
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    HttpGet::get(url);
}

void LogOut::get() { m_get(WX_SERVER_HOST); }

void LogOut::getV2() { m_get(WX_V2_SERVER_HOST); }
