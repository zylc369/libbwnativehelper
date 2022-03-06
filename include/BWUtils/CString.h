#ifndef BWSIMPLEJNI_BWUTILS_CSTRING_H
#define BWSIMPLEJNI_BWUTILS_CSTRING_H

#include <string>
#include <CObject.h>

namespace buwai {

class CString : public CObject {
public:
    CString();

    /**
     * 构造函数。
     * @param[in] str 字符串，构造函数会将该字符串拷贝一份。
     *                该参数可以传入NULL，这表示要清空字符串释放字符串内存。
     * @exception StringException 字符串拷贝失败将会抛出该异常。
     */
    CString(const char* str);

    CString(std::string str);

//    CString(CString& str);

    CString(CString const &str);

    ~CString();

    /**
     * 重载赋值运算符。
     * @param[in] str 字符串，函数内会将该字符串拷贝一份。
     *                可以赋值为NULL，这表示要清空字符串释放字符串内存。
     * @return 返回CString类对象的引用。
     */
    CString& operator= (const char* str);
    CString& operator= (std::string str);
    CString& operator= (CString const &str);

    CString& operator+= (const char* str);
    CString& operator+= (std::string str);
    // CString& operator+= (CString str);
    CString& operator+= (CString& str);

    CString& operator+= (int8_t value);
    CString& operator+= (uint8_t value);
    CString& operator+= (int16_t value);
    CString& operator+= (uint16_t value);
    CString& operator+= (int32_t value);
    CString& operator+= (uint32_t value);
    CString& operator+= (int64_t value);
    CString& operator+= (uint64_t value);

    /**
     * 追加字符串。
     * @param[in] str 追加的字符串。
     * @return 返回CString类对象的引用。
     */
    CString& operator+ (const char* str);
    CString& operator+ (std::string str);
    // CString& operator+ (CString str);
    CString& operator+ (CString& str);

    /**
     * 追加数字。
     */
    CString& operator+ (int8_t value);
    CString& operator+ (uint8_t value);
    CString& operator+ (int16_t value);
    CString& operator+ (uint16_t value);
    CString& operator+ (int32_t value);
    CString& operator+ (uint32_t value);
    CString& operator+ (int64_t value);
    CString& operator+ (uint64_t value);

    char operator[] (int index);

    /**
     * 追加字符串。
     * @param[in] buffer 字符串数组。
     * @param[in] len 追加的长度。
     * @return 返回CString类对象的引用。
     */
    CString& Append(const char* buffer, size_t len);

    /**
     * 获得字符串索引。
     * @param[in] str 字符串。
     * @return 找到相应的字符串，则返回索引号；若未找到，则返回-1。
     */
    int IndexOf(const char* str);

    /**
     * 判断两个字符串是否相等。
     * @param[in] str 字符串。
     * @return 相等，则返回true；否则，返回false。
     */
    bool Equal(const char* str);

    /**
     * 判断两个字符串是否相等。
     * @param[in] str 字符串。
     * @return 相等，则返回true；否则，返回false。
     */
    bool Equal(CString& str);

    /**
     * 获得字符串长度。
     * @return 返回字符串长度。
     */
    size_t Length();

    /**
     * 获得C字符串。
     * @return 返回C字符串。
     */
    const char * GetCString() const;

    /**
     * 格式化字符串。
     * @param[in] format 格式。
     * @retur 返回格式化后的字符串。
     * @exception StringException 生成格式化字符串失败会抛出该异常。
     * @exception MemoryAllocationFailureException 内存分配失败会抛出该异常。
     */
    static CString Format(const char* format, ...);

private:

    char* mString;
    size_t mLength;

    /**
     * 初始化。
     * @param[in] str 字符串。
     */
    void Init(const char* str);

    /**
     * 销毁。
     */
    void Destroy();
};

bool operator== (const char* str1, CString& str2);
bool operator== (CString& str1, const char* str2);
bool operator== (CString& str1, CString& str2);

bool operator!= (const char* str1, CString& str2);
bool operator!= (CString& str1, const char* str2);
bool operator!= (CString& str1, CString& str2);

}

#endif //BWSIMPLEJNI_BWUTILS_CSTRING_H
