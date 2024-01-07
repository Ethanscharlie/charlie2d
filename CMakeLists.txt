cmake_minimum_required(VERSION 3.5)                                                                 
project(charlie2D)       

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CHARLIE_SOURCES 
   ${PROJECT_SOURCE_DIR}/source/*.cpp
   ${PROJECT_SOURCE_DIR}/imgui/*.cpp 
  )
add_library(charlie2D STATIC ${CHARLIE_SOURCES})                                                            
target_include_directories(charlie2D PUBLIC 
    /usr/include/SDL2
    /usr/include/nlohmann
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/imgui
    )      
target_link_libraries(charlie2D
    SDL2
    SDL2_ttf
    SDL2_mixer
    SDL2_image
    )
