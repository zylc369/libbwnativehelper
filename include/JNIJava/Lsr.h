#ifndef _LRSR_H_
#define _LRSR_H_

#include <stddef.h>
#include <stdint.h>
#include <jni.h>
#include <CObject.h>

namespace buwai {

//////////////////////////////////////////////////////////////////////////
//

// #define COMPARE(_op_)                                           \
//     inline bool operator _op_ (const Lsr<T>& o) const {              \
//     return mPtr _op_ o.mPtr;                                  \
// }

//////////////////////////////////////////////////////////////////////////
// 轻量级智能管理JNI本地引用。
// 传入的类型只能是能传入DeleteLocalRef函数的类型。
//
// 所谓本地引用，例如：env->FindClass(...)返回的就是一个本地引用。
// 错误用例1：
// jstring str;
// Lsr<jstring> a = str;
// Lsr<jstring> b = a.get(); // 不能这样赋值，因为这样赋值类里面的计数器就会不准了。

template <typename T>
class Lsr : public CObject {
public:
//    inline Lsr() : mPtr(0), mEnv(NULL), mpCount(NULL) { }

    Lsr(JNIEnv* env, T other);
    Lsr(const Lsr<T>& other);
    template<typename U> Lsr(JNIEnv* env, U other);
    template<typename U> Lsr(const Lsr<U>& other);

    ~Lsr();

    // Assignment

    //Lsr& operator = (T other);
    Lsr& operator = (const Lsr<T>& other);

    template<typename U> Lsr& operator = (const Lsr<U>& other);
    //template<typename U> Lsr& operator = (U other);

    //! Special optimization for use by ProcessState (and nobody else).
    //void force_set(T other);

    // Reset

    void Clear();

    // Accessors

    inline  T      operator* () const  { return mPtr; }
//    inline  T      operator-> () const { return mPtr;  }
    inline  T      Get() const         { return mPtr; }

    int32_t GetCount() const   { return NULL == mpCount ? 0 : *mpCount; }

    // Operators

    //     COMPARE(==)
    //     COMPARE(!=)
    //     COMPARE(>)
    //     COMPARE(<)
    //     COMPARE(<=)
    //     COMPARE(>=)

private:
    template<typename Y> friend class Lsr;
    //     template<typename Y> friend class sp;
    //     template<typename Y> friend class wp;
    //     void set_pointer(T ptr);
    T mPtr;
    //mutable volatile int32_t mCount;
    int32_t* mpCount;

    JNIEnv* mEnv;

    inline void IncStrong() const {
        __sync_fetch_and_add(mpCount, 1);
    }
    inline void DecStrong() {
        if (__sync_fetch_and_sub(mpCount, 1) == 1) {
            if (NULL != mEnv && NULL != mPtr) {
                mEnv->DeleteLocalRef(mPtr);
                mEnv = NULL;
            }
            mPtr = NULL;
            delete mpCount;
            mpCount = NULL;
        }
    }
};

// ---------------------------------------------------------------------------
// No user serviceable parts below here.

template<typename T>
Lsr<T>::Lsr(JNIEnv* env, T other)
: mPtr(other), mpCount(new int32_t(1)), mEnv(env)
{}

template<typename T>
Lsr<T>::Lsr(const Lsr<T>& other)
: mPtr(other.mPtr), mpCount(other.mpCount), mEnv(other.mEnv)
{
    if (mPtr) IncStrong();
}

template<typename T> template<typename U>
Lsr<T>::Lsr(JNIEnv* env, U other) : mPtr(other), mpCount(new int32_t(1)), mEnv(env)
{}

template<typename T> template<typename U>
Lsr<T>::Lsr(const Lsr<U>& other)
: mPtr(other.mPtr), mpCount(other.mpCount), mEnv(other.mEnv)
{
    if (mPtr) IncStrong();
}

template<typename T>
Lsr<T>::~Lsr()
{
    if (mPtr) DecStrong();
}

template<typename T>
Lsr<T>& Lsr<T>::operator = (const Lsr<T>& other) {
    T otherPtr(other.mPtr);
    if (otherPtr) other.IncStrong();
    if (mPtr) DecStrong();
    mpCount = other.mpCount;
    mPtr = otherPtr;
    mEnv = other.mEnv;

    return *this;
}

// template<typename T>
// Lsr<T>& Lsr<T>::operator = (T other)
// {
//     if (other) other->IncStrong(this);
//     if (mPtr) mPtr->DecStrong(this);
//     mPtr = other;
//     return *this;
// }

template<typename T> template<typename U>
Lsr<T>& Lsr<T>::operator = (const Lsr<U>& other) {
    T otherPtr(other.mPtr);
    if (otherPtr) other.IncStrong();
    if (mPtr) DecStrong();
    mpCount = other.mpCount;
    mPtr = otherPtr;
    mEnv = other.mEnv;

    return *this;
}

// template<typename T> template<typename U>
// Lsr<T>& Lsr<T>::operator = (U other)
// {
//     if (other) ((T)other)->IncStrong(this);
//     if (mPtr) mPtr->DecStrong(this);
//     mPtr = other;
//     return *this;
// }

// template<typename T>
// void Lsr<T>::force_set(T other)
// {
//     other->forceIncStrong(this);
//     mPtr = other;
// }

template<typename T>
void Lsr<T>::Clear()
{
    if (mPtr) {
        DecStrong();
        mPtr = NULL;
        mEnv = NULL;
        mpCount = NULL;
    }
}

// template<typename T>
// void Lsr<T>::set_pointer(T ptr) {
//     mPtr = ptr;
// }

}

#endif