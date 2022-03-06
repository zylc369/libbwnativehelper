#ifndef BWSIMPLEJNI_BWUTILS_FILEDIR_H
#define BWSIMPLEJNI_BWUTILS_FILEDIR_H

#include <stdio.h>
#include <BWUtils/CString.h>

namespace buwai {

////////////////////////////////////////////////////////////////////////////
// 文件。
/**
 * 拷贝文件。
 * @param[in] src 源文件的文件指针。
 * @param[in] dest 输出文件的文件指针。
 * @return true: 拷贝成功。false: 拷贝失败。
 */
bool CopyFile(FILE* src, FILE* dest);

/**
 * 拷贝文件。
 * @param[in] srcFile 源文件。
 * @param[in] outputFile 输出文件。
 * @return true: 拷贝成功。false: 拷贝失败。
 */
bool CopyFile(const char* srcFile, const char* outputFile);

/**
 * 通过文件描述符获得文件名。
 * @param[in] 文件描述符。
 * @return 获得成功，则返回文件名指针。获得失败，则返回NULL。
 */
CString GetFilePath(const int fd);

/**
 * 获得文件长度。
 * @param[in] 文件描述符。
 * @return 获得成功，则返回文件长度。获得失败，则返回-1。
 */
ssize_t GetFileSize(const int fd);

/**
 * 获得文件长度。
 * @param[in] filePath 文件路径。
 * @return 获得成功，则返回文件长度。获得失败，则返回-1。
 */
off_t GetFileSize(const char* filePath);

/**
 * 获得文件长度。
 * @param[in] 文件指针。
 * @return 获得成功，则返回文件长度。获得失败，则返回-1。
 */
off_t GetFileSize(FILE* fi);

}

#endif //BWSIMPLEJNI_BWUTILS_FILEDIR_H
