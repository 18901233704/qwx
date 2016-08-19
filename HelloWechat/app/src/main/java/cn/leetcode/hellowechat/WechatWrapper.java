package cn.leetcode.hellowechat;

public class WechatWrapper {
    public native String getNetworkServerIp();

    static {
        /**
         * TODO: please build libLeslie.so shared library at first!
         *
         * cd ./qwx
         * make -f Android.mk
         * cp libWechatWrapper.so HelloWechat/app/src/main/jniLibs/armeabi
         * cp $(ANDROID_NDK_ROOT)/sources/cxx-stl/stlport/libs/armeabi/libstlport_shared.so HelloWechat/app/src/main/jniLibs/armeabi
         */
        System.loadLibrary("WechatWrapper");
    }
}
