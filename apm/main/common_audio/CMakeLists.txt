SET(common_audio_src window_generator.cc
                     audio_util.cc
                     blocker.cc
                     fir_filter.cc
                     wav_writer.cc
                     wav_header.cc
                     channel_buffer.cc)

INCLUDE_DIRECTORIES("./include" ../../../ ../../main)
ADD_LIBRARY(libcommon_audio STATIC ${common_audio_src})
if (NOT ANDROID)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_PRJ_C_FLAGS} -std=c++11 -Wno-unused-local-typedefs -Wno-sign-compare")
MESSAGE(INFO " libcommon_audio CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

SET_TARGET_PROPERTIES(libcommon_audio PROPERTIES OUTPUT_NAME "libcommon_audio")
TARGET_LINK_LIBRARIES(libcommon_audio libsystem_wrappers)

ADD_SUBDIRECTORY(resampler)
ADD_SUBDIRECTORY(signal_processing)
ADD_SUBDIRECTORY(vad)
