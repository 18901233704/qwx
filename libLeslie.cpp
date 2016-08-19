#include <stdio.h>
#ifdef QWX_ANDROID
#include <jni.h>
#endif

#define LIBLESLIE_EXPORTED __attribute__((__visibility__("default")))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef QWX_ANDROID
jstring Java_cn_leetcode_helloleslie_HelloLeslie_sayHello(JNIEnv* env, jobject thiz) 
{
    return env->NewStringUTF("Hello World by Leslie");
}

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
