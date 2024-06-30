cmake_minimum_required(VERSION 3.15)
set(PROJECT_N algorithmBase)
project(${PROJECT_N} LANGUAGES CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../../build_install)
find_package(fmt REQUIRED)
find_package(app REQUIRED)
file(GLOB root_header_files "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB root_src_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_compile_options(
    -Wall            # 启用所有常见警告
    -Wextra          # 启用额外的警告
    -Werror          # 将警告视为错误
    -pedantic        # 严格遵循C标准
    -Wshadow         # 警告变量名被隐藏的情况
    -Wno-conversion  # 忽略隐式转换警告
    -Wsign-conversion # 警告有符号和无符号之间的隐式转换
    -Wno-unused      # 忽略未使用警告
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/pcdtool.cpp PROPERTIES
    COMPILE_FLAGS "-Wno-sign-conversion")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")
else()
    add_compile_options("-g3")
endif()

add_library(${PROJECT_N} ${root_header_files} ${root_src_files})

add_executable(demo main.cpp)

target_link_libraries(demo 
    PUBLIC 
    ${PROJECT_N}
    fmt::fmt
    app::app
)

target_include_directories(${PROJECT_N}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_N}>
)

target_link_libraries(${PROJECT_N} 
    PUBLIC 
    fmt::fmt
    app::app
    pthread
)

option(BUILD_TESTS "Build Tests" OFF)
if(BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS ${PROJECT_N} 
            EXPORT ${PROJECT_N}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
)

install(FILES
            ${root_header_files} 
            DESTINATION include/${PROJECT_N}
)

install(EXPORT
            ${PROJECT_N}
            NAMESPACE  "algorithmBase::"
            DESTINATION  lib/cmake/${PROJECT_N}
)

install(DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/include/transforms3d
            ${CMAKE_CURRENT_SOURCE_DIR}/include/sophus
            DESTINATION include/${PROJECT_N}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_N}Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_N}Config.cmake
        INSTALL_DESTINATION lib/cmake
)

install(FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_N}Config.cmake DESTINATION lib/cmake/${PROJECT_N})

