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
LOCAL_MODULE := ilbc-a

LOCAL_SRC_FILES := ./lsf_to_poly.c \
                   ./lsf_interpolate_to_poly_dec.c \
                   ./pack_bits.c \
                   ./encode.c \
                   ./init_encode.c \
                   ./refiner.c \
                   ./abs_quant_loop.c \
                   ./lsf_check.c \
                   ./poly_to_lsf.c \
                   ./get_sync_seq.c \
                   ./hp_output.c \
                   ./get_cd_vec.c \
                   ./enhancer_interface.c \
                   ./frame_classify.c \
                   ./abs_quant.c \
                   ./hp_input.c \
                   ./enh_upsample.c \
                   ./filtered_cb_vecs.c \
                   ./simple_interpolate_lsf.c \
                   ./cb_search_core.c \
                   ./enhancer.c \
                   ./gain_dequant.c \
                   ./smooth_out_data.c \
                   ./state_construct.c \
                   ./lpc_encode.c \
                   ./augmented_cb_corr.c \
                   ./smooth.c \
                   ./decode.c \
                   ./gain_quant.c \
                   ./lsf_interpolate_to_poly_enc.c \
                   ./simple_lsf_quant.c \
                   ./cb_mem_energy_calc.c \
                   ./unpack_bits.c \
                   ./get_lsp_poly.c \
                   ./index_conv_dec.c \
                   ./do_plc.c \
                   ./create_augmented_vec.c \
                   ./decode_residual.c \
                   ./chebyshev.c \
                   ./poly_to_lsp.c \
                   ./split_vq.c \
                   ./vq4.c \
                   ./interpolate.c \
                   ./swap_bytes.c \
                   ./init_decode.c \
                   ./xcorr_coef.c \
                   ./state_search.c \
                   ./comp_corr.c \
                   ./index_conv_enc.c \
                   ./interpolate_samples.c \
                   ./nearest_neighbor.c \
                   ./simple_lpc_analysis.c \
                   ./cb_search.c \
                   ./ilbc.c \
                   ./cb_update_best_index.c \
                   ./cb_construct.c \
                   ./simple_lsf_dequant.c \
                   ./decoder_interpolate_lsf.c \
                   ./bw_expand.c \
                   ./lsp_to_lsf.c \
                   ./cb_mem_energy.c \
                   ./my_corr.c \
                   ./vq3.c \
                   ./lsf_to_lsp.c \
                   ./sort_sq.c \
                   ./cb_mem_energy_augmentation.c \
                   ./window32_w32.c \
                   ./energy_inverse.c \
                   ./constants.c \
# End of src search 

LOCAL_LINK_MODE := c
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
LOCAL_STATIC_LIBRARIES := signal_processing-a
# LOCAL_EXPORT_CFLAGS :=
# LOCAL_EXPORT_CPPFLAGS :=
# LOCAL_EXPORT_LDFLAGS :=

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(heads)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
	                $(LOCAL_PATH)/interface \
	                $(LOCAL_PATH)/../../ \
	                $(LOCAL_PATH)/../../main \

# Export include path
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_STATIC_LIBRARY)
