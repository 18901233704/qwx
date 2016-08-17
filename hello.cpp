#include <stdio.h>
#include <dlfcn.h>

void (*sayHello)(int, char**);
void (*sayHelloNoP)();

int main(int argc, char *argv[]) 
{
    void *handle = NULL;
    char *error = NULL;

    printf("Hello World\n");

    handle = dlopen(argv[1] ? argv[1] : "libLeslie.so", RTLD_LAZY);
    if (!handle) {
        error = dlerror();
        printf("ERROR: fail to dlopen %s\n", error);
        goto cleanup;
    }

    sayHello = (void (*)(int, char**)) dlsym(handle, "sayHello");
    error = dlerror();
    if (error) {
        printf("ERROR: fail to dlsym %s\n", error);
        goto cleanup;
    }
    (*sayHello)(argc, argv);

    sayHelloNoP = (void (*)()) dlsym(handle, "sayHello");
    error = dlerror();
    if (error) {
        printf("ERROR: fail to dlsym %s\n", error);
        goto cleanup;
    }
    (*sayHelloNoP)();

cleanup:
    if (handle) {
        dlclose(handle);
        handle = NULL;
    }

    return 0;
}
