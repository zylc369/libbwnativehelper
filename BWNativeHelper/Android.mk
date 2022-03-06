# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


LOCAL_PATH := $(call my-dir)

local_src_files := \
    BWNativeHelper.cpp \
    BWNativeUtils.cpp \
    BWCommon.cpp

#
# Build for the target (device).
#

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
    $(local_src_files)
LOCAL_MODULE := libbwnativehelper
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_CLANG := true
LOCAL_CFLAGS := -Werror -fvisibility=protected
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include external/stlport/stlport bionic/ bionic/libstdc++/include libcore/include
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/ external/libcxx/include
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/ $(LOCAL_PATH)/../include/bwnativehelper external/libcxx/include
LOCAL_SHARED_LIBRARIES += libcutils libc++ libstlport libdl
LOCAL_WHOLE_STATIC_LIBRARIES = BWUtils JNIJava
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_SHARED_LIBRARY)

#
# Build for the host.
#

include $(CLEAR_VARS)
LOCAL_MODULE := libbwnativehelper
LOCAL_MODULE_TAGS := optional
LOCAL_CLANG := true
LOCAL_SRC_FILES := \
    $(local_src_files)
LOCAL_CFLAGS := -Werror -fvisibility=protected
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include libcore/include
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/ $(LOCAL_PATH)/../include/bwnativehelper external/libcxx/include
LOCAL_SHARED_LIBRARIES := liblog libc++
LOCAL_WHOLE_STATIC_LIBRARIES = BWUtils JNIJava
LOCAL_LDFLAGS := -ldl
ifeq ($(strip $(HOST_OS)),darwin)
LOCAL_LDFLAGS += -Wl,-lstdc++
endif
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk $(LOCAL_PATH)/../BWUtils/Android.mk $(LOCAL_PATH)/../JNIJava/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_HOST_SHARED_LIBRARY)

#
# Tests.
#

#include $(LOCAL_PATH)/tests/Android.mk
