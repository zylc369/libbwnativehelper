#include "Stdafx.h"

#ifdef HAVE_ANDROID_OS
#include "cutils/properties.h"
#endif

#include "log/log.h"

#include <BWNativeHelper/BWNativeHelper.h>

pthread_mutex_t gBWNativeHelperMutex = PTHREAD_MUTEX_INITIALIZER;

BWNativeHelper* BWNativeHelper::mInstance = NULL;

BWNativeHelper::BWNativeHelper() :
    mHandle(NULL),
    GetInstrumentationExt_(NULL),
    InsertOrUpdateTraceMethodInfo_(NULL),
    DeleteTraceMethodInfo_(NULL),
    QueryTraceMethodInfo_(NULL), 
    InsertOrUpdateHookMethodInstInfo_(NULL),
    DeleteHookMethodInstInfo_(NULL),
    DeleteHookMethodInstInfoInMethod_(NULL),
    DeleteHookMethodInstInfoInPackage_(NULL),
    QueryHookMethodInstInfoInMethod_(NULL),
    QueryHookMethodInstInfo_(NULL),
    SetBWDumpFlags_(NULL),
    GetBWDumpFlags_(NULL),
    InitWhenChildStart_(NULL) {
}

BWNativeHelper::~BWNativeHelper() {
    LOG_ALWAYS_FATAL_IF(mInstance == NULL, "BWNativeHelper instance not initialized");
    if (mHandle != NULL) {
        dlclose(mHandle);
    }
}

BWNativeHelper* BWNativeHelper::GetInstance() {
    if (NULL == mInstance) {
        if (0 == pthread_mutex_lock(&gBWNativeHelperMutex)) {
            if (NULL == mInstance) {
                mInstance = new BWNativeHelper;
                if (!mInstance->Init(NULL)) {
                    BWLOGE("[-] BWNativeHelper初始化失败。");
                    delete mInstance;
                    mInstance = NULL;
                }
            }
            pthread_mutex_unlock(&gBWNativeHelperMutex);
        }
    }
    return mInstance;
}

#ifdef HAVE_ANDROID_OS
static const char* kLibrarySystemProperty = "persist.sys.dalvik.vm.lib.2";
static const char* kDebuggableSystemProperty = "ro.debuggable";
static const char* kDebuggableFallback = "0";  // Not debuggable.
#endif
static const char* kLibraryFallback = "libart.so";

template<typename T> void UNUSED(const T&) {}

const char* BWNativeHelper::GetLibrary(const char* library, char* buffer) {
#ifdef HAVE_ANDROID_OS
    const char* default_library;

    char debuggable[PROPERTY_VALUE_MAX];
    property_get(kDebuggableSystemProperty, debuggable, kDebuggableFallback);

    if (strcmp(debuggable, "1") != 0) {
        // Not a debuggable build.
        // Do not allow arbitrary library. Ignore the library parameter. This
        // will also ignore the default library, but initialize to fallback
        // for cleanliness.
        library = kLibraryFallback;
        default_library = kLibraryFallback;
    } else {
        // Debuggable build.
        // Accept the library parameter. For the case it is NULL, load the default
        // library from the system property.
        if (buffer != NULL) {
            property_get(kLibrarySystemProperty, buffer, kLibraryFallback);
            default_library = buffer;
        } else {
            // No buffer given, just use default fallback.
            default_library = kLibraryFallback;
        }
    }
#else
  UNUSED(buffer);
  const char* default_library = kLibraryFallback;
#endif
    if (library == NULL) {
        library = default_library;
    }

    return library;
}

bool BWNativeHelper::Init(const char* library) {
#ifdef HAVE_ANDROID_OS
  char buffer[PROPERTY_VALUE_MAX];
#else
  char* buffer = NULL;
#endif
    library = GetLibrary(library, buffer);

    mHandle = dlopen(library, RTLD_NOW);
    if (mHandle == NULL) {
        if (strcmp(library, kLibraryFallback) == 0) {
            // Nothing else to try.
            BWLOGE("[-] Failed to dlopen %s: %s", library, dlerror());
            return false;
        }
        // Note that this is enough to get something like the zygote
        // running, we can't property_set here to fix this for the future
        // because we are root and not the system user. See
        // RuntimeInit.commonInit for where we fix up the property to
        // avoid future fallbacks. http://b/11463182
        BWLOGW("[-] Falling back from %s to %s after dlopen error: %s",
              library, kLibraryFallback, dlerror());
        library = kLibraryFallback;
        mHandle = dlopen(library, RTLD_NOW);
        if (mHandle == NULL) {
            BWLOGE("[-] dlopen失败：%s-%s", library, dlerror());
            return false;
        }
    }
    if (!FindSymbol(reinterpret_cast<void**>(&GetInstrumentationExt_),
                    "GetInstrumentationExt")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&InsertOrUpdateTraceMethodInfo_),
                    "InsertOrUpdateTraceMethodInfo")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&DeleteTraceMethodInfo_),
                    "DeleteTraceMethodInfo")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&QueryTraceMethodInfo_),
                    "QueryTraceMethodInfo")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&InsertOrUpdateHookMethodInstInfo_),
                    "InsertOrUpdateHookMethodInstInfo")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&DeleteHookMethodInstInfo_),
                    "DeleteHookMethodInstInfo")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&DeleteHookMethodInstInfoInMethod_),
                    "DeleteHookMethodInstInfoInMethod")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&DeleteHookMethodInstInfoInPackage_),
                    "DeleteHookMethodInstInfoInPackage")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&QueryHookMethodInstInfoInMethod_),
                    "QueryHookMethodInstInfoInMethod")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&SetBWDumpFlags_),
                    "SetBWDumpFlags")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&GetBWDumpFlags_),
                    "GetBWDumpFlags")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&InitWhenChildStart_),
                    "InitWhenChildStart")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&InitWhenChildStart_),
                    "InitWhenChildStart")) {
        return false;
    }
    return true;
}

bool BWNativeHelper::FindSymbol(void** pointer, const char* symbol) {
    *pointer = dlsym(mHandle, symbol);
    if (*pointer == NULL) {
        BWLOGE("[-] 未找到符号：%s-%s\n", symbol, dlerror());
        dlclose(mHandle);
        mHandle = NULL;
        return false;
    }
    return true;
}

void* BWNativeHelper::GetInstrumentationExt() {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->GetInstrumentationExt_();
    } else {
        return NULL;
    }
}

bool BWNativeHelper::InsertOrUpdateTraceMethodInfo(Lsp<TraceMethodInfoBase>* traceMethodInfo) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        // BWLOGI("[*] BWNativeHelper::InsertOrUpdateTraceMethodInfo - traceMethodInfo=%p, hash=%d, "
        //     "methodLocation addr=%p", traceMethodInfo,
        //     (*traceMethodInfo)->methodLocation->methodIDBase->hash,
        //     &((*traceMethodInfo)->methodLocation));
        // BWLOGI("[*] BWNativeHelper::InsertOrUpdateTraceMethodInfo - methodLocation addr=%p, "
        //     "methodIDBase addr=%p, hash addr=%p",
        //     (*traceMethodInfo)->methodLocation.Get(), (*traceMethodInfo)->methodLocation->methodIDBase.Get(),
        //     &((*traceMethodInfo)->methodLocation->methodIDBase->hash));
        // BWLOGI("[*] BWNativeHelper::InsertOrUpdateTraceMethodInfo - sizeof(std::string)=%u,"
        // "sizeof(CString)=%u", (unsigned)sizeof(std::string), (unsigned)sizeof(CString));
        return instance->InsertOrUpdateTraceMethodInfo_(traceMethodInfo);
    } else {
        BWLOGE("[-] BWNativeHelper::InsertOrUpdateTraceMethodInfo - 获得BWNativeHelper实例失败。");
        return false;
    }
}

bool BWNativeHelper::DeleteTraceMethodInfo(int hash) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->DeleteTraceMethodInfo_(hash);
    } else {
        BWLOGE("[-] BWNativeHelper::DeleteTraceMethodInfo - 获得BWNativeHelper实例失败。");
        return false;
    }
}

bool BWNativeHelper::QueryTraceMethodInfo(int hash, Lsp<TraceMethodInfoBase>* traceMethodInfo) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->QueryTraceMethodInfo_(hash, traceMethodInfo);
    } else {
        BWLOGE("[-] BWNativeHelper::QueryTraceMethodInfo - 获得BWNativeHelper实例失败。");
        return false;
    }
}

bool BWNativeHelper::InsertOrUpdateHookMethodInstInfo(Lsp<HookMethodInstInfoBase>* hookMethodInstInfo) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL == instance) {
        BWLOGE("[-] BWNativeHelper::InsertOrUpdateHookMethodInstInfo - 获得BWNativeHelper实例失败。");
        return false;
    }
    return instance->InsertOrUpdateHookMethodInstInfo_(hookMethodInstInfo);
}

unsigned int BWNativeHelper::DeleteHookMethodInstInfo(int hash, long instLineNum) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->DeleteHookMethodInstInfo_(hash, instLineNum);
    } else {
        BWLOGE("[-] BWNativeHelper::DeleteHookMethodInstInfo - 获得BWNativeHelper实例失败。");
        return 0;
    }
}

unsigned int BWNativeHelper::DeleteHookMethodInstInfoInMethod(int hash) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->DeleteHookMethodInstInfoInMethod_(hash);
    } else {
        BWLOGE("[-] BWNativeHelper::DeleteHookMethodInstInfoInMethod - 获得BWNativeHelper实例失败。");
        return 0;
    }
}

unsigned int BWNativeHelper::DeleteHookMethodInstInfoInPackage() {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->DeleteHookMethodInstInfoInPackage_();
    } else {
        BWLOGE("[-] BWNativeHelper::DeleteHookMethodInstInfoInPackage - 获得BWNativeHelper实例失败。");
        return 0;
    }
}

bool BWNativeHelper::QueryHookMethodInstInfoInMethod(int hash, 
    std::vector< Lsp<HookMethodInstInfoBase> >* hookMethodInstInfos) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->QueryHookMethodInstInfoInMethod_(hash, hookMethodInstInfos);
    } else {
        BWLOGE("[-] BWNativeHelper::QueryHookMethodInstInfoInMethod - 获得BWNativeHelper实例失败。");
        return false;
    }
}

bool BWNativeHelper::QueryHookMethodInstInfo(int hash, long instLineNum, Lsp<HookMethodInstInfoBase>* hookMethodInstInfo) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->QueryHookMethodInstInfo_(hash, instLineNum, hookMethodInstInfo);
    } else {
        BWLOGE("[-] BWNativeHelper::QueryHookMethodInstInfo - 获得BWNativeHelper实例失败。");
        return false;
    }
}

bool BWNativeHelper::SetBWDumpFlags(int flags) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->SetBWDumpFlags_(flags);
    } else {
        BWLOGE("[-] BWNativeHelper::SetBWDumpFlags - 获得BWNativeHelper实例失败。");
        return false;
    }
}

int BWNativeHelper::GetBWDumpFlags() {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->GetBWDumpFlags_();
    } else {
        BWLOGE("[-] BWNativeHelper::GetBWDumpFlags - 获得BWNativeHelper实例失败。");
        return BWDumpBase::BW_DUMP_FLAGS_INVALID;
    }
}

bool BWNativeHelper::InitWhenChildStart(void* initParams, uint arraySize) {
    BWNativeHelper* instance = BWNativeHelper::GetInstance();
    if (NULL != instance) {
        return instance->InitWhenChildStart_(initParams, arraySize);
    } else {
        BWLOGE("[-] BWNativeHelper::InitWhenChildStart - 获得BWNativeHelper实例失败。");
        return false;
    }
}
