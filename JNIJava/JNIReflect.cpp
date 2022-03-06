#include "Stdafx.h"
#include <BWUtils/CString.h>
#include <JNIJava/JStringHelper.h>
#include <JNIJava/JNIReflect.h>

namespace buwai {

jfieldID BWGetFieldID(JNIEnv* env, const char* classDesc, const char* fieldName, const char* sig) {
    Lsr<jclass> clazz(env, env->FindClass(classDesc));
    if (NULL == clazz.Get()) {
        return NULL;
    }

    return env->GetFieldID(clazz.Get(), fieldName, sig);
}

jfieldID BWGetFieldID(JNIEnv* env, jobject obj, const char* fieldName, const char* sig) {
    Lsr<jclass> clazz(env, env->GetObjectClass(obj));
    if (NULL == clazz.Get()) {
        return NULL;
    }
    return env->GetFieldID(clazz.Get(), fieldName, sig);
}

jfieldID BWGetStaticFieldID(JNIEnv* env, const char* classDesc, const char* fieldName, const char* sig) {
    Lsr<jclass> clazz(env, env->FindClass(classDesc));
    if (NULL == clazz.Get()) {
        return NULL;
    }

    return env->GetStaticFieldID(clazz.Get(), fieldName, sig);
}

jint BWGetStaticIntField(JNIEnv* env, const char* classDesc, const char* fieldName, const char* sig) {
    jfieldID fieldID = BWGetStaticFieldID(env, classDesc, fieldName, sig);
    Lsr<jclass> clazz(env, env->FindClass(classDesc));
    if (NULL == clazz.Get()) {
        return -1;
    }
    return env->GetStaticIntField(clazz.Get(), fieldID);
}

// ��÷���ID��
jmethodID BWGetMethodID(JNIEnv* env, jobject obj, const char* methodName, const char* sig) {
	jclass clazz = env->GetObjectClass(obj);
	if (NULL == clazz) {
		return NULL;
	}
	jmethodID id = env->GetMethodID(clazz, methodName, sig);
	env->DeleteLocalRef(clazz);
	return id;
}

// ��÷���ID��
jmethodID BWGetMethodID(JNIEnv* env, const char* className, const char* methodName, const char* sig) {
	jclass clazz = env->FindClass(className);
	if (NULL == clazz) {
		return NULL;
	}
	jmethodID id = env->GetMethodID(clazz, methodName, sig);
	env->DeleteLocalRef(clazz);
	return id;
}

jmethodID BWGetMethodID(JNIEnv* env, jobject obj, jstring methodName, jstring sig) {
    CString cmethodName = JStringToCString(env, methodName);
    if ("" == cmethodName) {
        return NULL;
    }
    CString csig = JStringToCString(env, sig);
    if ("" == csig) {
        return NULL;
    }
    return BWGetMethodID(env, obj, cmethodName.GetCString(), csig.GetCString());
}

jmethodID BWGetMethodID(JNIEnv* env, jstring className, jstring methodName, jstring sig) {
    CString cclassName = JStringToCString(env, className);
    if ("" == cclassName) {
        return NULL;
    }
    CString cmethodName = JStringToCString(env, methodName);
    if ("" == cmethodName) {
        return NULL;
    }
    CString csig = JStringToCString(env, sig);
    if ("" == csig) {
        return NULL;
    }
    return BWGetMethodID(env, cclassName.GetCString(), cmethodName.GetCString(), csig.GetCString());
}

// ��þ�̬����ID��
jmethodID BWGetStaticMethodID(JNIEnv* env, const char* className, const char* methodName, const char* sig) {
 	jclass clazz = env->FindClass(className);
 	if (NULL == clazz) {
 		return NULL;
 	}
 	jmethodID id = env->GetStaticMethodID(clazz, methodName, sig);
 	env->DeleteLocalRef(clazz);
 	return id;
}

//////////////////////////////////////////////////////////////////////////

void BWCallVoidMethod(JNIEnv* env, jobject obj, const char* methodName, const char* sig, ...) {
    jmethodID id = BWGetMethodID(env, obj, methodName, sig);
    if (NULL == id) return;

    va_list ap;
    va_start(ap, sig);
    env->CallVoidMethodV(obj, id, ap);
    va_end(ap);
}

// ����Java������
jobject BWCallObjectMethod(JNIEnv* env, jobject obj, const char* methodName, const char* sig, ...) {
	jmethodID id = BWGetMethodID(env, obj, methodName, sig);
	if (NULL == id) return NULL;

	va_list ap;
	va_start(ap, sig);
	jobject result = env->CallObjectMethodV(obj, id, ap);
	va_end(ap);

	return result;
}

void BWCallStaticVoidMethod(JNIEnv* env, const char* className, const char* methodName, const char* sig, ...) {
	jclass clazz = env->FindClass(className);
	if (NULL == clazz) {
		return;
	}

	jmethodID id = env->GetStaticMethodID(clazz, methodName, sig);
	if (NULL == id) return;

	va_list ap;
	va_start(ap, sig);
	env->CallStaticVoidMethodV(clazz, id, ap);
	va_end(ap);

	env->DeleteLocalRef(clazz);
}

jint BWCallStaticIntMethod(JNIEnv* env, const char* className, const char* methodName, const char* sig, ...) {
	jclass clazz = env->FindClass(className);
	if (NULL == clazz) {
		return 0;
	}

	jmethodID id = env->GetStaticMethodID(clazz, methodName, sig);
	if (NULL == id) return 0;

	va_list ap;
	va_start(ap, sig);
	jint result = env->CallStaticIntMethodV(clazz, id, ap);
	va_end(ap);

	env->DeleteLocalRef(clazz);
	return result;
}

// ����Java������
jobject BWCallStaticObjectMethod(JNIEnv* env, const char* className, const char* methodName, const char* sig, ...) {
	jclass clazz = env->FindClass(className);
	if (NULL == clazz) {
		return NULL;
	}

	jmethodID id = env->GetStaticMethodID(clazz, methodName, sig);
	if (NULL == id) return NULL;

	va_list ap;
	va_start(ap, sig);
	jobject result = env->CallStaticObjectMethodV(clazz, id, ap);
	va_end(ap);

	return result;
}

}
