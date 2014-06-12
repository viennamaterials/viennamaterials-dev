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


#include "boost/algorithm/string/split.hpp"

struct viennastar_markup : public viennamaterials::markup
{
  viennamaterials::string const& translate(viennamaterials::string const& q)
  {
    std::string result_query = "/materials/material[id=\"Si\"]";
    std::vector<std::string> query_parts;
    boost::algorithm::split(query_parts, q, boost::algorithm::is_any_of("/"));
    for(std::vector<std::string>::const_iterator iter = query_parts.begin();
        iter != query_parts.end(); iter++)
    {
    }
  }
};

void test_material_library(viennamaterials::library_handle& matlib)
{
  using namespace viennamaterials;
  string Si_bandgap_query = "/materials/material[id=\"Si\"]/parameter[name=\"bandgap\"]/value/text()";
  string value = matlib->query_native(Si_bandgap_query);
  std::cout << "value: " << value << std::endl;


  viennamaterials::markup_handle mymarkup = viennamaterials::markup_handle(new viennastar_markup);
  matlib->set_markup(mymarkup);
  string result = matlib->query("Si/bandgap");


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
//    viennamaterials::library_handle mylib = viennamaterials::generator(filename);
//    test_material_library(mylib);
  }
  return 0;
}
