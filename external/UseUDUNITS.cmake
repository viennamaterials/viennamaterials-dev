SET(UDUNITS_PATH ${CMAKE_CURRENT_LIST_DIR}/udunits-2.1.24)


include(ExternalProject)

ExternalProject_Add(
  udunits
  SOURCE_DIR ${UDUNITS_PATH}
  CONFIGURE_COMMAND ${UDUNITS_PATH}/configure --prefix=${CMAKE_BINARY_DIR}/udunits
  BINARY_DIR ${CMAKE_BINARY_DIR}/udunits/build
  #BUILD_COMMAND make -j4
)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/udunits/include)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/udunits/lib) 
SET(UDUNITSLIB udunits2)
