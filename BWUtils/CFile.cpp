#include "Stdafx.h"
#include <BWUtils/CFile.h>

namespace buwai {

CFile::CFile(CString& path) {
    this->mPath = path;
}

CString& CFile::GetPath() {
    return mPath;
}

// 获得文件大小。
ssize_t CFile::Length() {
    FILE* fp = fopen(mPath.GetCString(), "r");
    if (NULL == fp) {
        return -1;
    }
    int start, end;
    off_t filesize = 1;
    int localErrno = 0;

    start = fseeko(fp, 0L, SEEK_CUR);
    if (start < 0) {
        localErrno = errno;
        goto _ret;
    }
    end = fseeko(fp, 0L, SEEK_END);
    if (end < 0) {
        localErrno = errno;
        goto _ret;
    }
    filesize = ftello(fp);
    if (filesize < 0) {
        localErrno = errno;
    }
    fseeko(fp, start, SEEK_SET);
_ret:
    fclose(fp);
    errno = localErrno;
    return filesize;
}

bool CFile::Exist() {
    return 0 == access(mPath.GetCString(), F_OK);
}

bool CFile::IsFile() {
    return !IsDir();
}

bool CFile::IsDir() {
    struct stat fileStat;
    return (stat(mPath.GetCString(), &fileStat) == 0) && S_ISDIR(fileStat.st_mode);
}

bool CFile::MkDir() {
    return (mkdir(mPath.GetCString(), 0777)) < 0;
}

bool CFile::MkDirs() {
    char str[PATH_MAX];
    char currpath[PATH_MAX], *pathpiece;
    struct stat st;
    strncpy(str, mPath.GetCString(), sizeof(str));
    // reset path
    strcpy(currpath, "");
    // create the pieces of the path along the way
    pathpiece = strtok(str, "/");
    if(str[0] == '/') {
        // prepend / if needed
        strcat(currpath, "/");
    }
    while(pathpiece != NULL) {
        if(strlen(currpath) + strlen(pathpiece) + 2/*NUL and slash*/ > PATH_MAX) {
            std::ostringstream oss;
            oss << "[-] MkDirs - 无效路径，路径太长了：" << (strlen(currpath) + strlen(pathpiece) + 2);
            BWLOGE("%s", oss.str().c_str());
            oss.str("");
        }
        strcat(currpath, pathpiece);
        strcat(currpath, "/");
        if(stat(currpath, &st) != 0) {
            if((mkdir(currpath, 0777)) < 0) {
                BWLOGE("[-] MkDirs - mkdir失败，路径：%s，错误信息：%d, %s。",
                       currpath, errno, strerror(errno));
                return false;
            }
        }
        pathpiece = strtok(NULL, "/");
    }
    return true;
}

}
