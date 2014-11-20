
OPTION(IPD2XML_VERBOSE         "ipd2xml: verbose mode"                         ON)
OPTION(IPD2XML_VERIFY          "ipd2xml: verify XML"                           ON)
# uses functionality of viennamaterials to import the data from XML and verify against IPD data


ADD_SUBDIRECTORY(ViennaIPD)
ADD_SUBDIRECTORY(tinyxml)

ADD_EXECUTABLE(ipd2xml ipd2xml.cpp)
TARGET_LINK_LIBRARIES(ipd2xml viennaipd tinyxml)

IF(IPD2XML_VERBOSE)
ADD_DEFINITIONS(-DVERBOSE_MODE)
ENDIF(IPD2XML_VERBOSE)

IF(IPD2XML_VERIFY)
ADD_DEFINITIONS(-DVERIFY_XML)
TARGET_LINK_LIBRARIES(ipd2xml ${LIBRARIES})
ENDIF(IPD2XML_VERIFY)
