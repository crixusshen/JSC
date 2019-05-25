#include <jni.h>
#include <string>
#include "Log.h"
#include "JavaScriptCore/JavaScript.h"
#include "Calculation.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myjniapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    LOGD("stringFromJNI_Value: %s", hello.c_str());
    return env->NewStringUTF(hello.c_str());
}extern "C"
JNIEXPORT jdouble JNICALL
Java_com_example_myjniapplication_MainActivity_calculation(JNIEnv *env, jobject instance,
                                                           jstring jsBuffer_) {
    const char *jsBuffer = env->GetStringUTFChars(jsBuffer_, 0);

    JSValueRef exception = nullptr;
    // JS的执行环境
    JSGlobalContextRef executor_context_ = ::JSGlobalContextCreate(nullptr);
    // 将char字符转化为缓冲字符串类型
    JSStringRef script = ::JSStringCreateWithUTF8CString(jsBuffer);
    // JS和JAVA之间的转化类型 - JSValueRef
    JSValueRef value = ::JSEvaluateScript(executor_context_, script, 0, 0, 0, &exception);
    // 判断计算值是否是数值，包括整形或浮点型
    if (JSValueIsNumber(executor_context_, value)) {
        double result = JSValueToNumber(executor_context_, value, nullptr);
        LOGD("JSValueIsNumber: %f", result);
        return result;
    }
    // 释放字符占用的内存空间
    ::JSStringRelease(script);
    return -1;
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjniapplication_MainActivity_callObject(JNIEnv *env, jobject instance) {
    Calculation calculation;
    calculation.runScript("calculation.num=99;calculation.testStatic();calculation.testDynamic();console.log('num is:'+calculation.num)");
}