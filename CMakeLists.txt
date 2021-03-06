PROJECT(ViennaMaterials)

cmake_minimum_required(VERSION 2.6)

INCLUDE_DIRECTORIES(".")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

OPTION(BUILD_EXAMPLES "Build the examples" ON)
OPTION(BUILD_TESTS    "Build the tests"    ON)
OPTION(BUILD_PYLIB    "Build the Python library"    OFF)
OPTION(BUILD_UNITS    "Build with units support (requireds UDUNITS)"    ON)
#option(BUILD_DTD_CHECK_SUPPORT "Perform DTD checks of the input XML material data - requires libxml2" OFF)

#set default build type
IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

#IF (NOT BUILD_SHARED_LIBS)
#  SET(BUILD_SHARED_LIBS ON)
#ENDIF()

#set compiler flags
SET(CMAKE_CXX_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

IF(BUILD_UNITS)
  FIND_PACKAGE(UDUNITS)
  IF(UDUNITS_FOUND)
    INCLUDE_DIRECTORIES(${UDUNITS_INCLUDE_DIR})
    SET(LIBRARIES ${LIBRARIES} ${UDUNITS_LIBRARIES})
    ADD_DEFINITIONS(-DVIENNAMATERIALS_WITH_UNITS)
    MESSAGE(STATUS "Required udunits libraries detected: ViennaMaterials will be built with unit support!")
  ELSE(UDUNITS_FOUND)
    MESSAGE(WARNING "Required udunits libraries missing: ViennaMaterials will be built without unit support!")
  ENDIF(UDUNITS_FOUND)
ELSE(BUILD_UNITS)
  MESSAGE(WARNING "Units support deactivated: ViennaMaterials will be built without unit support!")
ENDIF(BUILD_UNITS)

IF(BUILD_PYLIB)
  FIND_PACKAGE(SWIG)
  IF(SWIG_FOUND)
    INCLUDE(${SWIG_USE_FILE})
    FIND_PACKAGE(PythonLibs)
    IF(PYTHONLIBS_FOUND)
      INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
      LINK_DIRECTORIES(${PYTHON_LIBRARY_DIR})
      MESSAGE(STATUS "Required Python libraries and SWIG detected: ViennaMaterials will be built with Python support!")
    ELSE(PYTHONLIBS_FOUND)
      MESSAGE(WARNING "Required Python libraries missing: ViennaMaterials will be built without Python support!")
    ENDIF(PYTHONLIBS_FOUND)
  ELSE(SWIG_FOUND)
    MESSAGE(WARNING "SWIG missing: ViennaMaterials will be built without Python support!")
  ENDIF(SWIG_FOUND)
ELSE(BUILD_PYLIB)
  MESSAGE(WARNING "Python deactivated: ViennaMaterials will be built without Python support!")
ENDIF(BUILD_PYLIB)

INCLUDE_DIRECTORIES(.)
#ADD_SUBDIRECTORY(external)

#IF(BUILD_DTD_CHECK_SUPPORT)
#  FIND_PACKAGE(LibXml2)
#  IF(LIBXML2_FOUND)
#    INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})
#    SET(LIBRARIES ${LIBRARIES} ${LIBXML2_LIBRARIES})
#    ADD_DEFINITIONS(-DVIENNAMATERIALS_HAS_LIBXML2)
#  ENDIF(LIBXML2_FOUND)
#ENDIF(BUILD_DTD_CHECK_SUPPORT)


# http://www.cmake.org/Wiki/CMake_RPATH_handling
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

# build the ViennaMaterials library
SET(VIENNAMATERIALS_LIBRARY viennamaterials)
FILE(GLOB_RECURSE LIBSOURCES src/*.cpp)
ADD_LIBRARY(${VIENNAMATERIALS_LIBRARY} ${LIBSOURCES})
TARGET_LINK_LIBRARIES(${VIENNAMATERIALS_LIBRARY} ${LIBRARIES})
IF(NOT BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(${VIENNAMATERIALS_LIBRARY} PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF(NOT BUILD_SHARED_LIBS)


SET(LIBRARIES ${LIBRARIES} ${VIENNAMATERIALS_LIBRARY})
INSTALL(TARGETS ${VIENNAMATERIALS_LIBRARY} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY viennamaterials/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/viennamaterials)
INSTALL(DIRECTORY units/ DESTINATION ${CMAKE_INSTALL_PREFIX}/units)

IF(BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(examples)
ENDIF(BUILD_EXAMPLES)

IF(BUILD_TESTS)
  INCLUDE(CTest)
  ADD_SUBDIRECTORY(tests)
ENDIF()

IF(SWIG_FOUND)
  ADD_SUBDIRECTORY(python)
ENDIF(SWIG_FOUND)
