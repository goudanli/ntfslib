cmake_minimum_required(VERSION 3.6)
project(volumeTest)

set(CMAKE_C_STANDARD 99)

INCLUDE_DIRECTORIES(/ntfslib/include)

set(SOURCE_FILES main.c unix_io.c unix_io.h)

ADD_DEFINITIONS("-DHAVE_CONFIG_H")
add_executable(volumeTest ${SOURCE_FILES})

target_link_libraries(volumeTest "/ntfslib/lib/libntfs.a"  "-framework CoreFoundation")
set_target_properties(PROPERTIES LINK_FLAGS "-Wl,-F/System/Library/Frameworks")