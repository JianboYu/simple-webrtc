AUX_SOURCE_DIRECTORY(. system_wrappers_src)
INCLUDE_DIRECTORIES(../interface ../../ ./spreadsortlib)
ADD_LIBRARY(libsystem_wrappers STATIC ${system_wrappers_src})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                    -std=c++11 \
                    -Wno-unused-local-typedefs \
                    -Wno-sign-compare \
                    -Wno-switch")
if (NOT ANDROID)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif ()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                     ${CMAKE_PRJ_C_FLAGS} \
                     -DWEBRTC_LINUX \
                     -DWEBRTC_POSIX")
MESSAGE(INFO " libsystem_wrappers CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
SET_TARGET_PROPERTIES(libsystem_wrappers PROPERTIES OUTPUT_NAME "libsystem_wrappers")
