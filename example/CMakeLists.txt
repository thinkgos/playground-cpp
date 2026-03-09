# 禁用源代码构建以防止源代码损坏, 一般使用build目录
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

cmake_minimum_required(VERSION 4.0)

project(example
    VERSION 0.1.0
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(BUILD_FLAGS "-O0 -g3 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize-address-use-after-scope")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${BUILD_FLAGS}")
endif()

# test
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(Tests)
endif()

add_subdirectory(examples)