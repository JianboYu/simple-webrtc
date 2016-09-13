SET(audio_resampler_src push_resampler.cc
                        resampler.cc
                        push_sinc_resampler.cc
                        sinusoidal_linear_chirp_source.cc
                        sinc_resampler.cc
                        sinc_resampler_sse.cc 
)

INCLUDE_DIRECTORIES("./include" ../../../../ ../../../main)
ADD_LIBRARY(libaudio_resampler STATIC ${audio_resampler_src})
if (NOT ANDROID)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_PRJ_C_FLAGS} -std=c++11 -Wno-unused-local-typedefs -Wno-sign-compare")
MESSAGE(INFO " libaudio_resampler CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

SET_TARGET_PROPERTIES(libaudio_resampler PROPERTIES OUTPUT_NAME "libaudio_resampler")
TARGET_LINK_LIBRARIES(libaudio_resampler libsystem_wrappers)
