#include "Stdafx.h"
#include <BWUtils/FileDir.h>

namespace buwai {

////////////////////////////////////////////////////////////////////////////
// 文件。

// 拷贝文件。
bool CopyFile(FILE* src, FILE* dest) {
    size_t rc;
    unsigned char buf[BUF_SIZE_4096];

    while (true) {
        rc = fread(buf, sizeof(unsigned char), BUF_SIZE_4096, src);
        if (0 == rc) {
            break;
        }

        fwrite(buf, sizeof(unsigned char), rc, dest);
        if (BUF_SIZE_4096 != rc) {
            break;
        }
    }

    return true;
}

// 拷贝文件。
bool CopyFile(const char* srcFile, const char* outputFile) {
    FILE *in_file = NULL, *out_file = NULL;
    bool bRet = false;

    if ((in_file = fopen(srcFile, "rb")) == NULL) {
        BWLOGW("打开源文件失败。路径：%s", srcFile);
        return false;
    }
    if ((out_file = fopen(outputFile, "wb")) == NULL) {
        BWLOGW("打开目标件失败。路径：%s。%s。", outputFile, strerror(errno));
        fclose(in_file);
        return false;
    }

    bRet = CopyFile(in_file, out_file);

    fclose(out_file);
    fclose(in_file);

    return bRet;
}

// 通过文件描述符获得文件名。
CString GetFilePath(const int fd) {
    CString result((const char*) NULL);
    if (0 >= fd) {
        return result;
    }

    char buf[1024] = { '\0' };
    char file_path[PATH_MAX]; // PATH_MAX in limits.h
    snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fd);
    ssize_t len = readlink(buf, file_path, PATH_MAX - sizeof(char));
    if ((len != -1) && (len != 0)) {
        file_path[len - 1] = '\0';
        result = file_path;
        return result;
    }

    
    return result;
}

// 获得文件长度。
ssize_t GetFileSize(const int fd) {
    off_t start, end;

    start = lseek(fd, 0L, SEEK_CUR);
    end = lseek(fd, 0L, SEEK_END);
    //BWLOGI("GetFileSize(const int) - start=%ld, end=%ld, code=%d,%s。", start, end, errno, strerror(errno));
    (void) lseek(fd, start, SEEK_SET);

    if (start == (off_t) -1 || end == (off_t) -1) {
        return -1;
    }

    if (end == 0) {
        return -1;
    } else {
        return end;
    }
}

// 获得文件长度。
off_t GetFileSize(const char* filePath) {
    FILE* fi = fopen(filePath, "r");
    if (NULL == fi) {
        return -1;
    }
    off_t size = GetFileSize(fi);
    fclose(fi);
    return size;
}

// 获得文件长度。
off_t GetFileSize(FILE* fi) {
    int start, end;
    off_t filesize;

    start = fseeko(fi, 0L, SEEK_CUR);
    end = fseeko(fi, 0L, SEEK_END);
    filesize = ftello(fi);
    fseeko(fi, start, SEEK_SET);
    return filesize;
}

}
