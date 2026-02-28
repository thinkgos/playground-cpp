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

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME})

file(GLOB SRC_FILES "*.cpp")
target_sources(${TARGET_NAME}
    PRIVATE
       ${SRC_FILES}
)

set(CMAKE_VERBOSE_MAKEFILE ON)

# 使用c++ 20
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

# 指定编译选项
if(
  (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR
  (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
  target_compile_options(${TARGET_NAME} PRIVATE /W3)
elseif(
  (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
  (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
  target_compile_options(${TARGET_NAME} PRIVATE -Wall)
endif()


# install

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install (
  TARGETS ${TARGET_NAME}
  EXPORT ${TARGET_NAME}Targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}ConfigVersion.cmake
  COMPATIBILITY ExactVersion
  ARCH_INDEPENDENT
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}
)
