#pragma once

#include <SmartPointer.h>
#include <jni.h>

namespace buwai {

/**
 * 获得字段ID。
 * @param[in] env JNI环境指针。
 * @param[in] classDesc 字段所属类的描述符。
 * @param[in] fieldName 字段名。
 * @param[in] sig 字段类型签名。
 * @return 获得成功，返回字段ID。获得失败，返回NULL。
 */
jfieldID BWGetFieldID(JNIEnv* env, const char* classDesc, const char* fieldName, const char* sig);

/**
 * 获得字段ID。
 * @param[in] env JNI环境指针。
 * @param[in] obj Java对象。
 * @param[in] fieldName 字段名。
 * @param[in] sig 字段类型签名。
 * @return 获得成功，返回字段ID。获得失败，返回NULL。
 */
jfieldID BWGetFieldID(JNIEnv* env, jobject obj, const char* fieldName, const char* sig);

jfieldID BWGetStaticFieldID(JNIEnv* env, const char* classDesc, const char* fieldName, const char* sig);

jint BWGetStaticIntField(JNIEnv* env, const char* classDesc, const char* fieldName, const char* sig);

/**
 * 获得方法ID。
 * @param[in] env JNI环境指针。
 * @param[in] obj Java对象。
 * @param[in] methodName 方法名。
 * @param[in] sig 方法签名。
 * @return 获得成功，返回方法ID。获得失败，返回NULL。
 */
jmethodID BWGetMethodID(JNIEnv* env, jobject obj, const char* methodName, const char* sig);

/**
 * 获得方法ID。
 * @param[in] env JNI环境指针。
 * @param[in] className 类名。如：com/androidtest/Test。
 * @param[in] methodName 方法名。
 * @param[in] sig 方法签名。
 * @return 获得成功，返回方法ID。获得失败，返回NULL。
 */
jmethodID BWGetMethodID(JNIEnv* env, const char* className, const char* methodName, const char* sig);

/**
 * 获得方法ID。
 * @param[in] env JNI环境指针。
 * @param[in] obj Java对象。
 * @param[in] methodName 方法名。
 * @param[in] sig 方法签名。
 * @return 获得成功，返回方法ID。获得失败，返回NULL。
 */
jmethodID BWGetMethodID(JNIEnv* env, jobject obj, jstring methodName, jstring sig);

/**
 * 获得方法ID。
 * @param[in] env JNI环境指针。
 * @param[in] className 类名。如：com/androidtest/Test。
 * @param[in] methodName 方法名。
 * @param[in] sig 方法签名。
 * @return 获得成功，返回方法ID。获得失败，返回NULL。
 */
jmethodID BWGetMethodID(JNIEnv* env, jstring className, jstring methodName, jstring sig);

/**
 * 将这个函数注释掉是因为静态方法调用的时候需要clazz的值，在本函数中已经获得过一次clazz。
 */
jmethodID BWGetStaticMethodID(JNIEnv* env, const char* className, const char* methodName, const char* sig);

void BWCallVoidMethod(JNIEnv* env, jobject obj, const char* methodName, const char* sig, ...);

/**
 * 
 * @param[in] 
 * @param[in] 
 * @param[in] 
 * @param[in] 
 * @param[in] 可变参数。
 */
jobject BWCallObjectMethod(JNIEnv* env, jobject obj, const char* methodName, const char* sig, ...);

/**
 *
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in] 可变参数。
 */
void BWCallStaticVoidMethod(JNIEnv* env, const char* className, const char* methodName, const char* sig, ...);

/**
 *
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in] 可变参数。
 */
jint BWCallStaticIntMethod(JNIEnv* env, const char* className, const char* methodName, const char* sig, ...);

/**
 *
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in] 可变参数。
 */
jobject BWCallStaticObjectMethod(JNIEnv* env, const char* className, const char* methodName, const char* sig, ...);

}
