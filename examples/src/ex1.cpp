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
#include "viennamaterials/proxy.hpp"


#include "boost/algorithm/string/split.hpp"

namespace viennamaterials {

struct viennastar_proxy : public viennamaterials::proxy
{
  viennastar_proxy(viennamaterials::library_handle& matlib) :
    viennamaterials::proxy(matlib) {}

//  viennamaterials::string const& query(viennamaterials::string const& q)
//  {
//    std::string result_query = "/materials/material[id=\"Si\"]";
//    std::vector<std::string> query_parts;
//    boost::algorithm::split(query_parts, q, boost::algorithm::is_any_of("/"));
//    for(std::vector<std::string>::const_iterator iter = query_parts.begin();
//        iter != query_parts.end(); iter++)
//    {
//    }
//  }
};

} // viennamaterials

void test_native(viennamaterials::library_handle& matlib)
{
  using namespace viennamaterials;
  std::string Si_bandgap_query = "/materials/material[id=\"Si\"]/parameter[name=\"bandgap\"]/value/text()";
  std::string value = matlib->query_native(Si_bandgap_query);
  std::cout << "value: " << value << std::endl;
}

void test_proxy(viennamaterials::library_handle& matlib)
{
  using namespace viennamaterials;

  proxy_handle myproxy(new viennastar_proxy(matlib));


/*
  string   xmlresult = matlib->query("Si/bandgap");
  string   myunit    = matlib->query_unit("Si/bandgap");
  double   myvalue   = matlib->query_value("Si/bandgap");
  quantity myquan    = matlib->query_quantity("Si/bandgap");


*/

}

int main(int argc, char * argv[])
{
  std::string filename("../../database/materials.xml");
  {
    viennamaterials::library_handle mylib = viennamaterials::library_handle(new viennamaterials::pugixml(filename));
    test_native(mylib);
    test_proxy(mylib);
  }
  {
    viennamaterials::library_handle mylib = viennamaterials::library_handle(new viennamaterials::pugixml);
    mylib->read(filename);
    test_native(mylib);
    test_proxy(mylib);
  }
  {
//    viennamaterials::library_handle mylib = viennamaterials::generator(filename);
//    test_material_library(mylib);
  }
  return 0;
}
