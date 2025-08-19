cmake_minimum_required(VERSION 3.25)
project(Cpp_Design_Patterns)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp)

if(MSVC)
add_compile_options(/utf-8)
endif()

foreach(SRC_FILE ${ALL_SOURCE_FILES})
    get_filename_component(TARGET_NAME ${SRC_FILE} NAME_WE)
    get_filename_component(TARGET_DIR  ${SRC_FILE} DIRECTORY)

    if(TARGET ${TARGET_NAME})
        message(WARNING "Target '${TARGET_NAME}' already exists (from ${SRC_FILE}), skipping.")
        continue()
    endif()

    add_executable(${TARGET_NAME} ${SRC_FILE})
endforeach()