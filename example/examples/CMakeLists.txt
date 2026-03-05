
include(${CMAKE_SOURCE_DIR}/cmake/BuildExecutable.cmake)

build_executable(version)
build_executable(heap)


# concurrent
build_executable(cc-concurrent)