/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// ViennaMaterials includes
//
#include "viennamaterials/platform.hpp"

/** \example native.cpp

  With this example we show the utilization of ViennaMaterials by using
  native XPath [1] queries directly on pugixml's [2] XML backend.
  We are loading a dummy xml file, and perform a few queries.
  We show how to access the raw XML node as well as how to retrieve
  numerical values and units for a specific parameter.

  [1] XPath:   http://www.w3.org/TR/xpath/
  [2] pugixml: http://pugixml.org/
**/

int main(int argc, char * argv[])
{
  if(argc != 2)
  {
    std::cerr << "Error - Usage: " << argv[0] << " input-material-file.xml" << std::endl;
    std::cerr << "Example:       " << argv[0] << " data/material_test.xml" << std::endl;
    std::cerr << "Aborting .." << std::endl;
    return -1;
  }


  /** A string-path to a test material xml file **/
  std::string filename(argv[1]);

  /** Import the file and create a material library object **/
  viennamaterials::library_handle matlib = viennamaterials::generator(filename);

  /** Setup an arbitrary XPath query **/
  std::string query = "/material[id=\"Si\"]/parameter[id=\"bandgap\"]/value/text()";

  /** Perform the query and retrieve the string-based result **/
  std::string result_string = matlib->query(query);
  std::cout << "result string: " << result_string << std::endl;

  /** Setup the same query but use 'wildcards' this time **/
  query = "/*[id=\"Si\"]/*[id=\"bandgap\"]/value/text()";

  /** Perform the query and retrieve the string-based result **/
  result_string = matlib->query(query);
  std::cout << "result string: " << result_string << std::endl;

  /** Perform the same query, but auto convert to 'double' **/
  double value = matlib->query_value(query);
  std::cout << "value: " << value << std::endl;

  /** Setup the same query instead of accessing the value access the unit **/
  query = "/*[id=\"Si\"]/*[id=\"bandgap\"]/unit/text()";

  /** Perform the query and retrieve the string-based result **/
  result_string = matlib->query(query);
  std::cout << "unit string: " << result_string << std::endl;
  


  /** begin of hack-space **/
  std::cout << std::endl << "#################" << std::endl;

  query = "count(/material[1]/parameter)";
  double result_double = matlib->query_xpath_number(query);
  std::cout << "return number: " << result_double << std::endl;

  // query attributes of function element from parameter 1
  std::cout << "parameter1 function:" << std::endl;
  std::string element = "/*[id='test-material']/*[id='parameter1']/function";
  query = element + "/text()";
  result_string = matlib->query(query);
  std::cout << "  text: " << result_string << std::endl;
  query = "count(" + element + "/@*)"; //counts all attributes of function element
  double number_query = matlib->query_xpath_number(query);
  std::ostringstream converter;
  std::cout << "  number of function arguments: " << number_query << std::endl;
  for (long i = 1; i <= (long)number_query; i++)
  {
    converter.str("");
    converter << i;
    // query name of attribute
    query = "name(" + element + "/@*[" + converter.str() + "])";
//    std::cout << query << std::endl;
    result_string = matlib->query_xpath_string(query);
    std::cout << "    " << result_string << " --> ";
    // query value of attribute
    query = "string(" + element + "/@*[" + converter.str() + "])";
    result_string = matlib->query_xpath_string(query);
    std::cout << result_string << std::endl;
  }

  // query attributes of function element from parameter 2
  std::cout << "parameter2 function:" << std::endl;
  element = "/*[id='test-material']/*[id='parameter2']/function";
  query = element + "/text()";
  result_string = matlib->query(query);
  std::cout << "  text: " << result_string << std::endl;
  query = "count(" + element + "/@*)"; //counts all attributes of function element
  number_query = matlib->query_xpath_number(query);
  std::cout << "  number of function arguments: " << number_query << std::endl;
  for (long i = 1; i <= (long)number_query; i++)
  {
    converter.str("");
    converter << i;
    // query name of attribute
    query = "name(" + element + "/@*[" + converter.str() + "])";
//    std::cout << query << std::endl;
    result_string = matlib->query_xpath_string(query);
    std::cout << "    " << result_string << " --> ";
    // query value of attribute
    query = "string(" + element + "/@*[" + converter.str() + "])";
    result_string = matlib->query_xpath_string(query);
    std::cout << result_string << std::endl;
  }


  return 0;
}
