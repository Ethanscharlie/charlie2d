# Install script for directory: /home/ethanscharlie/Projects/Code/Github/charlie2d

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/ethanscharlie/Projects/Code/Github/charlie2d/out/Debug/libcharlie2D.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/charlie2D" TYPE FILE FILES
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/imgui/imconfig.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/imgui/imgui_internal.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/imgui/imstb_rectpack.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/imgui/imstb_textedit.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/imgui/imstb_truetype.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/Animation.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/Button.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/CircleCollider.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/Collider.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/EntityBox.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/ExtendedComponent.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/FadeTransition.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/JumpMan.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/LDTKEntity.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/Scheduler.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/Sprite.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/Templates.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/Text.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/Tile.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/UICanvas.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/UISliceRenderer.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/imguiUtils.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/ldtk.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/components/physicsBody.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/core/Camera.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/core/Component.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/core/Entity.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/core/GameManager.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/core/InputManager.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/core/Math.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/core/ResourceManager.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/core/Vector2f.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/imgui/imgui.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/imgui/imgui_impl_sdl2.h"
    "/home/ethanscharlie/Projects/Code/Github/charlie2d/include/imgui/imgui_impl_sdlrenderer2.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/ethanscharlie/Projects/Code/Github/charlie2d/out/Debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
