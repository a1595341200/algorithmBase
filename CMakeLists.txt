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
file(GLOB root_header_files "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB root_src_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_N} SHARED ${root_header_files} ${root_src_files})

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
)

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

include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_N}Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_N}Config.cmake
        INSTALL_DESTINATION lib/cmake
)

install(FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_N}Config.cmake DESTINATION lib/cmake/${PROJECT_N})

