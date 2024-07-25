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
  ${PROJECT_SOURCE_DIR}/include/SDL2
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/core
  ${PROJECT_SOURCE_DIR}/include/components
  ${PROJECT_SOURCE_DIR}/include/imgui
  ${PROJECT_SOURCE_DIR}/source
  ${PROJECT_SOURCE_DIR}/imgui
  )      

if (LINUX)
  set(SDL_LIB_PATH "${PROJECT_SOURCE_DIR}/lib/linux/SDL2")
elseif (WIN32)
  set(SDL_LIB_PATH "/usr/x86_64-w64-mingw32/lib")
endif()

find_library(SDL2_      SDL2       PATHS ${SDL_LIB_PATH})
find_library(SDL2_MAIN  SDL2main   PATHS ${SDL_LIB_PATH})
find_library(SDL2_TTF   SDL2_ttf   PATHS ${SDL_LIB_PATH})
find_library(SDL2_IMAGE SDL2_image PATHS ${SDL_LIB_PATH})
find_library(SDL2_MIXER SDL2_mixer PATHS ${SDL_LIB_PATH})
find_library(SDL2_GFX SDL2_gfx PATHS ${SDL_LIB_PATH})

if (LINUX)
  target_link_libraries(charlie2D
    ${SDL2_MAIN}
    ${SDL2_}
    ${SDL2_TTF}
    ${SDL2_IMAGE}
    ${SDL2_MIXER}
    ${SDL2_GFX}
    )
elseif (WIN32)
  target_link_libraries(charlie2D
    -static-libgcc
    -static-libstdc++
    -static
    mingw32
    ${SDL2_MAIN}
    ${SDL2_}
    ${SDL2_TTF}
    ${SDL2_IMAGE}
    ${SDL2_MIXER}
    ${SDL2_GFX}
    )
elseif(EMSCRIPTEN)
  # Enable Emscripten-specific flags
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_IMAGE=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s SDL2_IMAGE_FORMATS=['png']")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_TTF=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_MIXER=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_GFX=2")

endif()

set_target_properties(charlie2D PROPERTIES PUBLIC_HEADER "${CHARLIE_HEADERS}")      

install(TARGETS charlie2D
  LIBRARY DESTINATION lib  
  PUBLIC_HEADER DESTINATION include/charlie2D/  )
