// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef QWXUUID_H
#define QWXUUID_H

#include "httpget.h"

class QwxUUID : public HttpGet
{
    Q_OBJECT

public:
    explicit QwxUUID(HttpGet* parent = Q_NULLPTR);
    virtual ~QwxUUID();

    Q_INVOKABLE void get();

Q_SIGNALS:
    void error();
    void uuidChanged(QString uuid);
    void autologin(QString uin, QString sid, QString ticket, bool isV2);

protected:
    void finished(QNetworkReply* reply);
};

#endif // QWXUUID_H
