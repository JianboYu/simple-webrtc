LOCAL_PATH := $(call my-dir)

# Module declare context start
include $(CLEAR_VARS)
# Declare module name
LOCAL_MODULE := rtp_rtcp_base-a
LOCAL_MODULE_FILENAME := rtp_rtcp_base

LOCAL_SRC_FILES := \
                  ./platform_file.cc \
                  ./rate_limiter.cc \
                  ./rate_statistics.cc \
                  ./rtc_event_log.cc \
                  ./timestamp_extrapolator.cc \
                  ./metrics_default.cc \
                  ./rtp_rtcp_common_types.cc \

# End of src search 

LOCAL_LINK_MODE := c++
# Append common macro and cflags
LOCAL_CFLAGS := -DWEBRTC_POSIX -DWEBRTC_ANDROID

# Append common cpulspuls flags
LOCAL_CPPFLAGS := -std=gnu++11

# Append common link flags
LOCAL_LDFLAGS :=

# Append common link libraries
LOCAL_LDLIBS :=

# If need be, the comments can be removed
# LOCAL_CPP_EXTENSION :=
# LOCAL_DEPS_MODULES :=
# LOCAL_WHOLE_STATIC_LIBRARIES :=
# LOCAL_SHARED_LIBRARIES :=
# LOCAL_STATIC_LIBRARIES :=
# LOCAL_EXPORT_CPPFLAGS :=
# LOCAL_EXPORT_LDFLAGS :=

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(heads)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
	                $(LOCAL_PATH)/interface \
                  $(LOCAL_PATH)/../

include $(BUILD_STATIC_LIBRARY)
