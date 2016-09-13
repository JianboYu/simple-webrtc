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
LOCAL_MODULE := neteq-a
LOCAL_MODULE_FILENAME := neteq

LOCAL_SRC_FILES := ./delay_peak_detector.cc \
                   ./background_noise.cc \
                   ./merge.cc \
                   ./neteq.cc \
                   ./decoder_database.cc \
                   ./dtmf_tone_generator.cc \
                   ./normal.cc \
                   ./timestamp_scaler.cc \
                   ./time_stretch.cc \
                   ./expand.cc \
                   ./dsp_helper.cc \
                   ./random_vector.cc \
                   ./neteq_impl.cc \
                   ./delay_manager.cc \
                   ./comfort_noise.cc \
                   ./preemptive_expand.cc \
                   ./rtcp.cc \
                   ./accelerate.cc \
                   ./decision_logic_normal.cc \
                   ./statistics_calculator.cc \
                   ./dtmf_buffer.cc \
                   ./payload_splitter.cc \
                   ./audio_decoder.cc \
                   ./decision_logic.cc \
                   ./buffer_level_filter.cc \
                   ./packet_buffer.cc \
                   ./audio_decoder_impl.cc \
                   ./sync_buffer.cc \
                   ./audio_multi_vector.cc \
                   ./decision_logic_fax.cc \
                   ./audio_vector.cc \
                   ./audio_classifier.cc \
                   ./post_decode_vad.cc \
# End of src search 

LOCAL_LINK_MODE := c++
# Append common macro and cflags
LOCAL_CFLAGS :=
LOCAL_CFLAGS += $(defines)
LOCAL_CFLAGS += $(cflags) 

# Append common cpulspuls flags
LOCAL_CPPFLAGS :=
LOCAL_CPPFLAGS += $(cppflags) -std=gnu++11

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
LOCAL_STATIC_LIBRARIES :=  \
                          system_wrappers-a \
                          opus_codec-a \
													signal_processing-a \
													vad-a \
													cng-a \
                          isac-a \
# LOCAL_EXPORT_CFLAGS :=
# LOCAL_EXPORT_CPPFLAGS :=
# LOCAL_EXPORT_LDFLAGS :=

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(heads)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
	                $(LOCAL_PATH)/interface \
                  $(LOCAL_PATH)/../ \
                  $(LOCAL_PATH)/../main/ \

# Export include path
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_STATIC_LIBRARY)
