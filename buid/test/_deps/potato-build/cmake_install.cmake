# Install script for directory: /Users/wynston/Development/fantastic-potato

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Library/Developer/CommandLineTools/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/wynston/Development/fantastic-potato/buid/test/_deps/fmt-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/wynston/Development/fantastic-potato/buid/test/_deps/nlohmann_json-build/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Potato_Development" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Potato-1.0" TYPE DIRECTORY FILES "/Users/wynston/Development/fantastic-potato/buid/test/_deps/potato-build/PackageProjectInclude/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Potato_Development" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Potato-1.0" TYPE STATIC_LIBRARY FILES "/Users/wynston/Development/fantastic-potato/buid/test/_deps/potato-build/libPotato.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Potato-1.0/libPotato.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Potato-1.0/libPotato.a")
    execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Potato-1.0/libPotato.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Potato_Development" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Potato-1.0/PotatoTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Potato-1.0/PotatoTargets.cmake"
         "/Users/wynston/Development/fantastic-potato/buid/test/_deps/potato-build/CMakeFiles/Export/4f9b7248b8ed4cc3bbb51ea03b0d8c71/PotatoTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Potato-1.0/PotatoTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Potato-1.0/PotatoTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Potato-1.0" TYPE FILE FILES "/Users/wynston/Development/fantastic-potato/buid/test/_deps/potato-build/CMakeFiles/Export/4f9b7248b8ed4cc3bbb51ea03b0d8c71/PotatoTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Potato-1.0" TYPE FILE FILES "/Users/wynston/Development/fantastic-potato/buid/test/_deps/potato-build/CMakeFiles/Export/4f9b7248b8ed4cc3bbb51ea03b0d8c71/PotatoTargets-noconfig.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Potato_Development" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Potato-1.0" TYPE FILE FILES
    "/Users/wynston/Development/fantastic-potato/buid/test/_deps/potato-build/PotatoConfigVersion.cmake"
    "/Users/wynston/Development/fantastic-potato/buid/test/_deps/potato-build/PotatoConfig.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Potato_Development" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Potato-1.0" TYPE DIRECTORY FILES "/Users/wynston/Development/fantastic-potato/include/")
endif()

