PROJECT(ViennaMaterials)

cmake_minimum_required(VERSION 2.6)

INCLUDE_DIRECTORIES(".")

#set default build type 
IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

#set compiler flags
SET(CMAKE_CXX_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

INCLUDE(external/ViennaUtils/UseViennaUtils.cmake)
INCLUDE(external/ViennaIPD/UseViennaIPD.cmake)
INCLUDE(external/UseUDUNITS.cmake)

SET(LIBRARIES ${VIENNAIPDLIB} ${UDUNITSLIB})

message(${LIBRARIES})
ADD_SUBDIRECTORY(examples)
