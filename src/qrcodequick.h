// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef QRCODE_H
#define QRCODE_H

#include <Prison/Prison>

#include <QtQuick/QQuickPaintedItem>
#include <QPainter>

using namespace Prison;

class QRcodeQuick : public QQuickPaintedItem 
{
    Q_OBJECT

    Q_PROPERTY(QString code READ code WRITE setCode NOTIFY codeChanged)

public:
    explicit QRcodeQuick(QQuickItem* parent = Q_NULLPTR);
    ~QRcodeQuick();

    QString code() const;
    void setCode(const QString& code);

Q_SIGNALS:
    void codeChanged();

protected:
    void paint(QPainter* painter);

private:
    QString m_code;
    AbstractBarcode* m_barcode;
};

#endif // QRCODE_H
