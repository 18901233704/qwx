// Copyright (C) 2014 - 2017 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QLocale>
#include <QDateTime>

#include <random>
#include <time.h>

#include "uploadmedia.h"
#include "cookie.h"
#include "globaldeclarations.h"

UploadMedia::UploadMedia(QString filePath, 
                         int mediaCount, 
                         QString ticket,
                         QString uin,
                         QString sid,
                         QString skey,
                         QString deviceId, 
                         QObject* parent) 
  : QObject(parent)
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
    if (QFile::exists(filePath)) {
        QString url = FILE_SERVER_HOST + WX_CGI_PATH + "webwxuploadmedia?f=json";
        QMimeDatabase mimeDb;
        QMimeType mimeType = mimeDb.mimeTypeForFile(filePath);
#ifndef NDEBUG
        qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << mimeType << mimeType.name();
#endif
        QString mediaType = mimeType.name().startsWith("image/") ? "pic" : "doc";
        QLocale locale = QLocale(QLocale::English);
        QDateTime lastModifieDate = QDateTime::currentDateTime();
        QString lastModifieDateStr = locale.toString(lastModifieDate, "ddd MMM dd yyyy HH:mm:ss") + " GMT+0800 (China Standard Time)";
        qint64 fileSize = QFileInfo(filePath).size();
		std::mt19937 eng(time(NULL));
        std::uniform_int_distribution<long long> rand(1615250492, 519062714508114);
        QString clientMediaId = QString::number(rand(eng));
        QString webwxDataTicket = Cookie::getTicket();
#ifndef NDEBUG
        qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << lastModifieDateStr 
                 << fileSize << clientMediaId << webwxDataTicket;
#endif
    } else {
        qWarning() << "WARNING:" << filePath << "not exists!";
    }
}

UploadMedia::~UploadMedia() 
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}
