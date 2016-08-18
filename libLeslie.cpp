#include <stdio.h>

#define LIBLESLIE_EXPORTED __attribute__((__visibility__("default")))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef QWX_ANDROID
int main(int argc, char* argv[]) 
{
    printf("DEBUG: %s, %s, line %d\n", __FILE__, __func__, __LINE__);
    return 0;
}
#endif

void LIBLESLIE_EXPORTED sayHello(int argc, char* argv[]) 
{
    printf("Hello World by %s\n", argv[1] ? argv[1] : "Leslie");
}

#ifdef __cplusplus
}
#endif
