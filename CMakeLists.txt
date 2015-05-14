
SET(CMAKE_VERBOSE_MAKEFILE OFF)

PROJECT(vision)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

# SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)

INCLUDE(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
IF(GIT_SHA1 STREQUAL "GITDIR-NOTFOUND")
  MESSAGE(STATUS "This is not a build from a git clone.")
  SET(GIT_SHA1 "Not compiled from a git repository.")
  SET(GIT_DESCRIPTION
    "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
  SET(GIT_BRANCH "release")
ELSE()
  SET(GIT_BUILD 1)
  git_describe(GIT_DESCRIPTION --dirty)
  git_branch(GIT_BRANCH)
ENDIF()

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
  MESSAGE(STATUS "Enabled: OpenMP support")
ELSE()
  MESSAGE(FATAL_ERROR "Compiler does not support OpenMP. See http://openmp.org/wp/openmp-compilers/ for a list of OpenMP supporting compilers.")
ENDIF()

IF(NOT(DEFINED WITH_TCMALLOC) OR WITH_TCMALLOC)
  FIND_PACKAGE(LibGooglePerfTools)
  IF(GOOGLEPERFTOOLS_FOUND)
    MESSAGE(STATUS "Enabled: usage of TCMALLOC")
  ELSE()
    IF(WITH_TCMALLOC)
      MESSAGE(FATAL_ERROR "TCMALLOC required but not found")
    ELSE()
      MESSAGE(STATUS "Disabled: usage of TCMALLOC - TCMALLOC not found")
    ENDIF()
  ENDIF()
ELSE()
  MESSAGE(STATUS "Disabled: usage of TCMALLOC (use -DWITH_TCMALLOC=ON to enable)")
ENDIF()

SET(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

FIND_PACKAGE(Boost COMPONENTS
  program_options system filesystem date_time REQUIRED)
IF(Boost_FOUND)
  ADD_DEFINITIONS( "-DHAS_BOOST" )
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF()

ADD_SUBDIRECTORY(src)

# Add "tags" target and make my_project depending on this target.
set_source_files_properties(tags PROPERTIES GENERATED true)
add_custom_target(tags
  COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q src/
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_dependencies(tissue-recognition tags)
