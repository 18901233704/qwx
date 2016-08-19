#include <stdio.h>
#include <jni.h>
#include <dlfcn.h>

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

jstring (*Java_com_tencent_mm_network_Java2C_getNetworkServerIp)(JNIEnv*, jclass);

JNIEXPORT jstring Java_cn_leetcode_hellowechat_WechatWrapper_getNetworkServerIp(JNIEnv* env, jobject thiz) 
{
    void* handle = NULL;
    char *error = NULL;
    jstring ret;

    try {
    handle = dlopen("libwechatnetwork.so", RTLD_LAZY);
    if (handle) {
        error = (char*) dlerror();
        Java_com_tencent_mm_network_Java2C_getNetworkServerIp = (jstring (*)(JNIEnv*, jclass)) dlsym(handle, "Java_com_tencent_mm_network_Java2C_getNetworkServerIp");
        error = (char*) dlerror();
        if (!error)
            ret = (*Java_com_tencent_mm_network_Java2C_getNetworkServerIp)(env, (jclass) thiz);
        else
            ret = env->NewStringUTF("fail to dlsym");
        dlclose(handle);
        handle = NULL;
    } else {
        ret = env->NewStringUTF("fail to dlopen");
    }
    } catch (...) {
        ret = env->NewStringUTF("Oops!");
    }
    return ret;
}

#ifdef __cplusplus
}
#endif
