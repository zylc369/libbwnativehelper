#ifndef BWSIMPLEJNI_BWUTILS_EXCEPTIONBASE_H
#define BWSIMPLEJNI_BWUTILS_EXCEPTIONBASE_H

#include <exception>
#include <CObject.h>
#include <BWUtils/CString.h>

namespace buwai {

// class CString;

#define THROW(ExClass, args...)                                               \
    do                                                                           \
    {                                                                            \
        /*之所以起这个变量名是为了保证这个变量名唯一，如果这个变量名不唯一，那么就会出问题*/  \
        ExClass expThrowedABCDFXXXBW(args);                                      \
        expThrowedABCDFXXXBW.Init(__FILE__, __PRETTY_FUNCTION__, __LINE__);      \
        throw expThrowedABCDFXXXBW;                                              \
    }                                                                            \
    while (false)

#define SIMPLE_DECLARE_EXCEPTION(ExClass)                      \
    ExClass() throw();                                         \
    ExClass(CString& msg) throw();                             \
                                                               \
    ExClass(const char* msgFmt, ...) throw();                  \
                                                               \
    virtual ~ExClass() throw();


#define SIMPLE_DEFINE_EXCEPTION(ExClass, Base)                 \
    ExClass::ExClass() throw()                                 \
        : Base()                                               \
    { mClassName = #ExClass; }                                 \
    ExClass::ExClass(CString& msg) throw()                     \
        : Base(msg)                                            \
    { mClassName = #ExClass; }                                 \
                                                               \
    ExClass::ExClass(const char* msgFmt, ...) throw()          \
    {                                                          \
        va_list ap;                                            \
        va_start(ap, msgFmt);                                  \
        mMsg = StringFormat(msgFmt, ap);                       \
        va_end(ap);                                            \
        mClassName = #ExClass;                                 \
    }                                                          \
                                                               \
    ExClass::~ExClass() throw() {}

class ExceptionBase : public CObject, public std::exception
{
public:

    ExceptionBase() throw();

    ExceptionBase(CString& msg) throw();

    ExceptionBase(const char* msgFmt, ...) throw();

    virtual ~ExceptionBase() throw();

    void Init(const char* file, const char* func, int line);

    CString& GetClassName();

    virtual CString& GetMessage();

    CString& ToString();

    //std::string GetStackTrace() const;

protected:
    CString mClassName;
    CString mMsg;
    const char* mFile;
    const char* mFunc;
    int mLine;

private:
//    enum { MAX_STACK_TRACE_SIZE = 50 };
//    void* mStackTrace[MAX_STACK_TRACE_SIZE];
//    size_t mStackTraceSize;
    mutable CString mWhat;

    const char* what() const throw();
};

}

#endif //BWSIMPLEJNI_BWUTILS_EXCEPTIONBASE_H
