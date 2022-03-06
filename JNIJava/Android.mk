LOCAL_PATH := $(call my-dir)

SRC_FILES=./JStringHelper.cpp ./JNIReflect.cpp

C_INCLUDES=$(LOCAL_PATH)/../include external/libcxx/include

include $(CLEAR_VARS)

LOCAL_MODULE := JNIJava
LOCAL_CLANG := true
LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES := $(C_INCLUDES)
LOCAL_CFLAGS := -Werror -fvisibility=protected
LOCAL_SRC_FILES := $(SRC_FILES)
LOCAL_MULTILIB := both
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := JNIJava
LOCAL_CLANG := true
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -Werror -fvisibility=protected
LOCAL_C_INCLUDES := $(C_INCLUDES)

LOCAL_SRC_FILES := $(SRC_FILES)
LOCAL_MULTILIB := both
include $(BUILD_HOST_STATIC_LIBRARY)
