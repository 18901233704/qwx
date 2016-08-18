package com.tencent.mm.network;

public class Java2C {
    public native String getNetworkServerIp();

    static {
        System.loadLibrary("wechatnetwork");
    }
}
