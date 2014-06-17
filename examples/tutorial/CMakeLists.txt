

ADD_EXECUTABLE(native native.cpp)
TARGET_LINK_LIBRARIES(native ${LIBRARIES})

ADD_EXECUTABLE(proxy proxy.cpp)
TARGET_LINK_LIBRARIES(proxy ${LIBRARIES})

INSTALL(TARGETS native proxy DESTINATION ${CMAKE_INSTALL_PREFIX}/examples)

IF(UDUNITS_FOUND)
ADD_EXECUTABLE(units units.cpp)
TARGET_LINK_LIBRARIES(units ${LIBRARIES})
INSTALL(TARGETS units DESTINATION ${CMAKE_INSTALL_PREFIX}/examples)
ENDIF(UDUNITS_FOUND)

IF(SWIG_FOUND)
# copy the python example to the build directory, so it is available
# as the other C++ examples in the same examples/tutorial/directory
FILE(COPY python.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

INSTALL(FILES python.py DESTINATION ${CMAKE_INSTALL_PREFIX}/examples)
ENDIF(SWIG_FOUND)
