# Install script for directory: C:/Users/gabri/OneDrive/Documentos/Projetos/projetoCompGraf/abcg/external

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/emsdk/upstream/emscripten/cache/sysroot")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/msys64/mingw64/bin/objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/gabri/OneDrive/Documentos/Projetos/projetoCompGraf/build/abcg/external/imgui/cmake_install.cmake")
  include("C:/Users/gabri/OneDrive/Documentos/Projetos/projetoCompGraf/build/abcg/external/cppitertools/cmake_install.cmake")
  include("C:/Users/gabri/OneDrive/Documentos/Projetos/projetoCompGraf/build/abcg/external/fmt/cmake_install.cmake")
  include("C:/Users/gabri/OneDrive/Documentos/Projetos/projetoCompGraf/build/abcg/external/glm/cmake_install.cmake")
  include("C:/Users/gabri/OneDrive/Documentos/Projetos/projetoCompGraf/build/abcg/external/gsl/cmake_install.cmake")

endif()

