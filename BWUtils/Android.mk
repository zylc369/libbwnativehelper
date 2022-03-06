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
	CFile.cpp \
    CString.cpp \
	ExceptionBase.cpp \
	Exceptions.cpp \
	FileDir.cpp \
	StringHelper.cpp

#
# Build for the target (device).
#

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_CLANG := true
LOCAL_MODULE := BWUtils
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include external/stlport/stlport bionic/ bionic/libstdc++/include libcore/include
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include external/stlport/stlport bionic/ bionic/libstdc++/include libcore/include external/libcxx/include
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include external/libcxx/include

LOCAL_SRC_FILES := \
    $(local_src_files)

LOCAL_CPPFLAGS := -frtti -fexceptions
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_STATIC_LIBRARY)

#
# Build for the host.
#

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_CLANG := true
LOCAL_MODULE := BWUtils
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include external/stlport/stlport bionic/ bionic/libstdc++/include libcore/include
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include external/stlport/stlport
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include external/stlport/stlport bionic/ bionic/libstdc++/include libcore/include external/libcxx/include
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include external/libcxx/include
LOCAL_SRC_FILES := \
    $(local_src_files)

LOCAL_MULTILIB := both

LOCAL_CPPFLAGS := -frtti -fexceptions
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
include $(BUILD_HOST_STATIC_LIBRARY)
