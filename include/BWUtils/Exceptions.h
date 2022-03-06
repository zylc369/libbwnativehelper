#ifndef BWSIMPLEJNI_BWUTILS_EXCEPTIONS_H
#define BWSIMPLEJNI_BWUTILS_EXCEPTIONS_H

#include <BWUtils/CString.h>
#include <BWUtils/ExceptionBase.h>
#include <BWUtils/StringHelper.h>

namespace buwai {

/**
 * 输入输出异常。
 */
class IOException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(IOException);
};

/**
 * 不支持的Android SDK版本异常。
 */
class UnsupportedAndroidSDKException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(UnsupportedAndroidSDKException);
};

/**
 * 空指针异常。
 */
class NullPointerException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(NullPointerException);
};

/**
 * 无效值异常。
 */
class InvalidValueException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(InvalidValueException);
};

class RemoteException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(RemoteException);
};

/**
 * 数组下标越界异常。
 */
class ArrayIndexOutOfBoundsException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(ArrayIndexOutOfBoundsException);
};

/**
 * 动态链接异常。
 */
class DynamicLinkException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(DynamicLinkException);
};

/**
 * 与字符串相关的异常。
 */
class StringException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(StringException);
};

/**
 * 内存分配失败。
 */
class MemoryAllocationFailureException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(MemoryAllocationFailureException);
};

/**
 * 没有这个方法异常。
 */
class NoSuchMethodException : public ExceptionBase {
public:
    SIMPLE_DECLARE_EXCEPTION(NoSuchMethodException);
};

}

#endif //BWSIMPLEJNI_BWUTILS_EXCEPTIONS_H
