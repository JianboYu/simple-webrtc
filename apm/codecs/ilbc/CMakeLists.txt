SET(ilbc_src lsf_to_poly.c
             lsf_interpolate_to_poly_dec.c
             pack_bits.c
             encode.c
             init_encode.c
             refiner.c
             abs_quant_loop.c
             lsf_check.c
             poly_to_lsf.c
             get_sync_seq.c
             hp_output.c
             get_cd_vec.c
             enhancer_interface.c
             frame_classify.c
             abs_quant.c
             hp_input.c
             enh_upsample.c
             filtered_cb_vecs.c
             simple_interpolate_lsf.c
             cb_search_core.c
             enhancer.c
             gain_dequant.c
             smooth_out_data.c
             state_construct.c
             lpc_encode.c
             augmented_cb_corr.c
             smooth.c
             decode.c
             gain_quant.c
             lsf_interpolate_to_poly_enc.c
             simple_lsf_quant.c
             cb_mem_energy_calc.c
             unpack_bits.c
             get_lsp_poly.c
             index_conv_dec.c
             do_plc.c
             create_augmented_vec.c
             decode_residual.c
             chebyshev.c
             poly_to_lsp.c
             split_vq.c
             vq4.c
             interpolate.c
             swap_bytes.c
             init_decode.c
             xcorr_coef.c
             state_search.c
             comp_corr.c
             index_conv_enc.c
             interpolate_samples.c
             nearest_neighbor.c
             simple_lpc_analysis.c
             cb_search.c
             ilbc.c
             cb_update_best_index.c
             cb_construct.c
             simple_lsf_dequant.c
             decoder_interpolate_lsf.c
             bw_expand.c
             lsp_to_lsf.c
             cb_mem_energy.c
             my_corr.c
             vq3.c
             lsf_to_lsp.c
             sort_sq.c
             cb_mem_energy_augmentation.c
             window32_w32.c
             energy_inverse.c
             constants.c
)

INCLUDE_DIRECTORIES("./interface" ../../../ ../../main)
ADD_LIBRARY(libilbc STATIC ${ilbc_src})
if (NOT ANDROID)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

MESSAGE(INFO " libilbc CMAKE_CXX_FLAGS: ${CMAKE_C_FLAGS}")

SET_TARGET_PROPERTIES(libilbc PROPERTIES OUTPUT_NAME "libilbc")
TARGET_LINK_LIBRARIES(libilbc libsystem_wrappers)
