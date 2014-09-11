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
#include "viennamaterials/forwards.h"
#include "viennamaterials/generator.hpp"
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/proxy/viennastar.hpp"


int main(int argc, char * argv[])
{
  /** A string-path to a test material xml file **/
  std::string filename("../../examples/data/material_test.xml");

  /** Import the file and create a material library object **/
  viennamaterials::backend_handle matlib = viennamaterials::generator(filename);

  /** Create the 'viennastar' proxy object, which although simplifies the acces
      expects the input xml file to offer a specific structure **/
  viennamaterials::proxy_handle myproxy(new viennamaterials::viennastar_proxy(matlib));

  /** Access the xml node of Silicon's bandgap parameter **/
  std::string result_xml = myproxy->query("Si/bandgap");
  assert(result_xml ==
"<attribute>\n\
  <id>bandgap</id>\n\
  <scalar type=\"float\">1.107</scalar>\n\
  <unit>eV</unit>\n\
</attribute>");

  /** Access some nested parameters, this can be arbitrarily 'deep' **/
  std::string result_xml2 = myproxy->query("Si/DriftDiffusion/SRH/para_srh");
  assert(result_xml2 ==
"<attribute>\n\
  <id>para_srh</id>\n\
  <scalar type=\"float\">4.0</scalar>\n\
  <unit>cm</unit>\n\
</attribute>");

  /** Access the value of Silicon's bandgap parameter **/
  double result_value = myproxy->query_value<double>("Si/bandgap");
  assert(result_value == 1.107);

  /** Access the unit string of Silicon's bandgap parameter **/
  std::string result_unit = myproxy->query_unit("Si/bandgap");
  assert(result_unit == "eV");

  /** Access the quantity of Silicon's bandgap parameter;
      a quantity's value and unit can be accessed via the
      value() and unit() method respectively. **/
  viennamaterials::quantity<double> result_quantity = myproxy->query_quantity<double>("Si/bandgap");
  assert(result_quantity.value() == 1.107);
  assert(result_quantity.unit() == "eV");

  return 0;
}
