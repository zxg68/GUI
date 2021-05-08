cmake_minimum_required(VERSION 3.0)
project(GUI_v3)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -ggdb -std=c++17")

include_directories(.)
include_directories(/home/softbasic/zoe/nana/include)

find_library(NANA nana /home/softbasic/zoe/nana/build)

set(LIB_SRC
    src/Drawing.cpp
    src/Graph.cpp
    src/GUI.cpp
    src/Window.cpp
)

set(LIBS
    GUI_v3
    ${NANA}
)

add_library(GUI_v3 ${LIB_SRC})
# add_library(zcserver_static STATIC ${LIB_SRC})
# SET_TARGET_PROPERTIES (zcserver_static PROPERTIES OUTPUT_NAME "zcserver")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
