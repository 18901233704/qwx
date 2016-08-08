// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef GET_MSG_H
#define GET_MSG_H

#include "httppost.h"
#include "contact.h"

class GetMsg : public HttpPost 
{
    Q_OBJECT

    Q_PROPERTY(QString fromUserName READ fromUserName WRITE setFromUserName NOTIFY fromUserNameChanged)
    Q_PROPERTY(QString toUserName READ toUserName WRITE setToUserName NOTIFY toUserNameChanged)
    Q_PROPERTY(QStringList syncKey READ syncKey NOTIFY syncKeyChanged)
    Q_PROPERTY(bool needSaveLog READ needSaveLog WRITE setNeedSaveLog NOTIFY needSaveLogChanged)

public:
    explicit GetMsg(HttpPost* parent = Q_NULLPTR);
    ~GetMsg();

    QString fromUserName() const { return m_fromUserName; }
    void setFromUserName(const QString & fromUserName);

    QString toUserName() const { return m_toUserName; }
    void setToUserName(const QString & toUserName);
    
    QStringList syncKey() const { return m_syncKey; }

    bool needSaveLog() const { return m_needSaveLog; }
    void setNeedSaveLog(bool needSaveLog);

    Q_INVOKABLE void post(QString uin, 
                          QString sid, 
                          QString skey, 
                          QStringList syncKey);
    Q_INVOKABLE void postV2(QString uin, 
                            QString sid, 
                            QString skey, 
                            QStringList syncKey);
    Q_INVOKABLE QString contentWithoutUserName(QString content);
    Q_INVOKABLE QString contentToUserName(QString content, QString oriUserName);
    Q_INVOKABLE void notificationDBusCall(const QString &title, 
                                          const QString &body, 
                                          const QString &iconName = "qwx",
                                          bool persistent = false, 
                                          const QStringList &actions = QStringList());

Q_SIGNALS:
    void fromUserNameChanged();
    void toUserNameChanged();
    void error();
    void received(QString content, QString userName);
    void newMsg(QString content, QString fromUserName, QString toUserName);
    void syncKeyChanged();
    void noNewMsg();
    void needSaveLogChanged();

protected:
    void finished(QNetworkReply* reply);

private:
    void saveLog(QString createTimeStr, QString fromUserName, QString content);
    void postHandler(QString host, 
                     QString uin, 
                     QString sid, 
                     QString skey, 
                     QStringList syncKey);
    void handleNewMsg(QString msgId,
                      QString content,
                      QString fromUserNameStr,
                      QString toUserNameStr,
                      int createTime);

private:
    QString m_fromUserName = "";
    QString m_toUserName = "";
    QMap<QString, int> m_map;
    QStringList m_syncKey;
    bool m_needSaveLog = true;
    bool m_v2 = false;
    QString m_skey = "";
    Contact *m_contact = Q_NULLPTR;
};

#endif // GET_MSG_H
