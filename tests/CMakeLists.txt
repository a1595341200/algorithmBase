# find_package(GTest CONFIG REQUIRED)
# set(ENV{PKG_CONFIG_PATH} "/path/to/your/pkgconfig")
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLOG REQUIRED IMPORTED_TARGET libglog)
pkg_check_modules(GTEST REQUIRED IMPORTED_TARGET gtest)
pkg_check_modules(GMOCK REQUIRED IMPORTED_TARGET gmock)

message("GLOG: ${GLOG_LIBRARIES}")
message("GTEST: ${GTEST_LIBRARIES}")

macro(BUILD curdir)
    file(GLOB children ${curdir}/*.cpp)
    message("Building ${children}")
    foreach(child ${children})
    get_filename_component(FILENAME ${child} NAME_WE)
    add_executable(${FILENAME} ${child})
    target_link_libraries(${FILENAME} PRIVATE
        ${PROJECT_N}
        # GTest::gtest
        # GTest::gmock
        # PkgConfig::GLOG
        # PkgConfig::GTEST
        # PkgConfig::GMOCK
        ${GLOG_LIBRARIES}
        ${GMOCK_LIBRARIES}
        ${GTEST_LIBRARIES}
    )
    add_test(NAME ${FILENAME} COMMAND ${FILENAME})
    endforeach()
endmacro()
      
BUILD(${CMAKE_CURRENT_SOURCE_DIR})
