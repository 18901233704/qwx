#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

#ifdef QWX_ANDROID
#include <iostream>
#include <jni.h>
#endif

void (*sayHello)(int, char**);

#ifdef QWX_ANDROID
jstring (*Java_com_tencent_mm_network_Java2C_getNetworkServerIp)(JNIEnv*, jclass);
#endif

int main(int argc, char* argv[])
{
    const char *libFilename = NULL;
    void* handle = NULL;
    char* error = NULL;

    libFilename = argv[1] ? argv[1] : "libwechatnetwork.so";
    handle = dlopen(libFilename, RTLD_LAZY);
    if (!handle) {
        error = (char*) dlerror();
        printf("ERROR: fail to dlopen %s\n", error);
        goto cleanup;
    }
    printf("DEBUG: lazy binding %s\n", libFilename);

    error = (char*) dlerror();

    sayHello = (void (*)(int, char**)) dlsym(handle, "sayHello");
    error = (char*) dlerror();
    if (!error) {
        printf("DEBUG: try to calling sayHello\n");
        (*sayHello)(argc, argv);
    }

#ifdef QWX_ANDROID
    Java_com_tencent_mm_network_Java2C_getNetworkServerIp = (jstring (*)(JNIEnv*, jclass)) dlsym(handle, "Java_com_tencent_mm_network_Java2C_getNetworkServerIp");
    error = (char*) dlerror();
    if (!error) {
        std::cout << "DEBUG: try to calling Java_com_tencent_mm_network_Java2C_getNetworkServerIp" << std::endl;
        // FIXME: How to obtain JNI interface pointer (JNIEnv *)
        // but it is of course able to System.loadLibrary for com/tencent/mm/network/Java2C class
        std::cout << "DEBUG: " << (*Java_com_tencent_mm_network_Java2C_getNetworkServerIp)(new JNIEnv, NULL) << std::endl;
    }
#endif

cleanup:
    if (handle) {
        dlclose(handle);
        handle = NULL;
    }
    printf("Bye ;-)\n");

    return 0;
}
