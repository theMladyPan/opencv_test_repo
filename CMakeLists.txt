cmake_minimum_required(VERSION 3.0.0)

project(openCV_test VERSION 0.3 LANGUAGES CXX)

# The version number.
set (openCV_test_VERSION_MAJOR 0)
set (openCV_test_VERSION_MINOR 3)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# find opencv sources and libs
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
set (EXTRA_LIBS ${EXTRA_LIBS} ${OpenCV_LIBS})


find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIR})
set (EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_LIBRARIES})

foreach(lib in ${EXTRA_LIBS})
    message("Including: " ${lib})
    endforeach(lib)

include_directories(src/)
set( project_sources
   main.cpp
   #src/noise_filter.cpp
   #src/segmentation.cpp
   #src/pixattr.cpp
)

add_executable(${PROJECT_NAME} ${project_sources})
target_link_libraries (${PROJECT_NAME}  ${EXTRA_LIBS})



