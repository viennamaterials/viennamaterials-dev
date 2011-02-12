PROJECT(ViennaMaterials)

cmake_minimum_required(VERSION 2.6)

INCLUDE(CTest)
ENABLE_TESTING()

# for full cmake output, activate the following
# SET(CMAKE_VERBOSE_MAKEFILE ON)

# set the include paths
INCLUDE_DIRECTORIES(".")
INCLUDE_DIRECTORIES("external/")
INCLUDE_DIRECTORIES("../ViennaUtils/")  
INCLUDE_DIRECTORIES("../ViennaUtils/external")  

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../ViennaUtils/build)

# set the gnu g++ compiler flags
IF(CMAKE_COMPILER_IS_GNUCXX)
   ADD_DEFINITIONS(-Wall -pedantic -O2 -fPIC)       
ENDIF(CMAKE_COMPILER_IS_GNUCXX)






