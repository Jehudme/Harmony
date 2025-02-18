# Install script for directory: C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/blds/libflac/src/1.4.3-00043104ae.clean/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/pkgs/libflac_x64-windows/debug")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/blds/libflac/x64-windows-dbg/src/libFLAC/cmake_install.cmake")
  include("C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/blds/libflac/x64-windows-dbg/src/libFLAC++/cmake_install.cmake")
  include("C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/blds/libflac/x64-windows-dbg/src/share/replaygain_analysis/cmake_install.cmake")
  include("C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/blds/libflac/x64-windows-dbg/src/share/replaygain_synthesis/cmake_install.cmake")
  include("C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/blds/libflac/x64-windows-dbg/src/share/getopt/cmake_install.cmake")
  include("C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/blds/libflac/x64-windows-dbg/src/share/utf8/cmake_install.cmake")
  include("C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/blds/libflac/x64-windows-dbg/src/share/grabbag/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/jehud/source/repos/Jehudme/Harmony/vcpkg_installed/x64-windows/vcpkg/blds/libflac/x64-windows-dbg/src/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
