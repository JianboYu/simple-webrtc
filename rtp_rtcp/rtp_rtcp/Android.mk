LOCAL_PATH := $(call my-dir)

# Module declare context start
include $(CLEAR_VARS)
# Declare module name
LOCAL_MODULE := rtp_rtcp-so
LOCAL_MODULE_FILENAME := libmedianet

LOCAL_SRC_FILES := \
                  ./source/fec_receiver_impl.cc \
                  ./source/packet_loss_stats.cc \
                  ./source/playout_delay_oracle.cc \
                  ./source/receive_statistics_impl.cc \
                  ./source/remote_ntp_time_estimator.cc \
                  ./source/rtcp_packet.cc \
                  ./source/rtcp_packet/app.cc \
                  ./source/rtcp_packet/bye.cc \
                  ./source/rtcp_packet/common_header.cc \
                  ./source/rtcp_packet/compound_packet.cc \
                  ./source/rtcp_packet/dlrr.cc \
                  ./source/rtcp_packet/extended_jitter_report.cc \
                  ./source/rtcp_packet/extended_reports.cc \
                  ./source/rtcp_packet/fir.cc \
                  ./source/rtcp_packet/nack.cc \
                  ./source/rtcp_packet/pli.cc \
                  ./source/rtcp_packet/psfb.cc \
                  ./source/rtcp_packet/rapid_resync_request.cc \
                  ./source/rtcp_packet/receiver_report.cc \
                  ./source/rtcp_packet/remb.cc \
                  ./source/rtcp_packet/report_block.cc \
                  ./source/rtcp_packet/rpsi.cc \
                  ./source/rtcp_packet/rrtr.cc \
                  ./source/rtcp_packet/rtpfb.cc \
                  ./source/rtcp_packet/sdes.cc \
                  ./source/rtcp_packet/sender_report.cc \
                  ./source/rtcp_packet/sli.cc \
                  ./source/rtcp_packet/tmmb_item.cc \
                  ./source/rtcp_packet/tmmbn.cc \
                  ./source/rtcp_packet/tmmbr.cc \
                  ./source/rtcp_packet/transport_feedback.cc \
                  ./source/rtcp_packet/voip_metric.cc \
                  ./source/rtcp_receiver.cc \
                  ./source/rtcp_receiver_help.cc \
                  ./source/rtcp_sender.cc \
                  ./source/rtcp_utility.cc \
                  ./source/rtp_header_extension.cc \
                  ./source/rtp_header_extensions.cc \
                  ./source/rtp_header_parser.cc \
                  ./source/rtp_packet.cc \
                  ./source/rtp_receiver_impl.cc \
                  ./source/rtp_rtcp_impl.cc \
                  ./source/rtp_sender.cc \
                  ./source/rtp_utility.cc \
                  ./source/ssrc_database.cc \
                  ./source/time_util.cc \
                  ./source/tmmbr_help.cc \
                  ./source/dtmf_queue.cc \
                  ./source/rtp_receiver_audio.cc \
                  ./source/rtp_sender_audio.cc \
                  ./source/forward_error_correction.cc \
                  ./source/forward_error_correction_internal.cc \
                  ./source/producer_fec.cc \
                  ./source/rtp_packet_history.cc \
                  ./source/rtp_payload_registry.cc \
                  ./source/rtp_receiver_strategy.cc \
                  ./source/rtp_receiver_video.cc \
                  ./source/rtp_sender_video.cc \
                  ./source/rtp_format.cc \
                  ./source/rtp_format_h264.cc \
                  ./source/rtp_format_vp8.cc \
                  ./source/rtp_format_vp9.cc \
                  ./source/rtp_format_video_generic.cc \
                  ./source/vp8_partition_aggregator.cc \

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
LOCAL_STATIC_LIBRARIES := rtp_rtcp_base-a \
                          system_wrappers-a \
                          common_video-a
# LOCAL_EXPORT_CPPFLAGS :=
# LOCAL_EXPORT_LDFLAGS :=

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(heads)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
	                $(LOCAL_PATH)/interface \
                  $(LOCAL_PATH)/../

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := rtp_rtcp_test
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := \
                ./test/testAPI-new/test_api.cc

LOCAL_SHARED_LIBRARIES := rtp_rtcp-so \
                          system_wrappers-so

LOCAL_CPPFLAGS := -std=gnu++11
LOCAL_CFLAGS := -DWEBRTC_POSIX -DWEBRTC_LINUX -DWEBRTC_ANDROID

include $(BUILD_EXECUTABLE)
