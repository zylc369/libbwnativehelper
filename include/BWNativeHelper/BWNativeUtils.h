#pragma once

#include <jni.h>
#include <string>
#include <stdint.h>
#include <BWUtils/CString.h>
#include "SmartPointer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 计算字符串的哈希。
 * 它的算法是从Java的String.hashCode()中拷贝的，目的是要与Java字符串的哈希计算结果一致。
 * @param[in] str 字符串。
 * @return 返回字符串的哈希值。
 */
extern "C" int HashCode(const char* str, size_t len);

bool GetProcessName(pid_t pid, char processName[PATH_MAX]);

#ifdef __cplusplus
}
#endif

namespace buwai {

std::string GetOutputFromShell(const char* pCmd);

/**
 * 通过读取/data/system/packages.xml文件获得包名，这个文件是system权限。
 * @param[in] uid uid。
 * @return 返回包名。
 */
std::string GetPackageNameByUID(uid_t uid);

/**
 * 获得当前时间的字符串表示形式。
 * @return 返回当前时间的字符串表示形式。当不在使用这个返回值时调用者需要通过free函数释放字符串的内存。
 */
CString GetCurrentTimeString();

}
