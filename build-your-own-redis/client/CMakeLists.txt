
project(client
    VERSION 0.1.0
)

# 设置编译目标名称
set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME}
    PRIVATE
        main.cpp
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
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra)
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
