cmake_minimum_required(VERSION 3.0.0)

project(openCV_test VERSION 0.1 LANGUAGES CXX)

# The version number.
set (openCV_test_VERSION_MAJOR 0)
set (openCV_test_VERSION_MINOR 1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


# find opencv sources and libs
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

find_library(MY_LIBS
    libmy-libs.a
    HINTS /usr/lib/
    )

set (EXTRA_LIBS
    ${OpenCV_LIBS}
    ${MY_LIBS}
    )
message(${EXTRA_LIBS})

include_directories(src/)
set( project_sources
   main.cpp
)
add_executable(${PROJECT_NAME} ${project_sources})
target_link_libraries (${PROJECT_NAME}  ${EXTRA_LIBS})



