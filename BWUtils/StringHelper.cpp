#include "Stdafx.h"
#include <BWUtils/StringHelper.h>

namespace buwai {

std::string StringFormat(const std::string fmt, ...) {
    // TODO: 不够优雅，尝试std::ostringstream oss;是否可行。
    int final_n, n = ((int)fmt.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt.c_str());
        va_start(ap, fmt);
        final_n = vsnprintf(&formatted[0], n, fmt.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}

std::string StringFormatV(const char* fmt, va_list ap) {
    /* Reserve two times as much as the length of the fmt_str */
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int len = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);
    if (len < 0) {
        BWLOGE("[-] StringFormatV - len=%d。错误信息：%d-%s。", len, errno, strerror(errno));
        return "";
    }
    len++;
    char* formatted = new char[len];
    std::string result = "";
    if (NULL == formatted) {
        BWLOGE("[-] StringFormatV - 分配内存失败。错误信息：%d-%s。", errno, strerror(errno));
        return NULL;
    }
    int n = vsnprintf(formatted, len, fmt, ap);
    if (n < 0) {
        BWLOGE("[-] StringFormatV - n=%d。错误信息：%d-%s。", n, errno, strerror(errno));
    } else if (n >= len) {
        BWLOGE("[-] StringFormatV - 需要的字符串长度过长。vsnprintf返回值：%d，缓冲区大小：%d。", n, len);
    } else if (n == (len - 1)) {
        result = formatted;
    } else {
        BWLOGE("[-] StringFormatV - 未知情况。vsnprintf返回值：%d，缓冲区大小：%d。", n, len);
    }
    if (formatted) {
        delete [] formatted;
    }
    return result;
}

std::string StringFormat(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::string result = StringFormatV(fmt, ap);
    va_end(ap);
    return result;
}

// 字符串拷贝。
// Lsap<char> StringCopy(const char* str) {
//     size_t len = strlen(str);
//     Lsap<char> dest = new char[len + 1];
//     if (dest.IsEmpty()) {
//         return Lsap<char>();
//     }
//     strcpy(dest.Get(), str);
//     dest.Get()[len] = '\0';
//     return dest;
// }

// // 字符串追加。
// Lsap<char> StringAppend(const char* str1, const char* str2) {
//     return StringAppend(str1, str2, strlen(str2));
// }

// // 字符串追加。
// Lsap<char> StringAppend(const char* str1, const char* str2, size_t str2Length) {
//     size_t lenForSrc1 = strlen(str1);
//     size_t len = lenForSrc1 + str2Length;
//     Lsap<char> dest = new char[len + 1];
//     if (dest.IsEmpty()) {
//         return Lsap<char>();
//     }
//     strcpy(dest.Get(), str1);
//     strcpy(dest.Get() + lenForSrc1, str2);
//     dest.Get()[len] = '\0';
//     return dest;
// }

}
