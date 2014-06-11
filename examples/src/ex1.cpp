/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/generator.hpp"
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/markup.hpp"

struct viennastar : public viennamaterials::markup
{
  void operator()(viennamaterials::query const& q)
  {
  }
};

void test_material_library(viennamaterials::library_handle& matlib)
{
  using namespace viennamaterials;
  string Si_bandgap_query = "/materials/material[id=\"Si\"]/parameter[name=\"bandgap\"]/value/text()";
  string value = matlib->query_native(Si_bandgap_query);
  std::cout << "value: " << value << std::endl;


  viennamaterials::markup* mymarkup = new viennastar;
//  matlib->set_markup(mymarkup);
//  string result = matlib->query("Si/bandgap");

  delete mymarkup;
}


int main(int argc, char * argv[])
{
  std::string filename("../../database/materials.xml");
  {
    viennamaterials::library_handle mylib = viennamaterials::library_handle(new viennamaterials::pugixml(filename));
    test_material_library(mylib);
  }
  {
    viennamaterials::library_handle mylib = viennamaterials::library_handle(new viennamaterials::pugixml);
    mylib->read(filename);
    test_material_library(mylib);
  }
  {
    viennamaterials::library_handle mylib = viennamaterials::generator(filename);
    test_material_library(mylib);
  }
  return 0;
}
