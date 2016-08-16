#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <dlfcn.h>

double (*cosine)(double);

int main(int argc, char *argv[]) 
{
    void *handle = NULL;
    char *error = NULL;

    handle = dlopen(argv[1] ? argv[1] : "/usr/arm-linux-gnu/lib/libm.so", RTLD_LAZY);
    if (!handle) {
        error = dlerror();
        printf("ERROR: fail to dlopen %s\n", error);
        goto cleanup;
    }

    cosine = (double (*)(double)) dlsym(handle, "cos");
    error = dlerror();
    if (error) {
        printf("ERROR: fail to dlsym %s\n", error);
        goto cleanup;
    }

    printf("cos(2.0) = %f\n", (*cosine)(2.0));

cleanup:
    if (handle) {
        dlclose(handle);
        handle = NULL;
    }

    return 0;
}
