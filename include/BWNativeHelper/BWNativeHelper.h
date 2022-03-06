#pragma once

#include <map>
#include <vector>

#include <BWNativeHelper/BWCommon.h>

class BWNativeHelper {
public:
	static void* GetInstrumentationExt();

	static bool InsertOrUpdateTraceMethodInfo(Lsp<TraceMethodInfoBase>* traceMethodInfo);

	static bool DeleteTraceMethodInfo(int hash);

	static bool QueryTraceMethodInfo(int hash, Lsp<TraceMethodInfoBase>* traceMethodInfo);

	static bool InsertOrUpdateHookMethodInstInfo(Lsp<HookMethodInstInfoBase>* hookMethodInstInfo);

	static unsigned int DeleteHookMethodInstInfo(int hash, long instLineNum);

	static unsigned int DeleteHookMethodInstInfoInMethod(int hash);

	static unsigned int DeleteHookMethodInstInfoInPackage();

	static bool QueryHookMethodInstInfoInMethod(int hash, std::vector< Lsp<HookMethodInstInfoBase> >* hookMethodInstInfos);

	static bool QueryHookMethodInstInfo(int hash, long instLineNum, Lsp<HookMethodInstInfoBase>* hookMethodInstInfo);

	static bool SetBWDumpFlags(int flags);

	static int GetBWDumpFlags();

	static bool InitWhenChildStart(void* initParams, uint arraySize);

	~BWNativeHelper();

private:
	static BWNativeHelper* GetInstance();

	static BWNativeHelper* mInstance;

	void* mHandle;

	void* (*GetInstrumentationExt_)();

	bool (*InsertOrUpdateTraceMethodInfo_)(void*);

	bool (*DeleteTraceMethodInfo_)(int);

	bool (*QueryTraceMethodInfo_)(int, void*);

	bool (*InsertOrUpdateHookMethodInstInfo_)(void*);

	unsigned int (*DeleteHookMethodInstInfo_)(int, long);

	unsigned int (*DeleteHookMethodInstInfoInMethod_)(int);

	unsigned int (*DeleteHookMethodInstInfoInPackage_)();

	bool (*QueryHookMethodInstInfoInMethod_)(int, void*);

	bool (*QueryHookMethodInstInfo_)(int, long, void*);

	bool (*SetBWDumpFlags_)(int);

	int (*GetBWDumpFlags_)();

	bool (*InitWhenChildStart_)(void*, uint);

	static const char* GetLibrary(const char* library, char* buffer);

	BWNativeHelper();

	bool Init(const char* library);

	bool FindSymbol(void** pointer, const char* symbol);
};
