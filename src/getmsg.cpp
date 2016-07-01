// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <QFile>
#include <QJsonDocument>                                                           
#include <QJsonObject>                                                             
#include <QJsonArray>
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
        content = tr("[Picture]");

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

void GetMsg::m_handleNewMsg(QString content,
                            QString fromUserNameStr,
                            QString toUserNameStr,
                            int createTime)
{
    QString createTimeStr = QString::number(createTime);

    if (content.isEmpty())
        return;

    if (!m_map.contains(fromUserNameStr + toUserNameStr + createTimeStr)) {
        if (m_needSaveLog)
            m_saveLog(createTimeStr, fromUserNameStr, content);

        Q_EMIT newMsg(content, fromUserNameStr, toUserNameStr);
    }

    if ((fromUserNameStr == m_fromUserName && toUserNameStr == m_toUserName) ||
        (fromUserNameStr == m_toUserName && toUserNameStr == m_fromUserName)) {
        if (!m_map.contains(fromUserNameStr + toUserNameStr + createTimeStr))
            Q_EMIT received(content, fromUserNameStr);
    }

    if (m_map.size() > 64) {
        while (m_map.size() > 64 / 2)
            m_map.erase(m_map.begin());
    }

    m_map.insert(fromUserNameStr + toUserNameStr + createTimeStr, createTime);
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
                QString msgImgPath = QWXDIR + "/img_" + msgId + ".jpg";
                Download *downLoad = new Download;
                downLoad->get(url, msgImgPath, true, false);
                connect(downLoad, &Download::finished, [=] {
                    content = "<img src=\"file://" + msgImgPath +
                        "\" width=\"128\" height=\"128\">";
                    m_handleNewMsg(content, fromUserNameStr, toUserNameStr, time(nullptr));
                    downLoad->deleteLater();
                    qWarning() << "WARNING:" << __PRETTY_FUNCTION__ << msgId << m_skey;
                });
            } else if (msgType == 51) {
                // TODO: you are tapping on your phone ;-)
            } else {
                content = tr("Unsupport MsgType %1").arg(msgType);
            }
        }

        m_handleNewMsg(content, fromUserNameStr, toUserNameStr, createTime);
    }
    
    m_syncKey.clear();
    Q_FOREACH (const QJsonValue & val, obj["SyncKey"].toObject()["List"].toArray()) {
        m_syncKey.append(QString::number(val.toObject()["Key"].toInt()) + "|" + 
                QString::number(val.toObject()["Val"].toInt()));
    }
    Q_EMIT syncKeyChanged();
}
