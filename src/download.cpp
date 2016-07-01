// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <QFile>
#include <QDir>

#include "download.h"
#include "globaldeclarations.h"

Download::Download(QObject* parent) 
  : QObject(parent), 
    m_reply(nullptr)
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

Download::~Download() 
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void Download::get(QString url, QString filePath, bool needCookie, bool isAppend)
{
    if (!isAppend && QFile::exists(filePath))
        return;

    QNetworkRequest request(url);
    QFile file(QWXDIR + "/" + COOKIE_FILENAME);

    // webwx download file need cookie
    if (needCookie && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream out(&file);                                                
        QList<QNetworkCookie> cookies = QNetworkCookie::parseCookies(
            out.readAll().toUtf8());
        QVariant var;                                                          
        var.setValue(cookies);                                                 
        request.setHeader(QNetworkRequest::CookieHeader, var);                 
    }

    m_file.setFileName(filePath);
    m_file.open(isAppend ?
                QIODevice::WriteOnly | QIODevice::Append :
                QIODevice::WriteOnly);
    
    m_sslErrorConnection = connect(&m_nam, &QNetworkAccessManager::sslErrors, 
            [this](QNetworkReply* reply, const QList<QSslError> & errors) {
                reply->ignoreSslErrors(errors);
            });
    
    m_reply = m_nam.get(request);

    connect(m_reply,
            static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            [this](QNetworkReply::NetworkError code) {
                Q_UNUSED(code);
                Q_EMIT error();
            });
    m_downloadProgressConnection = connect(m_reply, &QNetworkReply::downloadProgress, 
            [this](qint64 bytesReceived, qint64 bytesTotal) {
                if (bytesTotal)
                    Q_EMIT downloaded(bytesReceived / bytesTotal);
            });
    if (isAppend) {
        m_readyReadConnection = connect(m_reply, &QNetworkReply::readyRead,
            [this] {
                m_file.write(m_reply->readAll());
                m_file.flush();
            });
    } else {
        connect(m_reply, &QNetworkReply::finished, [this] {
                m_file.write(m_reply->readAll());
                m_file.flush();
                m_file.close();
                Q_EMIT finished();
            });
    }
    m_finishConnection = connect(&m_nam, &QNetworkAccessManager::finished,
            [=] {
                if (isAppend)
                    Q_EMIT finished();

                disconnect(m_downloadProgressConnection);
                disconnect(m_readyReadConnection);
                disconnect(m_sslErrorConnection);
                disconnect(m_finishConnection);
            });
}
