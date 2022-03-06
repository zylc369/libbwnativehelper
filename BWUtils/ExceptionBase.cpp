#include "Stdafx.h"
#include <BWUtils/CString.h>
#include <BWUtils/StringHelper.h>
#include <BWUtils/ExceptionBase.h>

namespace buwai {

using namespace std;

ExceptionBase::ExceptionBase() throw()
    : mMsg(""),
      mFile("<unknown file>"),
      mFunc("<unknown func>"),
      mLine(-1)/*,
      mStackTraceSize(0)*/
{
    mClassName = "ExceptionBase";
}

ExceptionBase::ExceptionBase(CString& msg) throw()
    : mMsg(msg),
      mFile("<unknown file>"),
      mFunc("<unknown func>"),
      mLine(-1)/*,
      mStackTraceSize(0)*/
{
    mClassName = "ExceptionBase";
}

ExceptionBase::ExceptionBase(const char* msgFmt, ...) throw()
    : mFile("<unknown file>"),
      mFunc("<unknown func>"),
      mLine(-1)/*,
      mStackTraceSize(0)*/
{
    va_list ap;
    va_start(ap, msgFmt);
    mMsg = StringFormat(msgFmt, ap);
    va_end(ap);
    mClassName = "ExceptionBase";
}

ExceptionBase::~ExceptionBase() throw()
{}

void ExceptionBase::Init(const char* file, const char* func, int line) {
    if (NULL == file) {
        mFile = "<unknown file>";
    } else {
        mFile = file;
    }
    if (NULL == func) {
        mFunc = "<unknown func>";
    } else {
        mFunc = func;
    }
    mLine = line;
    //mStackTraceSize = backtrace(mStackTrace, MAX_STACK_TRACE_SIZE);
}

CString& ExceptionBase::GetClassName() {
    return mClassName;
}

const char* ExceptionBase::what() const throw() {
    return NULL;
}

CString& ExceptionBase::GetMessage() {
    return mMsg;
}

CString& ExceptionBase::ToString() {
    if ("" != mWhat) {
        CString str;
        if (mLine > 0) {
            str += mFile;
            str += "(";
            str += mLine;
            str += ")";
        }
        str += ": ";
        CString className = GetClassName();
        str += className;
        CString& msg = GetMessage();
        if ("" != msg) {
            str += ": ";
            str += msg;
        }
//        str += "\nStack Trace:\n";
//        str += GetStackTrace();
        mWhat = str;
    }
    return mWhat;
}

//std::string ExceptionBase::GetStackTrace() const
//{
//    if (mStackTraceSize == 0)
//        return "<No stack trace>\n";
//    char** strings = backtrace_symbols(mStackTrace, 10);
//    if (strings == NULL) // Since this is for debug only thus
//        // non-critical, don't throw an exception.
//        return "<Unknown error: backtrace_symbols returned NULL>\n";
//
//    std::string result;
//    for (size_t i = 0; i < mStackTraceSize; ++i)
//    {
//        std::string mangledName = strings[i];
//        std::string::size_type begin = mangledName.find('(');
//        std::string::size_type end = mangledName.find('+', begin);
//        if (begin == std::string::npos || end == std::string::npos)
//        {
//            result += mangledName;
//            result += '\n';
//            continue;
//        }
//        ++begin;
//        int status;
//        char* s = abi::__cxa_demangle(mangledName.substr(begin, end-begin).c_str(),
//                                      NULL, 0, &status);
//        if (status != 0)
//        {
//            result += mangledName;
//            result += '\n';
//            continue;
//        }
//        std::string demangledName(s);
//        free(s);
//        // Ignore ExceptionBase::Init so the top frame is the
//        // user's frame where this exception is thrown.
//        //
//        // Can't just ignore frame#0 because the compiler might
//        // inline ExceptionBase::Init.
//        result += mangledName.substr(0, begin);
//        result += demangledName;
//        result += mangledName.substr(end);
//        result += '\n';
//    }
//    free(strings);
//    return result;
//}

}
