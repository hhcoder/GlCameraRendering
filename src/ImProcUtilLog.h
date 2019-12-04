#ifndef _IMPROCLOG_H_
#define _IMPROCLOG_H_

#ifdef ONTARGET

#ifdef LOG_NIDEBUG
#undef LOG_NIDEBUG
#endif
#define LOG_NIDEBUG 0

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "IMPROC"

#include <utils/Log.h>
#define IMPROC_LOGE			LOGE
#define IMPROC_LOGI			LOGI
#define IMPROC_LOGD			LOGD

#else //ANDROID-NDK

#include <android/log.h>

#define LOG_TAG "IMPROC"

#define IMPROC_LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define IMPROC_LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define IMPROC_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif

#endif //_IMPROCLOG_H_
