// Copyright (C) 2014 - 2017 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <KLocalizedString>

#include <QFile>
#include <QJsonDocument>                                                           
#include <QJsonObject>                                                             
#include <QJsonArray>
#include <QStandardPaths>
#include <QDBusConnection>
#include <QDBusMessage>

#include <time.h>

#include "getmsg.h"
#include "download.h"
#include "globaldeclarations.h"

GetMsg::GetMsg(HttpPost* parent) 
    : HttpPost(parent)
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
    m_contact = new Contact;
    m_v2 ? m_contact->post() : m_contact->postV2();
}

GetMsg::~GetMsg() 
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
    delete m_contact;
    m_contact = Q_NULLPTR;
}

void GetMsg::notificationDBusCall(const QString &title, 
                                  const QString &body, 
                                  const QString &iconName,
                                  bool persistent, 
                                  const QStringList &actions)
{
    QDBusMessage dbusNotificationMessage = QDBusMessage::createMethodCall(
        "org.freedesktop.Notifications", "/org/freedesktop/Notifications", 
        "org.freedesktop.Notifications", QStringLiteral("Notify"));

    QList<QVariant> args;

    args.append(i18n("WeChat KDE frontend")); // app_name
    args.append((uint)0);  // notification to update
    args.append(iconName); // app_icon
    args.append(title); // summary
    args.append(body); // body

    QStringList actionList;
    int actId = 0;
    Q_FOREACH (const QString &actionName, actions) {
        actId++;
        actionList.append(QString::number(actId));
        actionList.append(actionName);
    }

    args.append(actionList); // actions

    args.append(QVariantMap()); // hints

    // Persistent     => 0  == infinite timeout
    // CloseOnTimeout => -1 == let the server decide
    int timeout = persistent ? 0 : -1;

    args.append(timeout); // expire timout

    dbusNotificationMessage.setArguments(args);

    QDBusMessage reply = QDBusConnection::sessionBus().call(dbusNotificationMessage, QDBus::Block, 4000);
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Error sending notification:" << reply.errorMessage();
    }
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

void GetMsg::postHandler(QString host, 
                         QString uin, 
                         QString sid, 
                         QString skey, 
                         QStringList syncKey) 
{
    m_skey = skey;
    QString ts = QString::number(time(NULL));
    QString url = host + WX_CGI_PATH + "webwxsync?sid=" + sid + 
        "&skey=" + m_skey + "&r=" + ts;
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    QString json = "{\"BaseRequest\":{\"Uin\":" + uin + ",\"Sid\":\"" + sid + 
        "\"},\"SyncKey\":{\"Count\":" + QString::number(syncKey.size()) + 
        ",\"List\":[";
    for (int i = 0; i < syncKey.size(); i++) {
        if (i != 0)
            json += ",";
        QStringList result = syncKey[i].split("|");
        if (!result.isEmpty())
            json += "{\"Key\":" + result[0] + ",\"Val\":" + result[1] + "}";
    }
    json += "]},\"rr\":" + ts + "}";
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << json;
#endif
    HttpPost::post(url, json, true);
}

void GetMsg::post(QString uin, QString sid, QString skey, QStringList syncKey) 
{
    postHandler(WX_SERVER_HOST, uin, sid, skey, syncKey);
}

void GetMsg::postV2(QString uin, QString sid, QString skey, QStringList syncKey)
{
    m_v2 = true;
    postHandler(WX_V2_SERVER_HOST, uin, sid, skey, syncKey);
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

void GetMsg::saveLog(QString createTimeStr, QString fromUserName, QString content) 
{
    QFile file(QWXDIR + "/" + fromUserName + ".txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << createTimeStr << DELIM << fromUserName << DELIM << content << "\n";
        file.close();
    }
}

void GetMsg::handleNewMsg(QString msgId,
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
            saveLog(createTimeStr, fromUserNameStr, content);

        notificationDBusCall(m_contact->getNickName(fromUserNameStr), content);
        Q_EMIT newMsg(content, fromUserNameStr, toUserNameStr);
    }

    if ((fromUserNameStr == m_fromUserName && toUserNameStr == m_toUserName) ||
        (fromUserNameStr == m_toUserName && toUserNameStr == m_fromUserName)) {
        if (!m_map.contains(msgId)) {
            notificationDBusCall(m_contact->getNickName(fromUserNameStr), content);
            Q_EMIT received(content, fromUserNameStr);
        }
    }

    if (m_map.size() > 64) { // magic number...
        while (m_map.size() > 64 / 2)
            m_map.erase(m_map.begin());
    }

    m_map.insert(msgId, createTime);
}

void GetMsg::finished(QNetworkReply* reply) 
{
    QString replyStr = QString(reply->readAll());
#ifndef NDEBUG
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
                    QString content = "<img src=\"file://" + msgImgPath +
                        "\" width=\"" + TN_WIDTH + "\" height=\"" + TN_HEIGHT + "\">";
                    handleNewMsg(msgId, content, fromUserNameStr,
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
                    QString content = "<a href=\"file://" + msgVoicePath + "\">" +
                        i18n("Voice") + "</a>";
                    handleNewMsg(msgId, content, fromUserNameStr,
                        toUserNameStr, time(nullptr));
                    downLoad->deleteLater();
                });
            } else if (msgType == 62) {
                QString url = m_v2 ? WX_V2_SERVER_HOST : WX_SERVER_HOST +
                    WX_CGI_PATH + "webwxgetvideo?msgid=" + msgId + "&skey=" +
                    m_skey;
                QString msgVideoPath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/video_" + msgId + VIDEO_FMT_SUF;
                Download *downLoad = new Download;
                downLoad->get(url, msgVideoPath, true, false);
                connect(downLoad, &Download::finished, [=] {
                    QString content = "<a href=\"file://" + msgVideoPath + "\">" +
                        i18n("Video") + "</a>";
                    handleNewMsg(msgId, content, fromUserNameStr,
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
                content = i18n("Unsupport MsgType %1", msgType);
            }
        }

        handleNewMsg(msgId, content, fromUserNameStr, toUserNameStr, createTime);
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
