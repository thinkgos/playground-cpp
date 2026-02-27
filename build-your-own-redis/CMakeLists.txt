# 禁用源代码构建以防止源代码损坏, 一般使用build目录
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

cmake_minimum_required(VERSION 4.0)

project(BuildYourOwnRedis
    VERSION 0.1.0
)

add_subdirectory(server)
add_subdirectory(client)