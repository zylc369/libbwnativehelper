#include "Stdafx.h"
#include <BWNativeHelper/BWNativeUtils.h>
#include <BWNativeHelper/BWCommon.h>

//////////////////////////////////////////////////////////////////////////
// BWCObject

BWCObject::~BWCObject() {}

//////////////////////////////////////////////////////////////////////////
// BWCString

// BWCString::BWCString(char* str) {
//     this->mLength = strlen(str);
// }

// BWCString::BWCString(Lsp<std::string> str) {
//     BWCString(str.Get());
// }

// BWCString::~BWCString() {}

//////////////////////////////////////////////////////////////////////////
// AppIDBase

const int AppIDBase::APP_TYPE_ANDROID_APP = 1;
const int AppIDBase::APP_TYPE_EXECUTABLE = 2;
const int AppIDBase::START_FLAG_UNSET = 0;
const int AppIDBase::START_FLAG_INTERPRETER = 1;
const int AppIDBase::START_FLAG_ROOT = (1 << 1);

AppIDBase::AppIDBase(int uid, const char* packageName, int appType, int startFlags) {
    // BWLOGI("[*] AppIDBase::AppIDBase - uid=%d", uid);
    // BWLOGI("[*] AppIDBase::AppIDBase - packageName=%s", packageName);
    // BWLOGI("[*] AppIDBase::AppIDBase - appType=%d", appType);
    // BWLOGI("[*] AppIDBase::AppIDBase - startFlags=%d", startFlags);
    this->uid = uid;
    // BWLOGI("[*] AppIDBase::AppIDBase - 1");
    this->packageName = packageName;
    // BWLOGI("[*] AppIDBase::AppIDBase - 2");
    this->appType = appType;
    // BWLOGI("[*] AppIDBase::AppIDBase - 3");
    this->startFlags = startFlags;
    // BWLOGI("[*] AppIDBase::AppIDBase - 4");
}

AppIDBase::~AppIDBase() {}

Lsp<BWCObject> AppIDBase::Clone() {
    return Lsp<AppIDBase> (new AppIDBase(this->uid, this->packageName.GetCString(),
      this->appType, this->startFlags));
}

bool AppIDBase::Equals(Lsp<BWCObject> o) {
    Lsp<AppIDBase> other = (Lsp<AppIDBase>) o;
    return (this->uid == other->uid) && (this->packageName == other->packageName) &&
           (this->appType == other->appType) && (this->startFlags == other->startFlags);
}

//////////////////////////////////////////////////////////////////////////
// MethodIDBase

MethodIDBase::MethodIDBase(const char* classDesc, const char* methodName, const char* methodSig, int hash) {
    this->classDesc = classDesc;
    this->methodName = methodName;
    this->methodSig = methodSig;
    this->hash = hash;
}

MethodIDBase::~MethodIDBase() {}

Lsp<BWCObject> MethodIDBase::Clone() {
    return Lsp<MethodIDBase> (new MethodIDBase(this->classDesc.GetCString(),
      this->methodName.GetCString(), this->methodSig.GetCString(), this->hash));
}

bool MethodIDBase::Equals(Lsp<BWCObject> o) {
    Lsp<MethodIDBase> other = (Lsp<MethodIDBase>) o;
    return (this->classDesc == other->classDesc) &&
           (this->methodName == other->methodName) &&
           (this->methodSig == other->methodSig) &&
           (this->hash == other->hash);
}

//////////////////////////////////////////////////////////////////////////
// MethodLocation

MethodLocation::MethodLocation(Lsp<AppIDBase> appIDBase, Lsp<MethodIDBase> methodIDBase) {
    this->appIDBase = appIDBase;
    this->methodIDBase = methodIDBase;
}

MethodLocation::~MethodLocation() {}

Lsp<BWCObject> MethodLocation::Clone() {
    return Lsp<MethodLocation> (new MethodLocation(this->appIDBase->Clone(), 
      this->methodIDBase->Clone()));
}

bool MethodLocation::Equals(Lsp<BWCObject> o) {
    Lsp<MethodLocation> other = (Lsp<MethodLocation>) o;
    return (this->appIDBase->Equals(other->appIDBase)) &&
           (this->methodIDBase->Equals(other->methodIDBase));
}

//////////////////////////////////////////////////////////////////////////
// InstructionLocation

InstructionLocation::InstructionLocation(Lsp<AppIDBase> appIDBase, Lsp<MethodIDBase> methodIDBase, 
    int64_t instLineNum, int64_t dexPC) : InstructionLocationSuper(appIDBase, methodIDBase) {
    this->instLineNum = instLineNum;
    this->dexPC = dexPC;
}

InstructionLocation::~InstructionLocation() {}

Lsp<BWCObject> InstructionLocation::Clone() {
    return Lsp<InstructionLocation> (new InstructionLocation(this->appIDBase->Clone(), 
        this->methodIDBase->Clone(), this->instLineNum, this->dexPC));
}

bool InstructionLocation::Equals(Lsp<BWCObject> o) {
    Lsp<InstructionLocation> other = (Lsp<InstructionLocation>) o;
    return InstructionLocationSuper::Equals(other) && (this->instLineNum == other->instLineNum) &&
           (this->dexPC == other->dexPC);
}

//////////////////////////////////////////////////////////////////////////
// TraceMethodInfoBase

const int TraceMethodInfoBase::TRACE_METHOD_INFO_FLAG_UNKNOW = 0;
const int TraceMethodInfoBase::TRACE_METHOD_INFO_FLAG_ENABLE = 1;
const int TraceMethodInfoBase::TRACE_METHOD_INFO_FLAG_PRINTCALLSTACK = 1 << 1;
const int TraceMethodInfoBase::TRACE_METHOD_INFO_FLAG_TRANSITIVE = 1 << 2;

const int TraceMethodInfoBase::GRANULARITY_LEVEL_METHOD = 1;
const int TraceMethodInfoBase::GRANULARITY_LEVEL_INSTRUCTION = 2;

const int TraceMethodInfoBase::PROMPT_METHOD_TYPE_ARY = 0;
const int TraceMethodInfoBase::PROMPT_METHOD_TYPE_NATIVE = 1;

TraceMethodInfoBase::TraceMethodInfoBase() : traceMethodFlags(TRACE_METHOD_INFO_FLAG_UNKNOW),
    granularity(GRANULARITY_LEVEL_METHOD), promptMethodType(PROMPT_METHOD_TYPE_ARY), deep(0) {}

TraceMethodInfoBase::~TraceMethodInfoBase() {}

Lsp<BWCObject> TraceMethodInfoBase::Clone() {
    Lsp<TraceMethodInfoBase> newTraceMethodInfoBase = new TraceMethodInfoBase();
    newTraceMethodInfoBase->methodLocation = (Lsp<MethodLocation>) this->methodLocation->Clone();
    newTraceMethodInfoBase->traceMethodFlags = this->traceMethodFlags;
    newTraceMethodInfoBase->granularity = this->granularity;
    newTraceMethodInfoBase->promptMethodType = this->promptMethodType;
    return newTraceMethodInfoBase;
}

bool TraceMethodInfoBase::Equals(Lsp<BWCObject> o) {
    Lsp<TraceMethodInfoBase> other = (Lsp<TraceMethodInfoBase>) o;
    return (this->methodLocation->Equals(other->methodLocation)) &&
           (this->traceMethodFlags == other->traceMethodFlags) && 
           (this->granularity == this->granularity) &&
           (this->promptMethodType == other->promptMethodType);
}

//////////////////////////////////////////////////////////////////////////
// HookMethodInstInfoBase::ContentData

HookMethodInstInfoBase::ContentData::ContentData() : isHookMethodStatic(false) {}

HookMethodInstInfoBase::ContentData::~ContentData() {}

Lsp<BWCObject> HookMethodInstInfoBase::ContentData::Clone() {
    Lsp<HookMethodInstInfoBase::ContentData> newContentData = 
        new HookMethodInstInfoBase::ContentData();

    newContentData->hookDexPath = this->hookDexPath;
    newContentData->hookClassDesc = this->hookClassDesc;
    newContentData->hookMethodName = this->hookMethodName;
    newContentData->hookMethodSig = this->hookMethodSig;
    newContentData->isHookMethodStatic = this->isHookMethodStatic;
    newContentData->thisRegister = this->thisRegister;
    newContentData->paramRegisters = this->paramRegisters;
    newContentData->returnRegister = this->returnRegister;
    return newContentData;
}

bool HookMethodInstInfoBase::ContentData::Equals(Lsp<BWCObject> o) {
    Lsp<HookMethodInstInfoBase::ContentData> other = (Lsp<HookMethodInstInfoBase::ContentData>) o;
    return (this->hookDexPath == other->hookDexPath) &&
           (this->hookClassDesc == other->hookClassDesc) &&
           (this->hookMethodName == other->hookMethodName) &&
           (this->hookMethodSig == other->hookMethodSig) &&
           (this->isHookMethodStatic == other->isHookMethodStatic) &&
           (this->thisRegister == other->thisRegister) &&
           (this->paramRegisters == other->paramRegisters) &&
           (this->returnRegister == other->returnRegister);
}

//////////////////////////////////////////////////////////////////////////
// HookMethodInstInfoBase

HookMethodInstInfoBase::HookMethodInstInfoBase() {}

HookMethodInstInfoBase::~HookMethodInstInfoBase() {}

Lsp<BWCObject> HookMethodInstInfoBase::Clone() {
    Lsp<HookMethodInstInfoBase> newHookMethodInstInfoBase = new HookMethodInstInfoBase();
    newHookMethodInstInfoBase->instructionLocation = (Lsp<InstructionLocation>) this->instructionLocation->Clone();
    newHookMethodInstInfoBase->contentData = (Lsp<HookMethodInstInfoBase::ContentData>) this->contentData->Clone();
    return newHookMethodInstInfoBase;
}

bool HookMethodInstInfoBase::Equals(Lsp<BWCObject> o) {
    Lsp<HookMethodInstInfoBase> other = (Lsp<HookMethodInstInfoBase>) o;
    return (this->instructionLocation->Equals(other->instructionLocation)) &&
           (this->contentData->Equals(other->contentData));
}

//////////////////////////////////////////////////////////////////////////
// BWDumpBase

const int BWDumpBase::BW_DUMP_FLAGS_INVALID = (1 << 31);
const int BWDumpBase::BW_DUMP_FLAGS_DISABLE = 0;
const int BWDumpBase::BW_DUMP_FLAGS_PRINT = 1;
const int BWDumpBase::BW_DUMP_FLAGS_WRITE_FILE = (1 << 1);
const int BWDumpBase::BW_DUMP_FLAGS_ALL = BW_DUMP_FLAGS_PRINT | BW_DUMP_FLAGS_WRITE_FILE;

BWDumpBase::BWDumpBase() : bwDumpFlags(BW_DUMP_FLAGS_ALL) {}

BWDumpBase::~BWDumpBase() {}

Lsp<BWCObject> BWDumpBase::Clone() {
    Lsp<BWDumpBase> newBWDumpBase = new BWDumpBase();
    newBWDumpBase->appIDBase = (Lsp<AppIDBase>) this->appIDBase->Clone();
    newBWDumpBase->bwDumpFlags = this->bwDumpFlags;
    return newBWDumpBase;
}

bool BWDumpBase::Equals(Lsp<BWCObject> o) {
    Lsp<BWDumpBase> other = (Lsp<BWDumpBase>) o;
    return (this->appIDBase->Equals(other->appIDBase)) &&
           (this->bwDumpFlags == other->bwDumpFlags);
}
