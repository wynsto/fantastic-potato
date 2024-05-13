#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Potato::Potato" for configuration ""
set_property(TARGET Potato::Potato APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Potato::Potato PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/Potato-1.0/libPotato.a"
  )

list(APPEND _cmake_import_check_targets Potato::Potato )
list(APPEND _cmake_import_check_files_for_Potato::Potato "${_IMPORT_PREFIX}/lib/Potato-1.0/libPotato.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
