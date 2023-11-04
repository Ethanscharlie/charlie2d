 cmake_minimum_required(VERSION 3.5)                                                                  
                                                                                                      
 # Set the project name                                                                               
 project(charlie2D)                                                                                   

 set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

 set(CMAKE_CXX_STANDARD 20)
                                                                                                      
 # Add all your .cpp files to a variable                                                              
 file(GLOB_RECURSE CHARLIE_SOURCES ${PROJECT_SOURCE_DIR}/source/*.cpp)
                                                                                                      
 # Create a static library with the given SOURCES                                                     
 add_library(charlie2D STATIC ${CHARLIE_SOURCES})                                                             
                                                                                                      
 # Include your header files                                                                          
 target_include_directories(charlie2D PUBLIC 
     /usr/include/SDL2
     /usr/include/nlohmann
     ${PROJECT_SOURCE_DIR}/include
     )      

 target_link_libraries(charlie2D
     SDL2
     SDL2_ttf
     SDL2_mixer
)

if (EMSCRIPTEN)
  # Enable Emscripten-specific flags
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_IMAGE=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s SDL2_IMAGE_FORMATS=['png']")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_TTF=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_MIXER=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --use-preload-plugins")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --shell-file ../minimal.html")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/img@/img")
else()
    target_link_libraries(charlie2D
        SDL2_image
)
endif()


