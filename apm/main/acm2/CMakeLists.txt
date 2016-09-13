AUX_SOURCE_DIRECTORY(. acm2_src)
INCLUDE_DIRECTORIES(../ ../../ ../../../ ./inc ./src)
ADD_LIBRARY(libacm2 SHARED ${acm2_src})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_PRJ_C_FLAGS} -std=c++11 -Wno-unused-local-typedefs -Wno-sign-compare")
MESSAGE(INFO " CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
if (NOT ANDROID)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif ()

SET_TARGET_PROPERTIES(libacm2 PROPERTIES OUTPUT_NAME "libacm2")
TARGET_LINK_LIBRARIES(libacm2 libneteq libsystem_wrappers
                      libpcm16 libcng libg711 libg722 libilbc
                      libopus libopus_thd
                      libcommon_audio libaudio_resampler
                      libaudio_signal_process libaudio_vad)
