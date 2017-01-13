// Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <QFile>
#include <QDebug>

#include "emotion.h"

Emotion::Emotion(QObject* parent)
  : QAbstractListModel(parent) 
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
    QFile file(":/images/emotion.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QStringList result = in.readLine().split(" ");
            if (result.size() != 2)
                continue;

            addEmotion(EmotionObject(result[0], result[1]));
        }
    }
}

Emotion::~Emotion() 
{
#ifndef NDEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

QString Emotion::contentWithImage(QString content) 
{
    for (int i = 0; i < m_emotions.size(); i++) {
        QString replace = "[" + m_emotions[i].title() + "]";
        if (content.contains(replace)) {
            content.replace(replace, 
                    "<img src=\"../images/" + m_emotions[i].name() + "\">");
        }
    }
    return content;
}

void Emotion::addEmotion(const EmotionObject & emotion) 
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_emotions << emotion;
    endInsertRows();
}

int Emotion::rowCount(const QModelIndex & parent) const 
{
    Q_UNUSED(parent);
    return m_emotions.size();
}

QVariant Emotion::data(const QModelIndex & index, int role) const 
{
    if (index.row() < 0 || index.row() >= m_emotions.size())
        return QVariant();

    const EmotionObject & emotion = m_emotions[index.row()];
    if (role == TitleRole)
        return emotion.title();
    else if (role == NameRole)
        return emotion.name();
    return QVariant(); 
}
                                                                                
QHash<int, QByteArray> Emotion::roleNames() const 
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[NameRole] = "name";
    return roles;
}
