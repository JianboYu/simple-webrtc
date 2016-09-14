SET(audio_vad_src webrtc_vad.c
                      vad_gmm.c
                      vad_filterbank.c
                      vad_core.c
                      vad_sp.c)
INCLUDE_DIRECTORIES("./include" ../../../../ ../../../main)
ADD_LIBRARY(libaudio_vad STATIC ${audio_vad_src})
if (NOT ANDROID)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

MESSAGE(INFO " libaudio_vad CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")

SET_TARGET_PROPERTIES(libaudio_vad PROPERTIES OUTPUT_NAME "libaudio_vad")
TARGET_LINK_LIBRARIES(libaudio_vad libsystem_wrappers libaudio_signal_process)
