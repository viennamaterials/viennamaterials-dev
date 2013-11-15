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


void test_material_library(viennamaterials::library* mylib)
{
}


int main(int argc, char * argv[])
{
  if(argc != 2)
  {
    std::cout << "Error - usage: " << argv[0] << "material_file.xml" << std::endl;
    return -1;
  }
  std::string filename(argv[1]);
  {
    viennamaterials::library* mylib = new viennamaterials::pugixml(filename);
    test_material_library(mylib);
    delete mylib;
  }
  {
    viennamaterials::library* mylib = new viennamaterials::pugixml;
    mylib->load(filename);
    test_material_library(mylib);
    delete mylib;
  }
  {
    viennamaterials::library* mylib = viennamaterials::generator(filename);
    test_material_library(mylib);
    delete mylib;  
  }
  return 0;
}
