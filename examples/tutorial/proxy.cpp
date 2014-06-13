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
#include "viennamaterials/proxy/viennastar.hpp"


int main(int argc, char * argv[])
{
  /** A string-path to a test material xml file **/
  std::string filename("../../../examples/data/test.xml");

  /** Import the file and create a material library object **/
  viennamaterials::library_handle matlib = viennamaterials::generator(filename);

  /** Create the 'viennastar' proxy object, which although simplifies the acces
      expects the input xml file to offer a specific structure **/
  viennamaterials::proxy_handle myproxy(new viennamaterials::viennastar_proxy(matlib));

  /** Access the xml node of Silicon's bandgap parameter **/
  std::string result_xml = myproxy->query("Si/bandgap");
  std::cout << "result xml: " << result_xml << std::endl;

  /** Access some nested parameters, this can be arbitrarily 'deep' **/
  std::string result_xml2 = myproxy->query("Si/DriftDiffusion/SRH/para_srh");
  std::cout << "result xml2: " << result_xml2 << std::endl;

  /** Access the value of Silicon's bandgap parameter **/
  double result_value = myproxy->query_value("Si/bandgap");
  std::cout << "result value: " << result_value << std::endl;

  /** Access the unit string of Silicon's bandgap parameter **/
  std::string result_unit = myproxy->query_unit("Si/bandgap");
  std::cout << "result unit: " << result_unit << std::endl;

  /** Access the quantity of Silicon's bandgap parameter;
      a quantity's value and unit can be accessed via the
      value() and unit() method respectively. **/
  viennamaterials::quantity result_quantity = myproxy->query_quantity("Si/bandgap");
  std::cout << "result quantity: " << result_quantity << " -> "
            << result_quantity.value() << " (" << result_quantity.unit() << ")" << std::endl;

  return 0;
}
