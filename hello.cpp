#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <dlfcn.h>

int main(int argc, char *argv[]) 
{
    char buf[PATH_MAX] = { '\0' };
    void *handle = NULL;
    char *error = NULL;

    if (getcwd(buf, sizeof(buf)) == NULL) {
        printf("ERROR: fail to getcwd %s\n", strerror(errno));
    }

    handle = dlopen(argv[1] ? argv[1] : "libwechatnetwork.so", RTLD_LAZY);
    if (!handle) {
        error = dlerror();
        printf("ERROR: fail to dlopen %s\n", error);
        goto cleanup;
    }

cleanup:
    if (handle) {
        dlclose(handle);
        handle = NULL;
    }

    return 0;
}
