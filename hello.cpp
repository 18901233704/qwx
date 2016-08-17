#include <stdio.h>
#include <dlfcn.h>

void (*sayHello)(int, char**);

int main(int argc, char *argv[]) 
{
    void *handle = NULL;
    char *error = NULL;

    printf("Hello World\n");
#ifdef ANDROID
    handle = dlopen(argv[1] ? argv[1] : "/data/local/tmp/libLeslie.so", RTLD_LAZY);
#else
    handle = dlopen(argv[1] ? argv[1] : "libLeslie.so", RTLD_LAZY);
#endif
    if (!handle) {
        error = dlerror();
        printf("ERROR: fail to dlopen %s\n", error);
        goto cleanup;
    }

    error = dlerror();

    sayHello = (void (*)(int, char**)) dlsym(handle, "sayHello");
    error = dlerror();
    if (error) {
        printf("ERROR: fail to dlsym %s\n", error);
        goto cleanup;
    }
    (*sayHello)(argc, argv);

cleanup:
    if (handle) {
        //dlclose(handle);
        handle = NULL;
    }

    return 0;
}
