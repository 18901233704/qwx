// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

import QtQuick 2.2
import QtQuick.Controls 1.1
import cn.com.isoft.qwx 1.0
import "global.js" as Global

Rectangle {
    id: splashView
    width: parent.width; height: parent.height
    color: "#010510"

    Init {
		id: initObj
		onSkeyChanged: {
			if (skey != "") {
				Global.skey = skey;
            }
			Global.deviceId = initObj.deviceId;
			Global.loginUserName = initObj.loginUserName;
			Global.loginNickName = initObj.loginNickName;
			Global.initContactList = initObj.contactList;
			Global.initSyncKey = syncKey;
			rootWindowStackView.clear();
			rootWindowStackView.push({item: Qt.resolvedUrl("NavigatorView.qml")});
		}
	}

    QwxUUID {
        id: uuidObj
        onError: {
            console.log("ERROR: fail to get UUID!")
        }
        onCookieExists: {
            Global.uin = uin;
            Global.sid = sid;
            Global.ticket = ticket;
            Global.v2 = isV2;
			if (Global.v2) {
				initObj.postV2(Global.uin, Global.sid, Global.ticket)
			} else {
				initObj.post(Global.uin, Global.sid, Global.ticket)
			}
        }
        onUuidChanged: {
            rootWindowStackView.clear();
            rootWindowStackView.push({                                             
                item: Qt.resolvedUrl("LoginView.qml"),                         
                properties: {uuid: uuid}
            })
        }
    }

    Image {
        source: "../images/splash.jpg"
        width: parent.width; height: parent.height
    }
}
