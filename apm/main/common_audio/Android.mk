# Auto genarate templements
# Author :By lichao
# Time   :Thu Dec 25 12:02:23 CST 2014
# -------------------------------------------------
LOCAL_PATH := $(call my-dir)

# These variables by script auto genarate. In order 
# to reduce the current make writing burden.
# Incomming common head path.
heads := $(call get-common-defines,heads)
# Incomming common cflags
cflags := $(call get-common-defines,cflags)
# Incomming common cppflags
cppflags := $(call get-common-defines,cppflags)
# Incomming common ldflags 
ldflags := $(call get-common-defines,ldflags)
# Incomming common ldlibs
ldlibs := $(call get-common-defines,ldlibs)
# Incomming common macro 
defines := $(call get-common-defines,defines)
# ------------------------------------------------

# Module declare context start
include $(CLEAR_VARS)
# Declare module name
LOCAL_MODULE := common_audio-a
LOCAL_MODULE_FILENAME := common_audio

# LOCAL_SRC_FILES := ./real_fourier.cc \
                   ./lapped_transform.cc 

LOCAL_SRC_FILES += ./window_generator.cc \
                   ./audio_util.cc \
                   ./blocker.cc \
                   ./fir_filter.cc \
                   ./wav_writer.cc \
                   ./wav_header.cc \
                   ./channel_buffer.cc
# End of src search 

#ifneq (,$(filter x86 x64 x86_64,$(TARGET_ARCH)))
#   LOCAL_SRC_FILES += ./fir_filter_sse.cc
#else
#   LOCAL_SRC_FILES += ./fir_filter_neon.cc
#endif

LOCAL_LINK_MODE := c++
# Append common macro and cflags
LOCAL_CFLAGS :=
LOCAL_CFLAGS += $(defines) -std=gnu++11
LOCAL_CFLAGS += $(cflags) 
ifneq (,$(filter x86 x64 x86_64,$(TARGET_ARCH)))
else
#LOCAL_CFLAGS += -mfloat-abi=softfp -mfpu=neon
endif


# Append common cpulspuls flags
LOCAL_CPPFLAGS :=
LOCAL_CPPFLAGS += $(cppflags)

# Append common link flags
LOCAL_LDFLAGS :=
LOCAL_LDFLAGS += $(ldflags)

# Append common link libraries
LOCAL_LDLIBS :=
LOCAL_LDLIBS += $(ldlibs)

# If need be, the comments can be removed
# LOCAL_CPP_EXTENSION :=
# LOCAL_DEPS_MODULES :=
# LOCAL_WHOLE_STATIC_LIBRARIES :=
# LOCAL_SHARED_LIBRARIES :=
LOCAL_STATIC_LIBRARIES := system_wrappers-a
# LOCAL_EXPORT_CFLAGS :=
# LOCAL_EXPORT_CPPFLAGS :=
# LOCAL_EXPORT_LDFLAGS :=

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(heads)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
	                $(LOCAL_PATH)/interface \
                  $(LOCAL_PATH)/../ \

# Export include path
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_STATIC_LIBRARY)

include $(call all-makefiles-under, \
					$(LOCAL_PATH)/resampler/Android.mk \
					$(LOCAL_PATH)/signal_processing/Android.mk \
					$(LOCAL_PATH)/vad/Android.mk \
)
