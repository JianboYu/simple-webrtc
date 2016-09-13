SET(opus_src opus_interface.c)
INCLUDE_DIRECTORIES("./include"
                     ../../../
                     ../../main
                     "./src/src/include"
)
ADD_LIBRARY(libopus STATIC ${opus_src})
if (NOT ANDROID)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()
MESSAGE(INFO " libopus CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")

SET_TARGET_PROPERTIES(libopus PROPERTIES OUTPUT_NAME "libopus")
TARGET_LINK_LIBRARIES(libopus libsystem_wrappers)

ADD_SUBDIRECTORY(src)
