// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

import QtQuick 2.2
import cn.com.isoft.qwx 1.0
import "global.js" as Global

Item {
    id: iView
    property QtObject qrcode: Qt.createQmlObject('import cn.com.isoft.qwx 1.0; QRcodeQuick { width: parent.width; height: parent.width }', iView, 'qrcodeObject');
    
    Component.onCompleted: {
        if (qrcode) {
            qrcode.code = Global.loginNickName;
        }
    }
}
