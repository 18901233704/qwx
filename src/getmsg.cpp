// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <KLocalizedString>

#include <QFile>
#include <QJsonDocument>                                                           
#include <QJsonObject>                                                             
#include <QJsonArray>
#include <QStandardPaths>
#include <time.h>

#include "getmsg.h"
#include "download.h"
#include "globaldeclarations.h"

GetMsg::GetMsg(HttpPost* parent) 
  : HttpPost(parent)
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

GetMsg::~GetMsg() 
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void GetMsg::setFromUserName(const QString & fromUserName) 
{
    if (m_fromUserName != fromUserName) {
        m_fromUserName = fromUserName;
        Q_EMIT fromUserNameChanged();
    }
}

void GetMsg::setToUserName(const QString & toUserName) 
{
    if (m_toUserName != toUserName) {
        m_toUserName = toUserName;
        Q_EMIT toUserNameChanged();
    }
}

void GetMsg::setNeedSaveLog(bool needSaveLog) 
{
    if (m_needSaveLog != needSaveLog) {
        m_needSaveLog = needSaveLog;
        Q_EMIT needSaveLogChanged();
    }
}

void GetMsg::m_post(QString host, 
                    QString uin, 
                    QString sid, 
                    QString skey, 
                    QStringList syncKey) 
{
    m_skey = skey;
    QString ts = QString::number(time(NULL));
    QString url = host + WX_CGI_PATH + "webwxsync?sid=" + sid + 
        "&skey=" + m_skey + "&r=" + ts;
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    QString json = "{\"BaseRequest\":{\"Uin\":" + uin + ",\"Sid\":\"" + sid + 
        "\"},\"SyncKey\":{\"Count\":" + QString::number(syncKey.size()) + 
        ",\"List\":[";
    for (int i = 0; i < syncKey.size(); i++) {
        if (i != 0)
            json += ",";
        QStringList result = syncKey[i].split("|");
        json += "{\"Key\":" + result[0] + ",\"Val\":" + result[1] + "}";
    }
    json += "]},\"rr\":" + ts + "}";
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << json;
#endif
    HttpPost::post(url, json, true);
}

void GetMsg::post(QString uin, QString sid, QString skey, QStringList syncKey) 
{
    m_post(WX_SERVER_HOST, uin, sid, skey, syncKey);
}

void GetMsg::postV2(QString uin, QString sid, QString skey, QStringList syncKey)
{
    m_v2 = true;
    m_post(WX_V2_SERVER_HOST, uin, sid, skey, syncKey);
}

QString GetMsg::contentWithoutUserName(QString content) 
{
    if (content.startsWith("@"))
        content = content.mid(content.indexOf(":<br/>") + QString(":<br/>").size());

    if (content.contains("<img src="))
        content = i18n("[Picture]");

    return content;
}

QString GetMsg::contentToUserName(QString content, QString oriUserName) 
{
    if (content.startsWith("@")) 
        return content.left(content.indexOf(":<br/>"));

    return oriUserName;
}

void GetMsg::m_saveLog(QString createTimeStr, QString fromUserName, QString content) 
{
    QFile file(QWXDIR + "/" + fromUserName + ".txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << createTimeStr << DELIM << fromUserName << DELIM << content << "\n";
        file.close();
    }
}

void GetMsg::m_handleNewMsg(QString msgId,
                            QString content,
                            QString fromUserNameStr,
                            QString toUserNameStr,
                            int createTime)
{
    if (msgId.isEmpty() || content.isEmpty())
        return;

    QString createTimeStr = QString::number(createTime);

    if (!m_map.contains(msgId)) {
        if (m_needSaveLog)
            m_saveLog(createTimeStr, fromUserNameStr, content);

        Q_EMIT newMsg(content, fromUserNameStr, toUserNameStr);
    }

    if ((fromUserNameStr == m_fromUserName && toUserNameStr == m_toUserName) ||
        (fromUserNameStr == m_toUserName && toUserNameStr == m_fromUserName)) {
        if (!m_map.contains(msgId))
            Q_EMIT received(content, fromUserNameStr);
    }

    if (m_map.size() > 64) {
        while (m_map.size() > 64 / 2)
            m_map.erase(m_map.begin());
    }

    m_map.insert(msgId, createTime);
}

void GetMsg::finished(QNetworkReply* reply) 
{
    QString replyStr = QString(reply->readAll());
#if QWX_DEBUG
    QFile file("getmsg.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << replyStr;
        file.close();
    }
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
    qDebug() << "DEBUG:" << replyStr;
#endif
    QJsonDocument doc = QJsonDocument::fromJson(replyStr.toUtf8());
    if (!doc.isObject()) { Q_EMIT error(); return; }
    QJsonObject obj = doc.object();
    if (obj["AddMsgCount"].toInt() == 0)
        Q_EMIT noNewMsg();

    Q_FOREACH (const QJsonValue & val, obj["AddMsgList"].toArray()) {
        QJsonObject msg = val.toObject();
        QString fromUserNameStr = msg["FromUserName"].toString();
        QString toUserNameStr = msg["ToUserName"].toString();
        int createTime = msg["CreateTime"].toInt();
        QString content = msg["Content"].toString();
        QString msgId = msg["MsgId"].toString();
        int msgType = msg["MsgType"].toInt();

        if (content.contains("msg")) {
            content = "";
            if (msgType == 3) {
                QString url = m_v2 ? WX_V2_SERVER_HOST : WX_SERVER_HOST +
                    WX_CGI_PATH + "webwxgetmsgimg?MsgID=" + msgId + "&skey=" +
                    m_skey;
                QString msgImgPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/img_" + msgId + ".jpg";
                Download *downLoad = new Download;
                downLoad->get(url, msgImgPath, true, false);
                connect(downLoad, &Download::finished, [=] {
                    content = "<img src=\"file://" + msgImgPath +
                        "\" width=\"128\" height=\"128\">";
                    m_handleNewMsg(msgId, content, fromUserNameStr,
                        toUserNameStr, time(nullptr));
                    downLoad->deleteLater();
                });
            } else if (msgType == 34) {
                QString url = m_v2 ? WX_V2_SERVER_HOST : WX_SERVER_HOST +
                    WX_CGI_PATH + "webwxgetvoice?msgid=" + msgId + "&skey=" +
                    m_skey;
                QString msgVoicePath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/voice_" + msgId + ".mp3";
                Download *downLoad = new Download;
                downLoad->get(url, msgVoicePath, true, false);
                connect(downLoad, &Download::finished, [=] {
                    content = "<a href=\"file://" + msgVoicePath + "\">" +
                        i18n("Voice") + "</a>";
                    m_handleNewMsg(msgId, content, fromUserNameStr,
                        toUserNameStr, time(nullptr));
                    downLoad->deleteLater();
                });
            } else if (msgType == 62) {
                QString url = m_v2 ? WX_V2_SERVER_HOST : WX_SERVER_HOST +
                    WX_CGI_PATH + "webwxgetvideo?msgid=" + msgId + "&skey=" +
                    m_skey;
                QString msgVideoPath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/video_" + msgId + ".mp4";
                Download *downLoad = new Download;
                downLoad->get(url, msgVideoPath, true, false);
                connect(downLoad, &Download::finished, [=] {
                    content = "<a href=\"file://" + msgVideoPath + "\">" +
                        i18n("Video") + "</a>";
                    m_handleNewMsg(msgId, content, fromUserNameStr,
                        toUserNameStr, time(nullptr));
                    downLoad->deleteLater();
                });
            } else if (msgType == 49) {
                content = i18n("Please view it on your phone") + " <a href=\"" +
                    msg["Url"].toString() + "\">" + msg["FileName"].toString() +
                    "</a>";
            } else if (msgType == 10002) {
                content = i18n("Withdraw a message");
            } else if (msgType == 51) {
                // TODO: you are tapping on your phone ;-)
            } else {
                content = i18n("Unsupport MsgType %1").arg(msgType);
            }
        }

        m_handleNewMsg(msgId, content, fromUserNameStr, toUserNameStr, createTime);
    }
    
    m_syncKey.clear();
    Q_FOREACH (const QJsonValue & val, obj["SyncKey"].toObject()["List"].toArray()) {
        m_syncKey.append(QString::number(val.toObject()["Key"].toInt()) + "|" + 
                QString::number(val.toObject()["Val"].toInt()));
    }
    if (m_syncKey.size() == 0) {
        QFile cookie(QWXDIR + "/" + COOKIE_FILENAME);
        cookie.remove();
    }
    Q_EMIT syncKeyChanged();
}
