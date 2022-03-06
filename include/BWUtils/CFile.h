#ifndef BWSIMPLEJNI_BWUTILS_CFILE_H
#define BWSIMPLEJNI_BWUTILS_CFILE_H

#include <BWDefine.h>
#include <CObject.h>
#include <BWUtils/CString.h>

namespace buwai {

class CFile : public CObject {
public:
    CFile(CString& path);

    CString& GetPath();

    /**
     * 获得文件大小。
     * @return 获得成功，则返回文件长度；获得失败，则返回-1，可通过errno查看错误信息。
     */
    ssize_t Length();

    /**
     * 文件是否存在。
     * @return 文件存在，则返回true；否则，返回false。
     */
    bool Exist();

    /**
     * 是否是一个文件。
     * @return 是一个文件，则返回true；否则，返回false。
     */
    bool IsFile();

    /**
     * 是否是一个目录。
     * @return 是一个目录，则返回true；否则，返回false。
     */
    bool IsDir();

    /**
     * 创建目录。
     * @return 创建成功，则返回true；否则，返回false，可通过errno查看错误信息。
     */
    bool MkDir();

    /**
     * 创建目录。
     * @return 创建成功，则返回true；否则，返回false，可通过errno查看错误信息。
     */
    bool MkDirs();

private:
    CString mPath;
};

}

#endif //BWSIMPLEJNI_BWUTILS_CFILE_H
