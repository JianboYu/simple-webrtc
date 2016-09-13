AUX_SOURCE_DIRECTORY(. neteq_src)
INCLUDE_DIRECTORIES(../main
                    ../
                    ../../
                    ./inc
                    ./src ../codecs/opus/src/src/celt/
                    "../codecs/opus/src/src/include"
                    ../codecs/opus/src/src/src/
                  )
ADD_LIBRARY(libneteq STATIC ${neteq_src})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_PRJ_C_FLAGS} -std=c++11 -Wno-unused-local-typedefs -Wno-sign-compare")

if (NOT ANDROID)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif ()

MESSAGE(INFO " CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
SET_TARGET_PROPERTIES(libneteq PROPERTIES OUTPUT_NAME "libneteq")
