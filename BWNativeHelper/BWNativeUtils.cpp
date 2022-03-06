#include "Stdafx.h"
#include <JNIJava/JStringHelper.h>
#include <BWNativeHelper/BWNativeUtils.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

int HashCode(const char* str, size_t len) {
    int h = 0;
    if (len > 0) {
        for (size_t i = 0; i < len; i++) {
            h = 31 * h + str[i];
        }
    }
    return h;
}

bool GetProcessName(pid_t pid, char processName[PATH_MAX]) {
    int fd, r;
    char cmd[PATH_MAX] = { '\0' };
    sprintf(cmd, "/proc/%d/cmdline", pid);
    fd = open(cmd, O_RDONLY);
    if (-1 == fd) {
        BWLOGE("[-] GetProcessName(pid_t, char[]) - open失败，pid=%d, cmd=%s。code=%d: %s。", pid, cmd, errno, strerror(errno));
        r = 0;
    } else {
        errno = 0;
        r = read(fd, processName, PATH_MAX - 1);
        close(fd);
        if (r < 0) {
            BWLOGE("[-] GetProcessName(pid_t, char[]) - read失败，pid=%d, cmd=%s。code=%d: %s。", pid, cmd, errno, strerror(errno));
            r = 0;
        }
    }
    processName[r] = '\0';

    if (-1 != fd)
        close(fd);
    return (r != 0);
}

namespace buwai {

std::string GetOutputFromShell(const char* pCmd)
{
    std::string output = "";
    char buf[256];
    FILE* stream = popen( pCmd, "r" ); //将“ls －l”命令的输出 通过管道读取（“r”参数）到FILE* stream
    if (NULL == stream) {
        BWLOGE("[-] GetOutputFromShell - 打开管道失败！错误号：%d-%s", errno, strerror(errno));
        return output;
    }
    while (1) {
        //将刚刚FILE* stream的数据流读取到buf中
        int readNum = fread( buf, sizeof(char), sizeof(buf) - 1, stream);
        if (0 == readNum) {
            if (feof(stream)) {
                break;
            }

            if (ferror(stream)) {
                BWLOGE("[-] GetOutputFromShell - 读取shell输出失败！错误号：%d-%s", errno, strerror(errno));
                break;
            } else {
                BWLOGE("[-] GetOutputFromShell - 未读取失败，也没有读取到输出结尾，这是什么鬼？");
                break;
            }
        }
        buf[readNum - 1] = '\0';
        output += buf;
    }
    
    if (pclose( stream ) < 0) {
        BWLOGE("[-] GetOutputFromShell - 关闭管道失败！错误号：%d-%s", errno, strerror(errno));
    }

    return output;
}

std::string GetPackageNameByUID(uid_t uid) {
    std::string packageFilePath = "/data/system/packages.xml";
    std::ostringstream oss;
    oss << "cat " << packageFilePath << " | grep \"userId=\\\"" << uid << "\\\"\"";
    std::string cmd = oss.str();
    std::string output = GetOutputFromShell(cmd.c_str());
    std::string packageName = "";
    int leftIndex = output.find("package name=\"");
    if (-1 == leftIndex) {
        BWLOGE("[-] GetPackageNameByUID - 查找左索引失败！uid=%d, cmd=%s, output=%s", 
            uid, cmd.c_str(), output.c_str());
    } else {
        leftIndex += strlen("package name=\"");
        int rightIndex = output.find("\"", leftIndex);
        if (-1 == rightIndex) {
            BWLOGE("[-] GetPackageNameByUID - 查找右索引失败！ leftIndex=%d, uid=%d, cmd=%s, output=%s", 
                leftIndex, uid, cmd.c_str(), output.c_str());
        } else {
            packageName = output.substr(leftIndex, rightIndex - leftIndex);
        }
    }
    return packageName;
}

CString GetCurrentTimeString() {
    time_t t;
    t = time(NULL);
    struct tm *lt;
    time_t ii = time(&t);
    t = time(NULL);
    lt = localtime(&t);
    int len = 50;
    char* nowtime = (char*) malloc(len);
    memset(nowtime, 0, len);
    strftime(nowtime, len, "%Y-%m-%d+%H-%M-%S", lt);
    CString result = nowtime;
    free(nowtime);
    return result;
}

}
