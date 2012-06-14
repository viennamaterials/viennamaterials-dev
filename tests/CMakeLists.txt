
PROJECT(ViennaMaterialsTest)

cmake_minimum_required(VERSION 2.6)

# configure dart/cdash support
INCLUDE(CTest)
ENABLE_TESTING()
IF(BUILD_TESTING)
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

INCLUDE_DIRECTORIES("../")
INCLUDE_DIRECTORIES("../external/")
INCLUDE_DIRECTORIES("../../ViennaUtils/")  
INCLUDE_DIRECTORIES("../../ViennaUtils/external")  

# set modules directory
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Modules/")

SET(VIENNAUTILS_INC_DIR "../../ViennaUtils/")

find_package(VIENNAUTILS REQUIRED)
include_directories(${VIENNAUTILS_INCLUDE_DIRS})

# set the gnu g++ compiler flags
IF(CMAKE_COMPILER_IS_GNUCXX)
   ADD_DEFINITIONS(-Wall -pedantic -O2 -fPIC)       
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

SET(PROGS library)

FOREACH(PROG ${PROGS})
   ADD_EXECUTABLE(${PROG} src/${PROG}.cpp)
   ADD_test(${PROG} ${PROG})
ENDFOREACH(PROG)
