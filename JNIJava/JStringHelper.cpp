#include "Stdafx.h"
#include <JNIJava/JStringHelper.h>

namespace buwai {

///////////////////////////////////////////////////////////

CString JStringToCString(JNIEnv* env, jstring jstr) {
    CString str;
    if (NULL == jstr) {
        return str;
    }

    jclass     jstrObj   = env->FindClass("java/lang/String");
    jstring    encode    = env->NewStringUTF("utf-8");
    jmethodID  methodId  = env->GetMethodID(jstrObj, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray byteArray = (jbyteArray)env->CallObjectMethod(jstr, methodId, encode);
    jsize      strLen    = env->GetArrayLength(byteArray);
    jbyte      *jBuf     = env->GetByteArrayElements(byteArray, JNI_FALSE);

    // BWLOGI("[*] %s - 0. str=%p, count=%d", __FUNCTION__, str.Get(), str.GetCount());

    if (jBuf != NULL) {
        // BWLOGI("[*] %s - 1. Lsp=%p, str=%p, count=%d", __FUNCTION__, &str, str.Get().Get(), str.Get().GetCount());
        str.Append((char*)jBuf, strLen);
        // BWLOGI("[*] %s - 2. str=%p, count=%d", __FUNCTION__, str.Get().Get(), str.Get().GetCount());
    }

    // BWLOGI("[*] %s - 3. str=%p, count=%d", __FUNCTION__, str.Get(), str.GetCount());
    env->ReleaseByteArrayElements(byteArray, jBuf, 0);
    // BWLOGI("[*] %s - 4. str=%s,%p, count=%d, strLen=%zu", __FUNCTION__, str.Get().Get(), str.Get().Get(),
    //     str.Get().GetCount(), strLen);
    return str;
}

jstring CStringToJString(JNIEnv* env, const char* pStr) {
    if (NULL == pStr) {
        return NULL;
    }
    int        strLen    = strlen(pStr);
    jclass     jstrObj   = env->FindClass("java/lang/String");
    jmethodID  methodId  = env->GetMethodID(jstrObj, "<init>", "([BLjava/lang/String;)V");
    jbyteArray byteArray = env->NewByteArray(strLen);
    jstring    encode    = env->NewStringUTF("utf-8");

    env->SetByteArrayRegion(byteArray, 0, strLen, (jbyte*)pStr);

    return (jstring)(env->NewObject(jstrObj, methodId, byteArray, encode));
}

}
