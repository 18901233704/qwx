// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>
// Copyright (c) 2010-2014 Sune Vuorela <sune@vuorela.dk>

#include "qrcodequick.h"

QRcodeQuick::QRcodeQuick(QQuickItem* parent)
    : QQuickPaintedItem(parent), 
      m_code(""), 
      m_barcode(Prison::createBarcode(Prison::QRCode))
{
}

QRcodeQuick::~QRcodeQuick() 
{
    delete m_barcode;
    m_barcode = Q_NULLPTR;
}

QString QRcodeQuick::code() const { return m_code; }
void QRcodeQuick::setCode(const QString& code) 
{
    if (m_code != code) {
        m_code = code;
        m_barcode->setData(m_code);
        Q_EMIT codeChanged();
        update();
    }
}

void QRcodeQuick::paint(QPainter* painter) 
{
    if (m_barcode) {
        QRect thisRect(0, 0, width(), height());
        QRect targetrect = thisRect;
        QImage image = m_barcode->toImage(targetrect.size());
        if (!image.isNull()) {
            QRectF rect(targetrect.left() + (targetrect.width() - image.size().width()) / 2,
                        targetrect.top() + (targetrect.height() - image.size().height()) / 2,
                        targetrect.size().width(),
                        targetrect.height());
            painter->drawImage(rect.topLeft(), image, image.rect());
        }
    }
}
