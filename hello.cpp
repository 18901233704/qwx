#include <iostream>

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
        std::cout << "ERROR: fail to getcwd " << strerror(errno) << std::endl;
    }

    handle = dlopen(argv[1] ? argv[1] : "libwechatnetwork.so", RTLD_LAZY);
    if (!handle) {
        error = dlerror();
        std::cout << "ERROR: fail to dlopen " << error << std::endl;
        goto cleanup;
    }

cleanup:
    if (handle) {
        dlclose(handle);
        handle = NULL;
    }

    return 0;
}
