#ifndef BWSIMPLEJNI_BWUTILS_STRINGHELPER_H
#define BWSIMPLEJNI_BWUTILS_STRINGHELPER_H

#include <string>
#include <SmartPointer.h>

namespace buwai {

std::string StringFormat(const char* fmt, ...);

std::string StringFormat(const std::string fmt, ...);

std::string StringFormatV(const char* fmt, va_list ap);

/**
 * 字符串拷贝。
 * @param src 要被拷贝的字符串。
 * @return 返回拷贝的字符串。
 */
// Lsap<char> StringCopy(const char* str);

// /**
//  * 字符追加。
//  * @param[in] str1 字符串1。
//  * @param[in] str2 字符串2。
//  * @return 返回字符串：str1 + str2。
//  */
// Lsap<char> StringAppend(const char* str1, const char* str2);

// /**
//  * 字符追加。
//  * @param[in] str1 字符串1。
//  * @param[in] str2 字符串2。
//  * @param[in] str2Length 要拷贝的字符串长度。
//  * @return 返回字符串：str1 + str2。
//  */
// Lsap<char> StringAppend(const char* str1, const char* str2, size_t str2Length);

}

#endif //BWSIMPLEJNI_BWUTILS_STRINGHELPER_H
