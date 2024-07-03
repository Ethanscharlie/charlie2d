cmake_minimum_required(VERSION 3.5)                                                                 
project(charlie2D)       

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CHARLIE_SOURCES 
  ${PROJECT_SOURCE_DIR}/source/*.cpp
  ${PROJECT_SOURCE_DIR}/imgui/*.cpp 
  )


file(GLOB_RECURSE CHARLIE_HEADERS  
  ${PROJECT_SOURCE_DIR}/include/*.h
)

add_library(charlie2D STATIC ${CHARLIE_SOURCES} ${CHARLIE_HEADERS})                                                            

target_include_directories(charlie2D PUBLIC 
  /usr/include/SDL2
  /usr/include/nlohmann
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/core
  ${PROJECT_SOURCE_DIR}/include/components
  ${PROJECT_SOURCE_DIR}/include/imgui
  ${PROJECT_SOURCE_DIR}/source
  ${PROJECT_SOURCE_DIR}/imgui
  )      

target_link_libraries(charlie2D
  SDL2
  SDL2_ttf
  SDL2_mixer
  SDL2_image
  SDL2_gfx
  )

set_target_properties(charlie2D PROPERTIES PUBLIC_HEADER "${CHARLIE_HEADERS}")      

install(TARGETS charlie2D
  LIBRARY DESTINATION lib  
  PUBLIC_HEADER DESTINATION include/charlie2D/  )
