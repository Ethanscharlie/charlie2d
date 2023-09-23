 cmake_minimum_required(VERSION 3.5)                                                                  
                                                                                                      
 # Set the project name                                                                               
 project(charlie2D)                                                                                   

 set(CMAKE_CXX_STANDARD 20)
                                                                                                      
 # Add all your .cpp files to a variable                                                              
 file(GLOB_RECURSE CHARLIE_SOURCES ${PROJECT_SOURCE_DIR}/source/*.cpp)
                                                                                                      
 # Create a static library with the given SOURCES                                                     
 add_library(charlie2D STATIC ${CHARLIE_SOURCES})                                                             
                                                                                                      
 # Include your header files                                                                          
 target_include_directories(charlie2D PUBLIC 
     /usr/include/SDL2
     ${PROJECT_SOURCE_DIR}/include
     )      

 target_link_libraries(charlie2D
     SDL2
     SDL2_image
     SDL2_ttf
     SDL2_mixer
)
