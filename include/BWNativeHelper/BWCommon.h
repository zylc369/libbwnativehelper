#pragma once

#include <stdint.h>

#include <SmartPointer.h>
#include <BWUtils/CString.h>

#include "BWMacro.h"

using namespace buwai; 

/**
 * 粒度。
 */
enum GranularityLevel {
    GRANULARITY_LEVEL_METHOD                    = 1,  // 方法级别。
    GRANULARITY_LEVEL_INSTRUCTION               = 2,  // 指令级别。
};

/**
 * 所要监控的方法类型。
 * 当GRANULARITY_LEVEL_CURRENT_METHOD被设置表示“提示”时才有意义。
 */
enum PromptMethodType {
    PROMPT_METHOD_TYPE_ARY                             = 0, // 不限。
    PROMPT_METHOD_TYPE_NATIVE                          = 1, // native方法。
};

/**
 * 超简单的键值对。
 */
struct SimpleKeyValuePair {
    const char* key;
    const char* value;
};

//////////////////////////////////////////////////////////////////////////
// BWCObject

class BWCObject {
public:
    virtual ~BWCObject();
};

//////////////////////////////////////////////////////////////////////////
// BWCString

// class BWCString : public BWCObject, std::string {
// public:
//     BWCString(char* str);

//     BWCString(Lsp<std::string> str);

//     virtual ~BWCString();

//     size_t Length();

// private:
//     size_t mLength;
// }

//////////////////////////////////////////////////////////////////////////
// AppIDBase

#define AppIDBaseSuper BWCObject

class AppIDBase : public AppIDBaseSuper {
public:
    /**
     * Android App程序。
     */
    static const int APP_TYPE_ANDROID_APP;
    /**
     * 本地代码的可执行程序。
     */
    static const int APP_TYPE_EXECUTABLE;

    /**
     * 未设置启动标志。
     */
    static const int START_FLAG_UNSET;
    /**
     * 以解释模式启动。
     */
    static const int START_FLAG_INTERPRETER;

    /**
     * 以root权限启动。
     */
    static const int START_FLAG_ROOT;

    /**
     * App UID。
     */
    int uid;

    /**
     * 对于应用来说这代表包名。
     * 对于可执行程序来说这代表完整路径。
     */
    CString packageName;

    /**
     * 应用类型。
     */
    int appType;

    /**
     * 启动标记。
     */
    int startFlags;

    AppIDBase(int uid, const char* packageName, int appType, int startFlags);

    virtual ~AppIDBase();

    virtual Lsp<BWCObject> Clone();

    virtual bool Equals(Lsp<BWCObject> other);
};

//////////////////////////////////////////////////////////////////////////
// MethodIDBase

#define MethodIDBaseSuper BWCObject

class MethodIDBase : public MethodIDBaseSuper {
public:
    /**
     * 类描述符。如：Ljava/lang/String;
     */
    CString classDesc;
    /**
     * 方法名。
     */
    CString methodName;
    /**
     * 方法签名。如：()Ljava/lang/String;。
     */
    CString methodSig;

    /**
     * 这个哈希值的计算方法：
     * (classDesc + methodName + methodSig).hashCode()。
     */
    int hash;

    MethodIDBase(const char* classDesc, const char* methodName, const char* methodSig, int hash);

    virtual ~MethodIDBase();

    virtual Lsp<BWCObject> Clone();

    virtual bool Equals(Lsp<BWCObject> other);
};

//////////////////////////////////////////////////////////////////////////
// MethodLocation

#define MethodLocationSuper BWCObject

class MethodLocation : public MethodLocationSuper {
public:
    Lsp<AppIDBase> appIDBase;
    Lsp<MethodIDBase> methodIDBase;

    MethodLocation(Lsp<AppIDBase> appIDBase, Lsp<MethodIDBase> methodIDBase);

    virtual ~MethodLocation();

    virtual Lsp<BWCObject> Clone();

    virtual bool Equals(Lsp<BWCObject> other);
};

//////////////////////////////////////////////////////////////////////////
// InstructionLocation

# define InstructionLocationSuper MethodLocation

class InstructionLocation : public InstructionLocationSuper {
public:
    /**
     * dex方法的指令行号。
     */
    int64_t instLineNum;
    /**
     * dex方法的pc值。
     * 这个值应该是一个运行时的概念。
     */
    int64_t dexPC;

    InstructionLocation(Lsp<AppIDBase> appIDBase, Lsp<MethodIDBase> methodIDBase, int64_t instLineNum, int64_t dexPC);

    virtual ~InstructionLocation();

    virtual Lsp<BWCObject> Clone();

    virtual bool Equals(Lsp<BWCObject> other);
};

//////////////////////////////////////////////////////////////////////////
// TraceMethodInfoBase

#define TraceMethodInfoBaseSuper BWCObject

class TraceMethodInfoBase : public TraceMethodInfoBaseSuper {
public:
    static const int TRACE_METHOD_INFO_FLAG_UNKNOW;
    /**
     * 这个标志指定启用跟踪信息。
     */
    static const int TRACE_METHOD_INFO_FLAG_ENABLE;
    /**
     * 这个标志指定需要打印调用堆栈。
     */
    static const int TRACE_METHOD_INFO_FLAG_PRINTCALLSTACK;
    /**
     * 这个标志指定跟踪信息是可传递的。
     */
    static const int TRACE_METHOD_INFO_FLAG_TRANSITIVE;

    /**
     * 粒度：方法级别。
     */
    static const int GRANULARITY_LEVEL_METHOD;
    /**
     * 粒度：指令级别。
     */
    static const int GRANULARITY_LEVEL_INSTRUCTION;

    /**
     * 要提示的方法类型：不限。
     * 当GRANULARITY_LEVEL_CURRENT_METHOD被设置表示“提示”时才有意义。
     */
    static const int PROMPT_METHOD_TYPE_ARY;
    /**
     * 要提示的方法类型：native方法。
     * 当GRANULARITY_LEVEL_CURRENT_METHOD被设置表示“提示”时才有意义。
     */
    static const int PROMPT_METHOD_TYPE_NATIVE;

    Lsp<MethodLocation> methodLocation;
    int traceMethodFlags;
    int granularity;
    int promptMethodType;

    /**
     * 跟踪深度。
     * 在数据库中没有这个字段，这是一个运行时信息。
     */
    uint32_t deep;

    TraceMethodInfoBase();

    virtual ~TraceMethodInfoBase();

    virtual Lsp<BWCObject> Clone();

    virtual bool Equals(Lsp<BWCObject> other);

    /**
     * 是否启用。
     * @return 返回true启用；返回false禁用。
     */
    inline bool IsEnable() {
        return ((traceMethodFlags & TRACE_METHOD_INFO_FLAG_ENABLE) != 0);
    }

    /**
     * 是否打印调用堆栈。
     * @return 返回true打印调用堆栈；返回false不打印调用堆栈。
     */
    inline bool IsPrintCallStack() {
        return ((traceMethodFlags & TRACE_METHOD_INFO_FLAG_PRINTCALLSTACK) != 0);
    }

    /**
     * 跟踪信息是否是可传递的。
     * @return 
     */
    inline bool IsTransitive() {
        return IsEnable() && ((traceMethodFlags & TRACE_METHOD_INFO_FLAG_TRANSITIVE) != 0);
    }

    inline void AddDeep() {
        ++deep;
    }

    inline void DecDeep() {
        --deep;
    }
};

#define HookMethodInstInfoBaseSuper BWCObject
#define ContentDataSuper BWCObject

class HookMethodInstInfoBase : public HookMethodInstInfoBaseSuper {
public:
    class ContentData : public ContentDataSuper {
    public:
        /**
         * 当Hook Dex路径为null时，表示调用的是当前dex中的方法。
         */
        CString hookDexPath;
        /**
         * Hook方法所属的类描述符。
         * 当调用当前dex中的方法，且是非静态方法时，此参数可以为null。
         */
        CString hookClassDesc;
        /**
         * Hook方法名。
         */
        CString hookMethodName;
        /**
         * Hook方法签名。
         */
        CString hookMethodSig;
        /**
         * Hook方法是否为静态方法。
         * true：静态方法；false：非静态方法。
         */
        bool isHookMethodStatic;
        /**
         * 当是静态方法时，这个字段无效；
         * 当Hook方法在其他的DEX中时，这个字段无效（因为this寄存器所引用的类型只可能是当前dex中的类型）。
         */
        CString thisRegister;
        /**
         * 参数寄存器，以逗号分隔。
         * 当不需要传入参数时将这个字段设置为null。
         */
        CString paramRegisters;
        /**
         * 返回寄存器。
         * 当不需要返回时将这个字段设置为null。
         */
        CString returnRegister;
        /**
         * hook方法的方法ID。这里指的是jmethodID。
         * 当Hook方法在其他dex中时，这个字段必须为有效值。
         * 这是一个运行时需要的状态，所以注释掉这个字段。
         */
    //        int64_t methodID = 0;
        ContentData();

        virtual ~ContentData();

        virtual Lsp<BWCObject> Clone();

        virtual bool Equals(Lsp<BWCObject> other);
    };


    Lsp<InstructionLocation> instructionLocation;
    Lsp<ContentData> contentData;

    HookMethodInstInfoBase();

    virtual ~HookMethodInstInfoBase();

    virtual Lsp<BWCObject> Clone();

    virtual bool Equals(Lsp<BWCObject> other);
};

#define BWDumpBaseSuper BWCObject

class BWDumpBase : public BWDumpBaseSuper {
public:
    static const int BW_DUMP_FLAGS_INVALID;
    static const int BW_DUMP_FLAGS_DISABLE;
    static const int BW_DUMP_FLAGS_PRINT;
    static const int BW_DUMP_FLAGS_WRITE_FILE;
    static const int BW_DUMP_FLAGS_ALL;

    Lsp<AppIDBase> appIDBase;
    int bwDumpFlags;

    BWDumpBase();

    virtual ~BWDumpBase();

    virtual Lsp<BWCObject> Clone();

    virtual bool Equals(Lsp<BWCObject> other);
};
