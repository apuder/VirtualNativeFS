#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_org_puder_virtualnativefs_VirtualNativeFS_sayHello(JNIEnv *env, jclass type) {
    std::string hello = "Hello from NDK";
    return env->NewStringUTF(hello.c_str());
}
