kwx
---

WeChat KDE frontend using KF5
微信 KDE 前端，使用[KF5](https://en.wikipedia.org/wiki/KDE_Frameworks_5)，基于[网页微信协议](https://github.com/Urinx/WeixinBot/blob/master/README.md)

images and protocol ***Copyright (C) by [Tencent](http://weixin.qq.com/)*** 
图片、协议***版权归[腾讯](http://weixin.qq.com/) 所有！***

## Build && Install 编译、运行

### Dependences for ArchLinux

```
sudo pacman -S qt5-base qt5-declarative qt5-quickcontrols qt5-graphicaleffects ki18n kdbusaddons kxmlgui kdeclarative 
```

### Dependences for Ubuntu 15.04+

```
sudo apt-get install qtdeclarative5-dev qt5-default qml-module-qtquick-controls qtdeclarative5-controls-plugin
```

### Build for Static Analyzing code
```
mkdir scan-build
cd scan-build
scan-build -k -v -V cmake .. -DCMAKE_INSTALL_PREFIX=/usr    \
    -DCMAKE_BUILD_TYPE=Debug
scan-build -k -v -V make -j 4
gdb ./src/kwx
r
```

## Robot 小逗比机器人
聊天状态下输入***小逗比出来***即可换出[聊天机器人](http://www.xiaodoubi.com/)
