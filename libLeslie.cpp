#include <stdio.h>
#ifdef QWX_ANDROID
#include <jni.h>
#include <iostream>
#else
#define LIBLESLIE_EXPORTED __attribute__((__visibility__("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef QWX_ANDROID
// TODO: JNI https://en.wikipedia.org/wiki/Java_Native_Interface
JNIEXPORT jstring Java_cn_leetcode_helloleslie_HelloLeslie_sayHello(JNIEnv* env, jobject thiz) 
{
    return env->NewStringUTF("Hello World by Leslie");
}

// TODO: it needs entry point when Android NDK dlopen this shared library
int main(int argc, char* argv[]) 
{
    std::cout << "DEBUG: " << __FILE__ << " " << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;
    return 0;
}
#else
void LIBLESLIE_EXPORTED sayHello(int argc, char* argv[]) 
{
    printf("Hello World by %s\n", argv[1] ? argv[1] : "Leslie");
}
#endif

#ifdef __cplusplus
}
#endif
