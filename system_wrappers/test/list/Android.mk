# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_MODULE := system_wrapper_list_test
LOCAL_MODULE_TAGS := optional
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := \
    list.cc \

LOCAL_WHOLE_STATIC_LIBRARIES := \
		system_wrappers-a

LOCAL_CPPFLAGS := -std=gnu++11
LOCAL_C_INCLUDES := \
						$(LOCAL_PATH)/../../interface/

include $(BUILD_EXECUTABLE)
