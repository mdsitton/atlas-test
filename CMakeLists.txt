cmake_minimum_required(VERSION 3.1)
project(Atlas)

add_definitions ("-Wall")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)

include(FindPkgConfig)

if(${WIN32})
    set(SDL2_PATH "${PROJECT_SOURCE_DIR}/extern/sdl2")
endif()

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/extern/stb")

add_executable(main
src/main.cpp
src/application.cpp
src/util.cpp
src/renderer.cpp
src/texture.cpp
src/GL/gl3w.c)

set(BINARY_DATA_DIR ${CMAKE_BINARY_DIR}/data)
set(SOURCE_DATA_DIR ${CMAKE_SOURCE_DIR}/data)


add_custom_command(TARGET main PRE_BUILD
                  COMMAND ${CMAKE_COMMAND} -E make_directory
                  ${BINARY_DATA_DIR})
add_custom_command(TARGET main PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${SOURCE_DATA_DIR} $<TARGET_FILE_DIR:main>/data/)


target_link_libraries(main ${CMAKE_DL_LIBS} ${SDL2_LIBRARY} ${OPENGL_LIBRARIES} )#${GLEW_LIBRARIES})
