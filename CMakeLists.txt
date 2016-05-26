cmake_minimum_required(VERSION 3.1)
project(Atlas)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)

include(FindPkgConfig)

if(${WIN32})
    set(SDL2_PATH "${PROJECT_SOURCE_DIR}/extern/sdl2")
endif()

find_package(GLEW REQUIRED)
include_directories($(GLEW_INCLUDE_DIRS))

find_package(OpenGL REQUIRED)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories("${PROJECT_SOURCE_DIR}/extern/stb")

add_executable(main src/main.cpp src/application.cpp src/util.cpp)
target_link_libraries(main ${SDL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
