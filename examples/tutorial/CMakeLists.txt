

ADD_EXECUTABLE(native native.cpp)
TARGET_LINK_LIBRARIES(native ${LIBRARIES})

ADD_EXECUTABLE(bench bench.cpp)
TARGET_LINK_LIBRARIES(bench ${LIBRARIES})

ADD_EXECUTABLE(native_proxy native_proxy.cpp)
TARGET_LINK_LIBRARIES(native_proxy ${LIBRARIES})

ADD_EXECUTABLE(material_library material_library.cpp)
TARGET_LINK_LIBRARIES(material_library ${LIBRARIES})

INSTALL(TARGETS native native_proxy material_library DESTINATION ${CMAKE_INSTALL_PREFIX}/examples)

IF(UDUNITS_FOUND)
ADD_EXECUTABLE(units units.cpp)
TARGET_LINK_LIBRARIES(units ${LIBRARIES})
INSTALL(TARGETS units DESTINATION ${CMAKE_INSTALL_PREFIX}/examples)
ENDIF(UDUNITS_FOUND)

IF(SWIG_FOUND)
# copy the python example to the build directory, so it is available
# as the other C++ examples in the same examples/tutorial/directory
FILE(COPY material_library.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

INSTALL(FILES material_library.py DESTINATION ${CMAKE_INSTALL_PREFIX}/examples)
ENDIF(SWIG_FOUND)
