
set(TARGET_NAME IntegrationTests)
  find_package(GTest REQUIRED)

add_executable(${TARGET_NAME})

file(GLOB_RECURSE TEST_SRC_FILES "*.cxx")
target_sources(${TARGET_NAME}
  PRIVATE
    ${TEST_SRC_FILES}
)

target_link_libraries(
    ${TARGET_NAME} 
    PRIVATE 
      GTest::gtest 
      GTest::gtest_main 
)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

# 自动测试注册
include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
