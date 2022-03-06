#ifndef _BW_MACRO_H_
#define _BW_MACRO_H_

#include <stdint.h>

/**
 * BW_CUSTOM_ROM_ENABLE当宏的值被置1时，自定义ROM的功能启动。
 */
#define BW_CUSTOM_ROM_ENABLE 1

/**
 * 日志的tag。
 */
#define BW_LOG_TAG "BWDebug"

/**
 * BW模块拥有的根目录。
 */
#define BW_ROOT_DIR "/data/local/bw"

/**
 * BW模块数据库目录。
 */
#define BW_DB_DIR (BW_ROOT_DIR"/db")

/**
 * BW模块的日志目录。
 */
#define BW_LOG_DIR (BW_ROOT_DIR"/log")

/**
 * 计算数组元素个数。只能用于数组，绝不能用于指针。
 */
#define array_size(arr) (sizeof(arr)/sizeof((arr)[0]))

// #define TRACE_METHOD_INFO_FLAG_UNKNOW			    0
// /**
//  * 这个标志指定启用跟踪信息。
//  */
// #define TRACE_METHOD_INFO_FLAG_ENABLE               1
// /**
//  * 这个标志指定需要打印调用堆栈。
//  */
// #define TRACE_METHOD_INFO_FLAG_PRINTCALLSTACK       (1 << 1)
// /**
//  * 这个标志指定跟踪信息是可传递的。
//  */
// #define TRACE_METHOD_INFO_TRANSITIVE				(1 << 2)

// /**
//  * 无效。
//  * 通常的原因是查询失败，或者还未设置flags标志。
//  */
// #define BW_DUMP_FLAGS_INVALID                       ((int32_t)(1 << 31))
// /**
//  * 禁用dump。
//  */
// #define BW_DUMP_FLAGS_DISABLE                        0
// /**
//  * 打印bw日志。
//  */
// #define BW_DUMP_FLAGS_PRINT                          1
// /**
//  * 将bw日志写入文件。
//  */
// #define BW_DUMP_FLAGS_WRITE_FILE                     (1 << 1)

// #define BW_DUMP_FLAGS_ALL (BW_DUMP_FLAGS_PRINT | BW_DUMP_FLAGS_WRITE_FILE)

#define BWDEBUG 1

#endif
