cmake_minimum_required(VERSION 3.20)
project(linq-lib-cpp)

option(BUILD_TESTS "Build tests" OFF)

add_library(linq-lib-cpp INTERFACE)

target_compile_features(linq-lib-cpp
    INTERFACE
      cxx_std_20
)

target_include_directories(linq-lib-cpp
    INTERFACE
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
)

if (BUILD_TESTS)
  file(GLOB TEST_SRCS ${PROJECT_SOURCE_DIR}/test/*.cpp)
  foreach(test_src ${TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE linq-lib-cpp)
  endforeach()
endif()