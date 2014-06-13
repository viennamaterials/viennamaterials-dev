/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// ViennaMaterials includes
//
#include "viennamaterials/generator.hpp"
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/proxy/viennastar.hpp"





void test_native_explicit(viennamaterials::library_handle& matlib)
{
  using namespace viennamaterials;
  std::string query = "/material[id=\"Si\"]/parameter[id=\"bandgap\"]/value/text()";
  std::string value = matlib->query(query);
  std::cout << "value: " << value << std::endl;
}

void test_native_wildcard(viennamaterials::library_handle& matlib)
{
  using namespace viennamaterials;
  std::string query = "/*[id=\"Si\"]/*[id=\"bandgap\"]/value/text()";
  std::string value = matlib->query(query);
  std::cout << "value: " << value << std::endl;
}

void test_proxy(viennamaterials::library_handle& matlib)
{
  using namespace viennamaterials;

  proxy_handle myproxy(new viennastar_proxy(matlib));

  std::string result_xml = myproxy->query("Si/bandgap");
  std::cout << "result xml: " << result_xml << std::endl;

  std::string result_xml2 = myproxy->query("Si/DriftDiffusion/SRH/para_srh");
  std::cout << "result xml2: " << result_xml2 << std::endl;

  double result_value = myproxy->query_value("Si/bandgap");
  std::cout << "result value: " << result_value << std::endl;

  std::string result_unit = myproxy->query_unit("Si/bandgap");
  std::cout << "result unit: " << result_unit << std::endl;

  quantity result_quantity = myproxy->query_quantity("Si/bandgap");
  std::cout << "result quantity: " << result_quantity << std::endl;
}

int main(int argc, char * argv[])
{
  std::string filename("../../database/materials.xml");
  {
//    viennamaterials::library_handle mylib = viennamaterials::library_handle(new viennamaterials::pugixml(filename));
//    test_native_explicit(mylib);
//    test_native_wildcard(mylib);
//    test_proxy(mylib);
  }
  {
//    viennamaterials::library_handle mylib = viennamaterials::library_handle(new viennamaterials::pugixml);
//    mylib->read(filename);
//    test_native(mylib);
//    test_proxy(mylib);
  }
  {
    viennamaterials::library_handle mylib = viennamaterials::generator(filename);
    test_native_explicit(mylib);
    test_native_wildcard(mylib);
    test_proxy(mylib);
  }
  return 0;
}
