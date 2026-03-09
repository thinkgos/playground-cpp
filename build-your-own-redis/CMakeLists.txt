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

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(BUILD_FLAGS "-O0 -g3 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fsanitize-recover=address")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${BUILD_FLAGS}")
endif()

add_subdirectory(container)
add_subdirectory(server)
add_subdirectory(client)

# 测试
if(BUILD_TESTING)
  enable_testing()

  # 单元测试
  set(TEST_TARGET_NAME UnitTests)
  find_package(GTest REQUIRED)

  add_executable(${TEST_TARGET_NAME})

  file(GLOB_RECURSE TEST_SRC_FILES "*.cxx")
  target_sources(${TEST_TARGET_NAME}
    PRIVATE
      ${TEST_SRC_FILES}
  )
  target_link_libraries(
    ${TEST_TARGET_NAME} 
    PRIVATE 
      GTest::gtest 
      GTest::gtest_main 
      container
  )

  target_compile_features(${TEST_TARGET_NAME} PRIVATE cxx_std_20)

  include(GoogleTest)
  gtest_discover_tests(${TEST_TARGET_NAME})

  # 集成测试
endif()