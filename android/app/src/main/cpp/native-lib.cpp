#include <jni.h>
#include <android/log.h>
#include <string>


extern "C" JNIEXPORT jint JNICALL
jmain(JNIEnv* env, jobject thiz, jobjectArray args);

extern "C" JNIEXPORT jint JNICALL
Java_com_github_huangqinjin_glogtest_MainActivity_main(
        JNIEnv* env, jobject thiz, jobjectArray args)
{
    return jmain(env, thiz, args);
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_github_huangqinjin_glogtest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    __android_log_print(ANDROID_LOG_INFO, "native", "%s", hello.c_str());
    return env->NewStringUTF(hello.c_str());
}
