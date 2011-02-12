
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


# build ViennaUtils
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../ViennaUtils/ "${CMAKE_CURRENT_BINARY_DIR}/ViennaUtils/")

# set the gnu g++ compiler flags
IF(CMAKE_COMPILER_IS_GNUCXX)
   ADD_DEFINITIONS(-Wall -pedantic -O2 -fPIC)       
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

SET(PROGS library)

FOREACH(PROG ${PROGS})
   ADD_EXECUTABLE(${PROG} src/${PROG}.cpp)
   TARGET_LINK_LIBRARIES(${PROG} pugixml)
   ADD_test(${PROG} ${PROG})
ENDFOREACH(PROG)
