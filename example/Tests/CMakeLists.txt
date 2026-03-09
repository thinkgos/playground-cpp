
set(TARGET_NAME IntegrationTests)
find_package(Catch2 3 REQUIRED)

add_executable(${TARGET_NAME})

file(GLOB SRC_FILES "*.cc")
target_sources(${TARGET_NAME}
  PRIVATE
    ${SRC_FILES}
)

target_link_libraries(
    ${TARGET_NAME} 
    PRIVATE 
        Catch2::Catch2WithMain
)

# 自动测试注册
include(CTest)
include(Catch)
catch_discover_tests(${TARGET_NAME})
