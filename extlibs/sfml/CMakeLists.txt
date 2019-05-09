include(FetchContent)

FetchContent_Declare(
    sfml
    URL https://www.sfml-dev.org/files/SFML-2.5.1-sources.zip
)

if(NOT sfml_POPULATED)
  FetchContent_Populate(sfml)
  set(BUILD_SHARED_LIBS FALSE)
  set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
  set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
  add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

