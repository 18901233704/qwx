// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import cn.com.isoft.qwx 1.0

Item {
    id: rootWindow
	width: 480; height: 822

    StackView {
        id: rootWindowStackView
        anchors.fill: parent
		initialItem: SplashView {}
    }
}
