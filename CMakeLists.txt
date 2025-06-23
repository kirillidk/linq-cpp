cmake_minimum_required(VERSION 3.20)
project(linq-lib-cpp)

option(LINQ_BUILD_EXAMPLES "Build examples" OFF)

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


if (LINQ_BUILD_EXAMPLES)
  file(GLOB EXAMPLE_SRCS ${PROJECT_SOURCE_DIR}/examples/*.cpp)
  foreach(example_src ${EXAMPLE_SRCS})
    get_filename_component(example_name ${example_src} NAME_WE)
    add_executable(${example_name} ${example_src})
    target_link_libraries(${example_name} PRIVATE linq-lib-cpp)
  endforeach()
endif()