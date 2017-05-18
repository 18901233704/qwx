# kwx

WeChat [KDE frontend](http://www.leetcode.cn/2017/01/qwx.html) using KF5 base on [webwx protocol](https://github.com/Urinx/WeixinBot/blob/master/README.md).

## Build && Debug

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
scan-build -k -v -V make -j4
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
./src/kwx
```

