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
  /** A string-path to a test material xml file **/
  std::string filename("../../examples/data/test.xml");

  /** Import the file and create a material library object **/
  viennamaterials::backend_handle matlib = viennamaterials::generator(filename);

  /** Setup an arbitrary XPath query **/
  std::string query = "/material[id=\"Si\"]/parameter[id=\"bandgap\"]/value/text()";

  /** Perform the query and retrieve the string-based result **/
  std::string result_string = matlib->query(query);
  assert(result_string == "1.107");

  /** Setup the same query but use 'wildcards' this time **/
  query = "/*[id=\"Si\"]/*[id=\"bandgap\"]/value/text()";

  /** Perform the query and retrieve the string-based result **/
  result_string = matlib->query(query);
  assert(result_string == "1.107");

  /** Perform the same query, but auto convert to 'double' **/
  double value = matlib->query_value(query);
  assert(value == 1.107);

  /** Setup the same query instead of accessing the value access the unit **/
  query = "/*[id=\"Si\"]/*[id=\"bandgap\"]/unit/text()";

  /** Perform the query and retrieve the string-based result **/
  result_string = matlib->query(query);
  assert(result_string == "eV");

  return 0;
}
