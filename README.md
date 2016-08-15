反向weixinxxx.apk
------------------

***请不要用于商业用途，版权归腾讯所有！***


## 依赖 for ArchLinux

```
yaourt -S apktool
```

apktool是反向apk的工具。


```
wget android-ndk32-r10-linux-x86_64.tar.bz2
```

android-ndk其中的工具链，至少需要***arm-linux-androideabi-objdump***


## 反向apk

```
apktool d weixin610android540.apk
```

可以通过修改smali/com/tencent/mm/ui/来just for fun ;-)

而重要的逻辑层全部被封装成库lib/armeabi

```
arm-linux-androideabi-objdump -d lib/armeabi/libwechatnetwork.so > libwechatnetwork.asm
```

可以发现例如***Java_com_tencent_mm_network_Java2C_getNetworkServerIp***函数入口
借助了java2c把Java项目“翻译”成了C/C++生成动态链接库，以防止反向工程？


## 借助 qemu-arm 二进制翻译 x86 架构

qemu-arm -L /usr/arm-linux-gnueabi/ [adrld](https://bitbucket.org/jigsaw_echo/adrld) ./hello 

不同的dynamic loader：

### /system/bin/linker

```
readelf --program-headers ./hello 

Elf 文件类型为 EXEC (可执行文件)
入口点 0x82d0
共有 7 个程序头，开始于偏移量52

程序头：
  類型           偏移量   虛擬位址   實體位址 檔案大小 記憶大小 旗標 對齊
  PHDR           0x000034 0x00008034 0x00008034 0x000e0 0x000e0 R   0x4
  INTERP         0x000114 0x00008114 0x00008114 0x00013 0x00013 R   0x1
      [Requesting program interpreter: /system/bin/linker]
  LOAD           0x000000 0x00008000 0x00008000 0x00390 0x00390 R E 0x1000
  LOAD           0x000ee4 0x00009ee4 0x00009ee4 0x0011c 0x00120 RW  0x1000
  DYNAMIC        0x000ef4 0x00009ef4 0x00009ef4 0x000e0 0x000e0 RW  0x4
  GNU_STACK      0x000000 0x00000000 0x00000000 0x00000 0x00000 RW  0
  GNU_RELRO      0x000ee4 0x00009ee4 0x00009ee4 0x0011c 0x0011c RW  0x4

 區段到節區映射中:
  段节...
   00     
   01     .interp 
   02     .interp .dynsym .dynstr .hash .gnu.version .gnu.version_r .rel.plt .plt .text .rodata .note.android.ident 
   03     .fini_array .init_array .preinit_array .dynamic .got .bss 
   04     .dynamic 
   05     
   06     .fini_array .init_array .preinit_array .dynamic .got
```

### /lib64/ld-linux-x86-64.so.2 

```
readelf --program-header /usr/bin/ls
```

如何修改ELF的.interp段？
