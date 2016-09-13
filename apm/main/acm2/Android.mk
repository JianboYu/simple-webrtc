LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
# Declare module name
LOCAL_MODULE := acm-so
LOCAL_MODULE_FILENAME := libacm

LOCAL_SRC_FILES := ./acm_opus.cc \
                   ./acm_dtmf_playout.cc \
                   ./acm_g7221c.cc \
                   ./nack.cc \
                   ./acm_pcmu.cc \
                   ./acm_speex.cc \
                   ./acm_ilbc.cc \
                   ./acm_amr.cc \
                   ./acm_celt.cc \
                   ./initial_delay_manager.cc \
                   ./acm_g722.cc \
                   ./acm_g729.cc \
                   ./acm_generic_codec.cc \
                   ./acm_g7221.cc \
                   ./acm_g7291.cc \
                   ./call_statistics.cc \
                   ./acm_red.cc \
                   ./audio_coding_module_impl.cc \
                   ./acm_receiver.cc \
                   ./acm_pcm16b.cc \
                   ./acm_resampler.cc \
                   ./acm_pcma.cc \
                   ./acm_amrwb.cc \
                   ./acm_gsmfr.cc \
                   ./audio_coding_module.cc \
                   ./acm_codec_database.cc \
                   ./acm_cng.cc \

#                   ../base/clock.cc \
                   ../base/checks.cc \
                   ../base/logging.cc
                   
#./acm_isac.cc \
# End of src search 

LOCAL_LINK_MODE := c++
# Append common macro and cflags
LOCAL_CFLAGS :=
LOCAL_CFLAGS += $(defines) -std=gnu++11
LOCAL_CFLAGS += $(cflags) -DWEBRTC_ANDROID -DWEBRTC_LINUX  -UNDEBUG

# Append common cpulspuls flags
LOCAL_CPPFLAGS :=
LOCAL_CPPFLAGS += $(cppflags)

# Append common link flags
LOCAL_LDFLAGS :=
LOCAL_LDFLAGS += $(ldflags)

# Append common link libraries
LOCAL_LDLIBS :=
LOCAL_LDLIBS += $(ldlibs) -llog

# If need be, the comments can be removed
# LOCAL_CPP_EXTENSION :=
# LOCAL_DEPS_MODULES :=
# LOCAL_WHOLE_STATIC_LIBRARIES :=
# LOCAL_SHARED_LIBRARIES :=
# LOCAL_STATIC_LIBRARIES :=
# LOCAL_EXPORT_CFLAGS :=
# LOCAL_EXPORT_CPPFLAGS :=
# LOCAL_EXPORT_LDFLAGS :=

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(heads)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
	                $(LOCAL_PATH)/interface \
                  $(LOCAL_PATH)/../../ \
                  $(LOCAL_PATH)/../

LOCAL_STATIC_LIBRARIES := \
               system_wrappers-a  \
               common_audio-a \
               audio_resampler-a \
							 signal_processing-a \
               neteq-a \
							 opus-a \
							 opus_codec-a \
							 g711-a \
							 g722-a \
               ilbc-a \
							 pcm16b-a \
               isac-a

#LOCAL_SHARED_LIBRARIES := \
               system_wrappers-a  \

# Export include path
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under, \
					$(LOCAL_PATH)/../common_audio/Android.mk \
)
