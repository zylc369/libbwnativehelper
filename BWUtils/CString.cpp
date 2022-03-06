#include "Stdafx.h"
#include <BWUtils/StringHelper.h>
#include <BWUtils/Exceptions.h>
#include <BWUtils/CString.h>

namespace buwai {

CString::CString() {
    Init("");
}

CString::CString(const char* str) {
    Init(str);
}

CString::CString(std::string str) {
    Init(str.c_str());
}

//CString::CString(CString& str) {
//    Init(str.GetCString());
//}

CString::CString(CString const &str) {
    Init(str.GetCString());
}

CString::~CString() {
    Destroy();
}

CString& CString::operator= (const char* str) {
    Destroy();
    Init(str);
    return *this;
}

CString& CString::operator= (std::string str) {
    return operator=(str.c_str());
}

CString& CString::operator= (CString const &str) {
    return operator=(str.GetCString());
}

// 初始化。
void CString::Init(const char* str) {
    mString = NULL;
    mLength = 0;
    mLength = strlen(str);
    mString = new char[mLength + 1];
    if (NULL == mString) {
        THROW(MemoryAllocationFailureException, "内存分配失败");
    }
    strncpy(mString, str, mLength);
    mString[mLength] = '\0';
}

// 销毁。
void CString::Destroy() {
    mLength = 0;
    if (NULL != mString) {
        delete[] mString;
    }
}

CString& CString::operator+= (const char* str) {
    return operator+(str);
}
CString& CString::operator+= (std::string str) {
    return operator+(str.c_str());
}
// CString& CString::operator+= (CString str) {
//     return operator+(str.GetCString());
// }
CString& CString::operator+= (CString& str) {
    return operator+(str.GetCString());
}

CString& CString::operator+= (int8_t value) {
    return operator+(value);
}
CString& CString::operator+= (uint8_t value) {
    return operator+(value);
}
CString& CString::operator+= (int16_t value) {
    return operator+(value);
}
CString& CString::operator+= (uint16_t value) {
    return operator+(value);
}
CString& CString::operator+= (int32_t value) {
    return operator+(value);
}
CString& CString::operator+= (uint32_t value) {
    return operator+(value);
}
CString& CString::operator+= (int64_t value) {
    return operator+(value);
}
CString& CString::operator+= (uint64_t value) {
    return operator+(value);
}

// 追加字符串。
CString& CString::operator+ (const char* str) {
    return Append(str, strlen(str));
}
CString& CString::operator+ (std::string str) {
    return operator+(str.c_str());
}
// CString& CString::operator+ (CString str) {
//     return operator+(str.GetCString());
// }
CString& CString::operator+ (CString& str) {
    return operator+(str.GetCString());
}

// 追加数字。
CString& CString::operator+ (int8_t value) {
    char buf[5];
    if (snprintf(buf, sizeof(buf), "%d", value) < 0) {
        THROW(StringException, "字符串拼接异常。");
    }
    return operator+(buf);
}
CString& CString::operator+ (uint8_t value) {
    char buf[5];
    if (snprintf(buf, sizeof(buf), "%d", value) < 0) {
        THROW(StringException, "字符串拼接异常。");
    }
    return operator+(buf);
}
CString& CString::operator+ (int16_t value) {
    char buf[8];
    if (snprintf(buf, sizeof(buf), "%d", value) < 0) {
        THROW(StringException, "字符串拼接异常。");
    }
    return operator+(buf);
}
CString& CString::operator+ (uint16_t value) {
    char buf[8];
    if (snprintf(buf, sizeof(buf), "%d", value) < 0) {
        THROW(StringException, "字符串拼接异常。");
    }
    return operator+(buf);
}
CString& CString::operator+ (int32_t value) {
    char buf[15];
    if (snprintf(buf, sizeof(buf), "%d", value) < 0) {
        THROW(StringException, "字符串拼接异常。");
    }
    return operator+(buf);
}
CString& CString::operator+ (uint32_t value) {
    char buf[15];
    if (snprintf(buf, sizeof(buf), "%u", value) < 0) {
        THROW(StringException, "字符串拼接异常。");
    }
    return operator+(buf);
}
CString& CString::operator+ (int64_t value) {
    char buf[25];
    if (snprintf(buf, sizeof(buf), "%lld", value) < 0) {
        THROW(StringException, "字符串拼接异常。");
    }
    return operator+(buf);
}
CString& CString::operator+ (uint64_t value) {
    char buf[25];
    if (snprintf(buf, sizeof(buf), "%llu", value) < 0) {
        THROW(StringException, "字符串拼接异常。");
    }
    return operator+(buf);
}

char CString::operator[] (int index) {
    if (index < 0) {
        THROW(ArrayIndexOutOfBoundsException, "index：%d", index);
    }
    size_t len = Length();
    if (index >= len) {
        THROW(ArrayIndexOutOfBoundsException, "index：%d", index);
    }
    return mString[index];
}

// 字符串追加。
CString& CString::Append(const char* buffer, size_t len) {
    size_t newLength = mLength + len;
    char* newString = new char[newLength + 1];
    if (NULL == newString) {
        THROW(MemoryAllocationFailureException, "内存分配失败");
    }
    strncpy(newString, mString, mLength);
    strncpy(newString + mLength, buffer, len);
    newString[newLength] = '\0';

    Destroy();

    mString = newString;
    mLength = newLength;

    return *this;
}

// 获得字符串索引。
int CString::IndexOf(const char* str) {
    if (NULL == str) {
        return -1;
    }
    const char* value = mString;
    size_t valueSize = Length();
    size_t strSize = strlen(str);
    if (strSize > valueSize) {
        return -1;
    }
    size_t valueIndex = 0;
    for (; (valueIndex < valueSize) && ((valueSize - valueIndex) < strSize); valueIndex++) {
        if (value[valueIndex] == str[0]) {
            size_t strIndex = 1;
            for (; strIndex < strSize; strIndex++) {
                if (value[valueIndex + strIndex] != str[strIndex]) {
                    break;
                }
            }
            if (strIndex == strSize) {
                return (int)valueIndex;
            }
        }
    }
    return -1;
}

bool CString::Equal(const char* str) {
    if (str == mString) {
        return true;
    }

    if ((NULL == str) || (NULL == mString)) {
        return false;
    }

    size_t strLength = strlen(str);
    if (strLength != Length()) {
        return false;
    }

    return strncmp(mString, str, strLength) == 0;
}

bool CString::Equal(CString& str) {
    return Equal(str.GetCString());
}

// 获得字符串长度。
size_t CString::Length() {
    return mLength;
}

// 获得C字符串。
    const char * CString::GetCString() const {
    return this->mString;
}

// 格式化字符串。
CString CString::Format(const char* format, ...) {
    /* Reserve two times as much as the length of the format_str */
    va_list ap;
    va_start(ap, format);
    CString result(StringFormatV(format, ap));
    va_end(ap);
    return result;
}

bool operator== (const char* str1, CString& str2) {
    return (str2.Equal(str1));
}
bool operator== (CString& str1, const char* str2) {
    return (str1.Equal(str2));
}
bool operator== (CString& str1, CString& str2) {
    return (str1.Equal(str2));
}

bool operator!= (const char* str1, CString& str2) {
    return !(str2.Equal(str1));
}
bool operator!= (CString& str1, const char* str2) {
    return !(str1.Equal(str2));
}
bool operator!= (CString& str1, CString& str2) {
    return !(str1.Equal(str2));
}

}
