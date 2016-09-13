# Auto genarate templements
# Author :By lichao
# Time   :Thu Dec 25 12:02:24 CST 2014
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
LOCAL_MODULE := signal_processing-a

LOCAL_SRC_FILES := ./auto_correlation.c \
                   ./resample_48khz.c \
                   ./ilbc_specific_functions.c \
                   ./cross_correlation.c \
                   ./resample_by_2_internal.c \
                   ./complex_fft.c \
                   ./lpc_to_refl_coef.c \
                   ./dot_product_with_scale.c \
                   ./vector_scaling_operations.c \
                   ./spl_sqrt.c \
                   ./levinson_durbin.c \
                   ./energy.c \
                   ./min_max_operations.c \
                   ./filter_ma_fast_q12.c \
                   ./downsample_fast.c \
                   ./division_operations.c \
                   ./copy_set_operations.c \
                   ./resample_fractional.c \
                   ./resample_by_2.c \
                   ./spl_init.c \
                   ./randomization_functions.c \
                   ./auto_corr_to_refl_coef.c \
                   ./get_scaling_square.c \
                   ./sqrt_of_one_minus_x_squared.c \
                   ./real_fft.c \
                   ./refl_coef_to_lpc.c \
                   ./spl_version.c \
                   ./splitting_filter.c \
                   ./resample.c \
                   ./filter_ar.c \
				   ./get_hanning_window.c  \
					./filter_ar_fast_q12.c


# End of src search 

#ifneq (,$(filter arm,$(TARGET_ARCH)))
# LOCAL_SRC_FILES += ./complex_bit_reverse_arm.S \
#					./spl_sqrt_floor_arm.S \
#                     
#else
# LOCAL_SRC_FILES += ./complex_bit_reverse.c \
#					./spl_sqrt_floor.c
#endif
LOCAL_SRC_FILES += ./complex_bit_reverse.c \
					./spl_sqrt_floor.c

ifeq (armeabi-v7a,$(TARGET_ABI))
#LOCAL_SRC_FILES += ./downsample_fast_neon.S \
				     ./min_max_operations_neon.S \
					 ./vector_scaling_operations_neon.S \
                     ./cross_correlation_neon.S \
					 ./filter_ar_fast_q12_armv7.S
endif

LOCAL_LINK_MODE := c
# Append common macro and cflags
LOCAL_CFLAGS := -DWEBRTC_POSIX
LOCAL_CFLAGS += $(defines)
LOCAL_CFLAGS += $(cflags) 

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
# LOCAL_STATIC_LIBRARIES :=
# LOCAL_EXPORT_CFLAGS :=
# LOCAL_EXPORT_CPPFLAGS :=
# LOCAL_EXPORT_LDFLAGS :=

LOCAL_STATIC_LIBRARIES := system_wrappers-a

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(heads)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
	                $(LOCAL_PATH)/interface \
									$(LOCAL_PATH)/../../ \
									$(LOCAL_PATH)/../../../ \

# Export include path
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_STATIC_LIBRARY)
