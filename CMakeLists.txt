cmake_minimum_required(VERSION 3.15)
set(PROJECT_N algorithmBase)
project(${PROJECT_N} LANGUAGES CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake)
add_compilation_options_check()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
# set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../../build_install)
find_package(fmt CONFIG REQUIRED)
find_package(app REQUIRED)
find_package(spdlog REQUIRED)
find_package(Armadillo REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Boost COMPONENTS  filesystem REQUIRED)

file(GLOB root_header_files "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB root_src_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pcdtool.cpp 
    PROPERTIES
    COMPILE_FLAGS "-Wno-effc++ -Wno-sign-conversion -Wno-conversion -Wno-old-style-cast"
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/trans_forms_group.cpp 
    PROPERTIES
    COMPILE_FLAGS "-Wno-effc++"
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ukf.cpp 
    PROPERTIES
    COMPILE_FLAGS "-Wno-float-conversion"
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
    PROPERTIES
    COMPILE_FLAGS "-Wno-float-conversion"
)


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")
else()
    add_compile_options("-g3")
endif()

add_library(${PROJECT_N} ${root_header_files} ${root_src_files})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLOG REQUIRED libglog)
add_executable(demo main.cpp)

target_link_libraries(demo 
    PUBLIC 
    ${PROJECT_N}
    fmt::fmt
    app::app
    ${GLOG_LIBRARIES}
    spdlog::spdlog
    ${Boost_LIBRARIES}
    dl
    ${Protobuf_LIBRARIES}
)

target_include_directories(${PROJECT_N}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${ARMADILLO_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:include/${PROJECT_N}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_N} 
    PUBLIC 
    fmt::fmt
    app::app
    pthread
    ${ARMADILLO_LIBRARIES}
    spdlog::spdlog
    ${Boost_LIBRARIES}
    dl
    ${Protobuf_LIBRARIES}
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

