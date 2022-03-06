#pragma once

#include <jni.h>
#include <CObject.h>
#include <BWUtils/CString.h>

namespace buwai {

/**
 * jstring类型转换为char*类型。
 * @param[in] env
 * @param[in] jstr 被转换的jstring类型字符串。
 * @return 返回转换后的字符串，若返回的类型。
 */
CString JStringToCString(JNIEnv* env, jstring jstr);

/**
 * 
 * @return 当不在使用时，用DeleteLocalRef释放。
 */
jstring CStringToJString(JNIEnv* env, const char* pStr);

}
