# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.29
cmake_policy(SET CMP0009 NEW)

# sources at CMakeLists.txt:29 (file)
file(GLOB NEW_GLOB LIST_DIRECTORIES true "/Users/wynston/Development/fantastic-potato/test/source/*.cpp")
set(OLD_GLOB
  "/Users/wynston/Development/fantastic-potato/test/source/greeter.cpp"
  "/Users/wynston/Development/fantastic-potato/test/source/main.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/Users/wynston/Development/fantastic-potato/buid/test/CMakeFiles/cmake.verify_globs")
endif()

# headers at /Users/wynston/Development/fantastic-potato/CMakeLists.txt:56 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/Users/wynston/Development/fantastic-potato/include/*.h")
set(OLD_GLOB
  "/Users/wynston/Development/fantastic-potato/include/potato/greeter.h"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/Users/wynston/Development/fantastic-potato/buid/test/CMakeFiles/cmake.verify_globs")
endif()

# sources at /Users/wynston/Development/fantastic-potato/CMakeLists.txt:57 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/Users/wynston/Development/fantastic-potato/source/*.cpp")
set(OLD_GLOB
  "/Users/wynston/Development/fantastic-potato/source/greeter.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/Users/wynston/Development/fantastic-potato/buid/test/CMakeFiles/cmake.verify_globs")
endif()
