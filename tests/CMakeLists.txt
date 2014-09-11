
set(TEST_PROGRAMS ${TEST_PROGRAMS} strings)
set(TEST_PROGRAMS ${TEST_PROGRAMS} native)
set(TEST_PROGRAMS ${TEST_PROGRAMS} native_proxy)

IF(UDUNITS_FOUND)
set(TEST_PROGRAMS ${TEST_PROGRAMS} units)
ENDIF(UDUNITS_FOUND)

# setup tests for all previously defined tests
#
foreach(PROG ${TEST_PROGRAMS})
  add_executable(${PROG}-test src/${PROG}.cpp)
  target_link_libraries(${PROG}-test ${VIENNAMATERIALS_LIBRARY})
  add_test(${PROG} ${PROG}-test)
endforeach(PROG)
