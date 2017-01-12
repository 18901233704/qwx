kwx
---

WeChat KDE frontend using KF5
微信 KDE 前端，使用[KF5](https://en.wikipedia.org/wiki/KDE_Frameworks_5)，基于[网页微信协议](https://github.com/Urinx/WeixinBot/blob/master/README.md)

images and protocol ***Copyright (C) by [Tencent](http://weixin.qq.com/)*** 
图片、协议***版权归[腾讯](http://weixin.qq.com/) 所有！***

## Build && Install 编译、运行

### Dependences for ArchLinux

```
sudo pacman -S cmake qt5-base qt5-declarative qt5-quickcontrols qt5-graphicaleffects ki18n kdbusaddons kxmlgui kdeclarative prison(OPTIONAL) openmp(OPTIONAL) llvm(OPTIONAL) 
```

### Build for Analyzer
```
mkdir scan-build
cd scan-build
scan-build -k -v cmake .. -DCMAKE_INSTALL_PREFIX=/usr    \
    -DCMAKE_BUILD_TYPE=Debug
scan-build -k -v -v -v -V make -j4
```

### Build for Sanitizer
```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr    \
    -DCMAKE_CXX_COMPILER=clang++    \
    -DECM_ENABLE_SANITIZERS='address;leak;undefined'    \
    -DCMAKE_BUILD_TYPE=Debug
make -j4
lldb ./src/kwx 
r
```

