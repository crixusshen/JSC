//
// Created by dfzq on 2019-05-23.
//

#ifndef MYJNIAPPLICATION_LOG_H
#define MYJNIAPPLICATION_LOG_H

#include <android/log.h>

#define TAG "NativeTag"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

#endif //MYJNIAPPLICATION_LOG_H
