SET(audio_signal_process_src auto_correlation.c
                            resample_48khz.c
                            ilbc_specific_functions.c
                            cross_correlation.c
                            resample_by_2_internal.c
                            complex_fft.c
                            lpc_to_refl_coef.c
                            dot_product_with_scale.c
                            vector_scaling_operations.c
                            spl_sqrt.c
                            levinson_durbin.c
                            energy.c
                            min_max_operations.c
                            filter_ma_fast_q12.c
                            downsample_fast.c
                            division_operations.c
                            copy_set_operations.c
                            resample_fractional.c
                            resample_by_2.c
                            spl_init.c
                            randomization_functions.c
                            auto_corr_to_refl_coef.c
                            get_scaling_square.c
                            sqrt_of_one_minus_x_squared.c
                            real_fft.c
                            refl_coef_to_lpc.c
                            spl_version.c
                            splitting_filter.c
                            resample.c
                            filter_ar.c
                            get_hanning_window.c
                            filter_ar_fast_q12.c
                            complex_bit_reverse.c
                            spl_sqrt_floor.c)

INCLUDE_DIRECTORIES("./include" ../../../../ ../../../main)
ADD_LIBRARY(libaudio_signal_process STATIC ${audio_signal_process_src})
if (NOT ANDROID)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWEBRTC_POSIX")

MESSAGE(INFO " libaudio_signal_process CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")

SET_TARGET_PROPERTIES(libaudio_signal_process PROPERTIES OUTPUT_NAME "libaudio_signal_process")
TARGET_LINK_LIBRARIES(libaudio_signal_process libsystem_wrappers)
