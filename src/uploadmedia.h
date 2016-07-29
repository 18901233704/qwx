// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef UPLOAD_MEDIA_H
#define UPLOAD_MEDIA_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class UploadMedia : public QObject 
{
    Q_OBJECT

public:
    UploadMedia(QString filePath, 
                int mediaCount, 
                QString ticket,
                QString uin,
                QString sid,
                QString skey,
                QString deviceId,
                QObject* parent = nullptr);
    virtual ~UploadMedia();

Q_SIGNALS:
    void finished(QString json);

private:
    QNetworkAccessManager m_nam;
    QMetaObject::Connection m_sslErrorConnection;
    QMetaObject::Connection m_finishConnection;
};

#endif // UPLOAD_MEDIA_H
