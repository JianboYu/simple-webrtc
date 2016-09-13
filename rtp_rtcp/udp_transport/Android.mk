LOCAL_PATH := $(call my-dir)

# Module declare context start
include $(CLEAR_VARS)
# Declare module name
LOCAL_MODULE := udp_transport-so
LOCAL_MODULE_FILENAME := libudp_transport

LOCAL_SRC_FILES := \
                  ./source/udp_socket_manager_posix.cc \
                  ./source/udp_socket_manager_wrapper.cc \
                  ./source/udp_socket_posix.cc \
                  ./source/udp_socket_wrapper.cc \
                  ./source/udp_transport_impl.cc \

#                 ./source/traffic_control_win.cc \
                  ./source/udp_socket2_win.cc \
                  ./source/udp_socket2_manager_win.cc \

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
LOCAL_STATIC_LIBRARIES := system_wrappers-a
# LOCAL_EXPORT_CPPFLAGS :=
# LOCAL_EXPORT_LDFLAGS :=

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(heads)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
	                $(LOCAL_PATH)/interface \
                  $(LOCAL_PATH)/../

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := udp_transport_test
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := \
                ./tests/test.cc \

LOCAL_SHARED_LIBRARIES := udp_transport-so \
                          system_wrappers-so

LOCAL_CPPFLAGS := -std=gnu++11

include $(BUILD_EXECUTABLE)

