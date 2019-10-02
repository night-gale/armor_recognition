cmake_minimum_required(VERSION 3.0.0)
project(blending VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)

aux_source_directory(./src DIR_SRCS)
include_directories(./inc)
add_executable(blending ${DIR_SRCS})

target_link_libraries(blending ${OpenCV_LIBS})

set(CMAKE_BUILD_TYPE "Debug")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
