function(build_executable target)
    add_executable(${target})
    target_sources(${target}
        PRIVATE
        ${target}.cpp
    )

    # 使用c++ 20
    target_compile_features(${target} PRIVATE cxx_std_20)

    # 指定编译选项
    if(
    (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR
    (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    )
    target_compile_options(${target} PRIVATE /W3)
    elseif(
    (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
    (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    )
    target_compile_options(${target} PRIVATE -Wall)
    endif()

    # install

    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install (
    TARGETS ${target}
    EXPORT ${target}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${target}ConfigVersion.cmake
    COMPATIBILITY ExactVersion
    ARCH_INDEPENDENT
    )

    install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${target}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${target}
    )
endfunction()

build_executable(version)
