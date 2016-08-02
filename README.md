qwx
----

WeChat Qt frontend 
微信Qt前端，基于[网页微信协议](https://github.com/Urinx/WeixinBot/blob/master/README.md)

images and protocol ***Copyright (C) by [Tencent](http://weixin.qq.com/)*** 
图片、协议***版权归[腾讯](http://weixin.qq.com/) 所有！***

## Build && Install 编译、运行

### Dependences for ArchLinux

```
sudo pacman -S qt5-base qt5-declarative qt5-tools qt5-quickcontrols qt5-graphicaleffects
```

### Dependences for Ubuntu 15.04+

```
sudo apt-get install qtdeclarative5-dev qt5-default qt5-qmake qml-module-qtquick-controls qtdeclarative5-controls-plugin
```

### Build for Debug
```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr    \
    -DCMAKE_BUILD_TYPE=Debug
make
./src/qwx
```

[RPM package spec file](https://github.com/isoft-linux/packages-extra/blob/master/qwx/qwx.spec)

## i18n also welcome other languages

```
lupdate src/*.cpp qml/*.qml -ts translations/zh_CN.ts
```

## Robot 小逗比机器人
Input ***robot come*** then here comes robot 聊天状态下输入***小逗比出来***即可换出[聊天机器人](http://www.xiaodoubi.com/)
