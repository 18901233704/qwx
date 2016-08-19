package cn.leetcode.helloleslie;

public class HelloLeslie {
    public native String sayHello();

    static {
        /**
         * TODO: please build libLeslie.so shared library at first!
         *
         * cd ./qwx
         * make -f Android.mk
         * cp libLeslie.so HelloLeslie/app/src/main/jniLibs/armeabi
         */
        System.loadLibrary("Leslie");
    }
}
