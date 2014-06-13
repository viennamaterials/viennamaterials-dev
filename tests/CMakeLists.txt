
set(TEST_PROGRAMS ${TEST_PROGRAMS} strings)


# setup tests for all previously defined tests
#
foreach(PROG ${TEST_PROGRAMS})
  add_executable(${PROG}-test src/${PROG}.cpp)
  target_link_libraries(${PROG}-test ${VIENNAMATERIALS_LIBRARY})
  add_test(${PROG} ${PROG}-test)
endforeach(PROG)
