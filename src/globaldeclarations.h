// Copyright (C) 2014 - 2017 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef GLOBAL_DECLARATIONS_H
#define GLOBAL_DECLARATIONS_H

#include <QString>
#include <QDir>

const QString CODE_NAME = "kwx";

const QString LOGIN_SERVER_HOST = "https://login.weixin.qq.com";
const QString FILE_SERVER_HOST = "https://file.wx.qq.com";
const QString WX_SERVER_HOST = "https://wx.qq.com";
const QString WX_V2_SERVER_HOST = "https://wx2.qq.com";
const QString WX_CGI_PATH = "/cgi-bin/mmwebwx-bin/";
// TODO: When implemented my own robot skeleton?
const QString XIAODOUBI_URL = "http://api.douqq.com/?key=Vml5Z0pFZGk9UHg2a2dPY0loZW49S3cxN3dVQUFBPT0&msg=";
const QString QWXDIR = QDir::homePath() + "/." + CODE_NAME;
const QString COOKIE_FILENAME = ".cookie";
const QString DELIM = ";|";
const QString TN_WIDTH = "128";
const QString TN_HEIGHT = "128";
const QString VIDEO_FMT_SUF = ".mp4";

const QString IPLOOKUP_URL = "http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=json";
const QString WEATHER_URL = "http://api.map.baidu.com/telematics/v3/weather?location=%1&output=json&ak=6xxzcQMhb4WgKX0EUkwG747K"; // magic number ;-)

#endif // GLOBAL_DECLARATIONS_H
