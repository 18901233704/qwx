package com.tencent.mm.network;

public class Java2C {
    public native String getNetworkServerIp();

    static {
        // Put all shared libraries into /system/lib
        System.loadLibrary("libwechatnetwork.so");
    }
}
