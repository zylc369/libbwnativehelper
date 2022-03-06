#ifndef BWSIMPLEJNI_SMARTPOINTER_H
#define BWSIMPLEJNI_SMARTPOINTER_H
#include <jni.h>
#include <android/log.h>
#include <stddef.h>
#include <stdint.h>
#include <CObject.h>

namespace buwai {

//////////////////////////////////////////////////////////////////////////
// 轻量级智能指针(Light Smart Point)。用于在堆中分配的内存。
// 错误用例1：
// int *i = new int(1);
// Lsp<int> a = i;
// Lsp<int> b = a.Get(); // 不能这样赋值，因为这样赋值类里面的计数器就会不准了。
//
// 使用Lsp智能指针的对象必须是"new"这种方法创建的指针。
// 
// 错误用例2：
// Lsp<char> str = new char[10];
// Lsp类无法释放数组。

template <typename T>
class Lsp : public CObject {
public:
    inline Lsp() : mPtr(NULL), mpCount(NULL) { }

    Lsp(T* other) : mPtr(other) {
        if (NULL != other) {
            mpCount = new int32_t(1);
        } else {
            mpCount = NULL;
        }
    }

    Lsp(const Lsp<T>& other): mPtr(other.mPtr), mpCount(other.mpCount) {
        if (mPtr) incStrong();
    }

    template<typename U>
    Lsp(U* other) : mPtr((char*)other) {
        if (NULL != other) {
            mpCount = new int32_t(1);
        } else {
            mpCount = NULL;
        }
    }

    template<typename U>
    Lsp(const Lsp<U>& other) : mPtr((T*)(other.mPtr)), mpCount(other.mpCount) {
        if (mPtr) incStrong();
    }

    ~Lsp() {
        if (mPtr) decStrong();
    }

    // Assignment

    Lsp& operator = (T* other) {
        // __android_log_print(ANDROID_LOG_INFO, "debug", "[*] %s - 1. mPtr=%p, mpCount=%p, other=%p", __FUNCTION__, mPtr, mpCount, other);
        // if (other) other->incStrong(this);
        if (mPtr) decStrong();
        // Lsp<T>(other);
        if (NULL != other) {
            mpCount = new int32_t(1);
        } else {
            mpCount = NULL;
        }
        mPtr = other;
        return *this;
    }

    Lsp& operator = (const Lsp<T>& other) {
        T* otherPtr(other.mPtr);
        if (otherPtr) other.incStrong();
        if (mPtr) decStrong();
        mpCount = other.mpCount;
        mPtr = otherPtr;

        return *this;
    }

    template<typename U>
    Lsp& operator = (const Lsp<U>& other) {
        T* otherPtr(other.mPtr);
        if (otherPtr) other.incStrong();
        if (mPtr) decStrong();
        mpCount = other.mpCount;
        mPtr = otherPtr;

        return *this;
    }

    // template<typename U>
    // Lsp& operator = (U* other) {
    //     if (other) ((T*)other)->incStrong(this);
    //     if (mPtr) mPtr->decStrong(this);
    //     mPtr = other;
    //     return *this;
    // }

    void Clear() {
        if (mPtr) {
            decStrong();
        }
    }

    // Accessors

    //    inline  T&      operator* () const  { return *mPtr; }
    // inline  T       operator[](int index) const { return mPtr[index]; }
    inline  T*      operator-> () const { return mPtr;  }
    inline  T*      Get() const         { return mPtr; }

    int32_t GetCount() const   { return NULL == mpCount ? 0 : *mpCount; }

    bool    IsEmpty() const { return NULL == mPtr ? true : false; }
    // Operators

    //     COMPARE(==)
    //     COMPARE(!=)
    //     COMPARE(>)
    //     COMPARE(<)
    //     COMPARE(<=)
    //     COMPARE(>=)

protected:
    template<typename Y> friend class Lsp;

    T* mPtr;
    //mutable volatile int32_t mCount;
    int32_t* mpCount;

    inline void incStrong() const {
        // __android_log_print(ANDROID_LOG_INFO, "debug", "[*] %s - 1. mPtr=%p, mpCount=%d", __FUNCTION__, mPtr, *mpCount);
        __sync_fetch_and_add(mpCount, 1);
        // __android_log_print(ANDROID_LOG_INFO, "debug", "[*] %s - 2. mPtr=%p, mpCount=%d", __FUNCTION__, mPtr, *mpCount);
    }
    inline void decStrong() {
        if (__sync_fetch_and_sub(mpCount, 1) == 1) {
            // __android_log_print(ANDROID_LOG_INFO, "debug", "[*] %s - mPtr=%p, mpCount=%d", __FUNCTION__, mPtr, *mpCount);
            if (NULL != mPtr) {
                delete static_cast<const T*>(mPtr);
                mPtr = NULL;
            }
            delete mpCount;
            mpCount = NULL;
        }
    }
};

/**
 * 轻量级智能指针。专用于管理数组指针。
 */
// template <typename T>
// class Lsap : public CObject {
// public:
//     inline Lsap() : mPtr(NULL), mpCount(NULL) { }

//     Lsap(T* other) : mPtr(other) {
//         if (NULL != other) {
//             mpCount = new int32_t(1);
//         } else {
//             mpCount = NULL;
//         }
//     }

//     Lsap(const Lsap<T>& other): mPtr(other.mPtr), mpCount(other.mpCount) {
//         if (mPtr) incStrong();
//     }

//     template<typename U>
//     Lsap(U* other) : mPtr((char*)other) {
//         if (NULL != other) {
//             mpCount = new int32_t(1);
//         } else {
//             mpCount = NULL;
//         }
//     }

//     template<typename U>
//     Lsap(const Lsap<U>& other) : mPtr((T*)(other.mPtr)), mpCount(other.mpCount) {
//         if (mPtr) incStrong();
//     }

//     ~Lsap() {
//         if (mPtr) decStrong();
//     }

//     // Assignment

//     Lsap& operator = (T* other) {
//         // __android_log_print(ANDROID_LOG_INFO, "debug", "[*] %s - 1. mPtr=%p, mpCount=%p, other=%p", __FUNCTION__, mPtr, mpCount, other);
//         // if (other) other->incStrong(this);
//         if (mPtr) decStrong();
//         // Lsap<T>(other);
//         if (NULL != other) {
//             mpCount = new int32_t(1);
//         } else {
//             mpCount = NULL;
//         }
//         mPtr = other;
//         return *this;
//     }

//     Lsap& operator = (const Lsap<T>& other) {
//         T* otherPtr(other.mPtr);
//         if (otherPtr) other.incStrong();
//         if (mPtr) decStrong();
//         mpCount = other.mpCount;
//         mPtr = otherPtr;

//         return *this;
//     }

//     template<typename U>
//     Lsap& operator = (const Lsap<U>& other) {
//         T* otherPtr(other.mPtr);
//         if (otherPtr) other.incStrong();
//         if (mPtr) decStrong();
//         mpCount = other.mpCount;
//         mPtr = otherPtr;

//         return *this;
//     }

//     // template<typename U>
//     // Lsap& operator = (U* other) {
//     //     if (other) ((T*)other)->incStrong(this);
//     //     if (mPtr) mPtr->decStrong(this);
//     //     mPtr = other;
//     //     return *this;
//     // }

//     void Clear() {
//         if (mPtr) {
//             decStrong();
//         }
//     }

//     // Accessors

//     //    inline  T&      operator* () const  { return *mPtr; }
//     // inline  T       operator[](int index) const { return mPtr[index]; }
//     inline  T*      operator-> () const { return mPtr;  }
//     inline  T*      Get() const         { return mPtr; }

//     int32_t GetCount() const   { return NULL == mpCount ? 0 : *mpCount; }

//     bool    IsEmpty() const { return NULL == mPtr ? true : false; }
//     // Operators

//     //     COMPARE(==)
//     //     COMPARE(!=)
//     //     COMPARE(>)
//     //     COMPARE(<)
//     //     COMPARE(<=)
//     //     COMPARE(>=)

// protected:
//     template<typename Y> friend class Lsap;

//     T* mPtr;
//     //mutable volatile int32_t mCount;
//     int32_t* mpCount;

//     inline void incStrong() const {
//         // __android_log_print(ANDROID_LOG_INFO, "debug", "[*] %s - 1. mPtr=%p, mpCount=%d", __FUNCTION__, mPtr, *mpCount);
//         __sync_fetch_and_add(mpCount, 1);
//         // __android_log_print(ANDROID_LOG_INFO, "debug", "[*] %s - 2. mPtr=%p, mpCount=%d", __FUNCTION__, mPtr, *mpCount);
//     }
//     inline void decStrong() {
//         if (__sync_fetch_and_sub(mpCount, 1) == 1) {
//             // __android_log_print(ANDROID_LOG_INFO, "debug", "[*] %s - mPtr=%p, mpCount=%d", __FUNCTION__, mPtr, *mpCount);
//             if (NULL != mPtr) {
//                 delete [] (mPtr);
//                 mPtr = NULL;
//             }
//             delete mpCount;
//             mpCount = NULL;
//         }
//     }
// };

}

#endif //BWSIMPLEJNI_SMARTPOINTER_H
