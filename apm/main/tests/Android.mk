LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := audio_coding_test
LOCAL_SRC_FILES :=  Channel.cc \
										PCMFile.cc  \
										TestAllCodecs.cc  \
										Test.cc
LOCAL_CFLAGS := -UNDEBUG -std=gnu++11
LOCAL_LDFLAGS := -pie -fPIE
LOCAL_SHARED_LIBRARIES := acm-so

LOCAL_LINK_MODE := c++
include $(BUILD_EXECUTABLE)
