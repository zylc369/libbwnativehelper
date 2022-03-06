#ifndef BWSIMPLEJNI_BWLOG_H
#define BWSIMPLEJNI_BWLOG_H

#include <android/log.h>

/**
 * 日志框架。
 * 用途：输入日志。
 */

/*
 用法：
# 定义日志标签。
DEFAULT_LOG_TAG := \"debug\"

# 根据构建类型定义默认日志等级。
ifeq ($(APP_OPTIM),release)
DEFAULT_LOG_LEVEL := LOG_LEVEL_ERROR
else
DEFAULT_LOG_LEVEL := LOG_LEVEL_VERBOSE
endif

# 追加编译标识。
LOCAL_CFLAGS += -DDEFAULT_LOG_TAG=$(DEFAULT_LOG_TAG)
LOCAL_CFLAGS += -DDEFAULT_LOG_LEVEL=$(DEFAULT_LOG_LEVEL)

# 动态链接日志库。
LOCAL_LDLIBS += -llog

// 详细格式输出宏
LOG_DETAIL

 */

#define LOG_LEVEL_VERBOSE 1
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_WARNING 4
#define LOG_LEVEL_ERROR 5
#define LOG_LEVEL_FATAL 6
#define LOG_LEVEL_SILENT 7

#ifndef DEFAULT_LOG_LEVEL
# define DEFAULT_LOG_LEVEL LOG_LEVEL_VERBOSE
#endif

#ifndef DEFAULT_LOG_TAG
# define DEFAULT_LOG_TAG "debug"
#endif

#define LOG_NOOP (void) 0

// 是否已详细格式输出？
#ifdef LOG_DETAIL
# define BWLOG_PRINT(level,fmt,...) \
 __android_log_print(level, DEFAULT_LOG_TAG, "{%s:%u} %s: [" fmt "]", \
 __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
# else
# define BWLOG_PRINT(level,fmt,...) \
 __android_log_print(level, DEFAULT_LOG_TAG, fmt, ##__VA_ARGS__)
#endif

# define BWLOG_VPRINT(level,fmt,ap) \
 __android_log_vprint(level, DEFAULT_LOG_TAG, fmt, ap)

#if LOG_LEVEL_VERBOSE >= DEFAULT_LOG_LEVEL
# define BWLOGV(fmt,...) \
BWLOG_PRINT(ANDROID_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#else
# define BWLOGV(...) LOG_NOOP
#endif

#if LOG_LEVEL_DEBUG >= DEFAULT_LOG_LEVEL
# define BWLOGD(fmt,...) \
BWLOG_PRINT(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)
#else
# define BWLOGD(...) LOG_NOOP
#endif

#if LOG_LEVEL_INFO >= DEFAULT_LOG_LEVEL
# define BWLOGI(fmt,...) \
BWLOG_PRINT(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)
#else
# define BWLOGI(...) LOG_NOOP
#endif

#if LOG_LEVEL_WARNING >= DEFAULT_LOG_LEVEL
# define BWLOGW(fmt,...) \
BWLOG_PRINT(ANDROID_LOG_WARN, fmt, ##__VA_ARGS__)
#else
#   define BWLOGW(...) LOG_NOOP
#endif

#if LOG_LEVEL_ERROR >= DEFAULT_LOG_LEVEL
#  define BWLOGE(fmt,...)\
     BWLOG_PRINT(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)
# else
# define BWLOGE(...) LOG_NOOP
#endif

#if LOG_LEVEL_FATAL >= DEFAULT_LOG_LEVEL
# define BWLOG_FATAL(fmt,...) \
BWLOG_PRINT(ANDROID_LOG_FATAL, fmt, ##__VA_ARGS__)
#else
# define BWLOG_FATAL(...) LOG_NOOP
#endif

#if LOG_LEVEL_FATAL >= DEFAULT_LOG_LEVEL
# define BWLOG_ASSERT(expression, fmt, ...) \
if (!(expression)) \
{ \
__android_log_assert(#expression, DEFAULT_LOG_TAG, \
fmt, ##__VA_ARGS__); \
}
#else
# define BWLOG_ASSERT(...) LOG_NOOP
#endif

#endif //BWSIMPLEJNI_BWLOG_H
