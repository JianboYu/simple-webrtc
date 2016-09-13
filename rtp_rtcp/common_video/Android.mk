LOCAL_PATH := $(call my-dir)

# Module declare context start
include $(CLEAR_VARS)
# Declare module name
LOCAL_MODULE := common_video-a
LOCAL_MODULE_FILENAME := common_video 

LOCAL_SRC_FILES := \
                ./bitrate_adjuster.cc \
                ./h264/sps_vui_rewriter.cc \
                ./h264/h264_common.cc \
                ./h264/pps_parser.cc \
                ./h264/sps_parser.cc \

#                ./i420_buffer_pool.cc \
                ./video_frame.cc \
                ./incoming_video_stream.cc \
                ./video_frame_buffer.cc \
                ./video_render_frames.cc \
                ./video_render_frames.h \
# End of src search 

LOCAL_LINK_MODE := c++
# Append common macro and cflags
LOCAL_CFLAGS := -DWEBRTC_POSIX -DWEBRTC_LINUX -DWEBRTC_ANDROID

# Append common cpulspuls flags
LOCAL_CPPFLAGS := -std=gnu++11

# Append common link flags
LOCAL_LDFLAGS :=

# Append common link libraries
LOCAL_LDLIBS := -llog

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
