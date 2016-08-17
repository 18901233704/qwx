#include <stdio.h>

#define LIBLESLIE_EXPORTED __attribute__((__visibility__("default")))

#ifdef __cplusplus
extern "C" {
#endif

int main(int argc, char* argv[]) 
{
    return 0;
}

void LIBLESLIE_EXPORTED sayHello(int argc, char* argv[]) 
{
    printf("Hello World by %s\n", argv[1] ? argv[1] : "Leslie");
}

#ifdef __cplusplus
}
#endif
