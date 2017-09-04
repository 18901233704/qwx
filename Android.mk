APP := hello
ROOT := /data/download
INSTALL_DIR := /home/xiangzhai/tmp
#NDK_PLATFORM_VER := 19 # 4.4.2
NDK_PLATFORM_VER := 22

ANDROID_NDK_ROOT := $(ROOT)/android-ndk-r14b
ANDROID_NDK_HOST := linux-x86_64
ANDROID_SDK_ROOT := $(ROOT)/android-sdk-linux
PREBUILD := $(ANDROID_NDK_ROOT)/toolchains/arm-linux-androideabi-4.9/prebuilt/$(ANDROID_NDK_HOST)

BIN := $(PREBUILD)/bin
LIB := $(ANDROID_NDK_ROOT)/platforms/android-$(NDK_PLATFORM_VER)/arch-arm/usr/lib
INCLUDE := $(ANDROID_NDK_ROOT)/platforms/android-$(NDK_PLATFORM_VER)/arch-arm/usr/include

CC := $(BIN)/arm-linux-androideabi-gcc
CXX := $(BIN)/arm-linux-androideabi-g++
GDB_CLIENT := $(BIN)/arm-linux-androideabi-gdb

LIBCRT := $(LIB)/crtbegin_dynamic.o

LINKER := /system/bin/linker

DEBUG := -g

CFLAGS := $(DEBUG) -fno-short-enums -fPIC -I$(INCLUDE) -I$(ANDROID_NDK_ROOT)/sources/cxx-stl/stlport/stlport
CFLAGS += -DQWX_ANDROID -DINSTALL_DIR=$(INSTALL_DIR)
CFLAGS += -Wl,-rpath-link=$(LIB),-dynamic-linker=$(LINKER) -L$(LIB) -L$(ANDROID_NDK_ROOT)/sources/cxx-stl/stlport/libs/armeabi
CFLAGS += -nostdlib -lc -ldl -lstlport_shared

all: hello hello-cpp libLeslie.so libWechatWrapper.so

hello: hello.c
	$(CC) -o $@ $< $(CFLAGS) $(LIBCRT)

hello-cpp: hello.cpp
	$(CXX) -o $@ $< $(CFLAGS) $(LIBCRT)

libLeslie.so: libLeslie.cpp
	$(CXX) -o $@ $< $(CFLAGS) -shared

libWechatWrapper.so: libWechatWrapper.cpp
	$(CXX) -o $@ $< $(CFLAGS) -shared

install: $(APP)
	$(ANDROID_SDK_ROOT)/platform-tools/adb push $(APP) $(INSTALL_DIR)/$(APP)
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell chmod 777 $(INSTALL_DIR)/$(APP)

shell:
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell

run:
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell $(INSTALL_DIR)/$(APP)

debug-install:
	$(ANDROID_SDK_ROOT)/platform-tools/adb push $(PREBUILD)/../gdbserver $(INSTALL_DIR)/gdbserver
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell chmod 777 $(INSTALL_DIR)/gdbserver

debug-go:
	$(ANDROID_SDK_ROOT)/platform-tools/adb forward tcp:1234: tcp:1234
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell $(INSTALL_DIR)/gdbserver :1234 $(INSTALL_DIR)/$(APP)

debug:
	$(GDB_CLIENT) $(APP)

clean:
	@rm -f *.o hello hello-cpp libLeslie.so libWechatWrapper.so
