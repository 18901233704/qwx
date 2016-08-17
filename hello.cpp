#include <stdio.h>
#include <dlfcn.h>

void (*sayHello)(int, char**);

char* (*Java_com_tencent_mm_network_Java2C_getNetworkServerIp)();

int main(int argc, char *argv[]) 
{
    void *handle = NULL;
    char *error = NULL;

    handle = dlopen(argv[1] ? argv[1] : "/data/local/tmp/libwechatnetwork.so", RTLD_LAZY);
    if (!handle) {
        error = dlerror();
        printf("ERROR: fail to dlopen %s\n", error);
        goto cleanup;
    }
    printf("DEBUG: lazy binding libwechatnetwork.so\n");

    error = dlerror();

    Java_com_tencent_mm_network_Java2C_getNetworkServerIp = (char* (*)()) dlsym(handle, "Java_com_tencent_mm_network_Java2C_getNetworkServerIp");
    error = dlerror();
    if (error) {
        printf("ERROR: fail to dlsym %s\n", error);
        goto cleanup;
    }
    printf("DEBUG: try to calling Java_com_tencent_mm_network_Java2C_getNetworkServerIp\n");
    (*Java_com_tencent_mm_network_Java2C_getNetworkServerIp)();

cleanup:
    if (handle) {
        dlclose(handle);
        handle = NULL;
    }

    return 0;
}
