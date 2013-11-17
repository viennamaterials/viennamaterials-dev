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


void test_material_library(viennamaterials::library* matlib)
{
  std::cout << "--------------------------------" << std::endl;
  
  // direclty access the numerical value of a parameter of a material
  //
  double value = matlib->get_parameter_value("Si", "bandgap");
  std::cout << "value: " << value << std::endl;
  
  // same with the unit
  //
  std::string unit  = matlib->get_parameter_unit("Si", "bandgap");
  std::cout << "unit: " << unit << std::endl;

  // retrieve all ID keys of materials under a certain category
  //
  viennamaterials::keys keys= matlib->get_materials_of_category(viennamaterials::key::oxide);
  
  // now traverse the result set ...
  //
  for(viennamaterials::keys::iterator iter = keys.begin(); iter != keys.end(); iter++)
  {
    // .. check if the material has a certain parameter
    //
    if(matlib->has_parameter(*iter, "permittivity"))
    {
      // if so, print the numerical value ..
      //
      std::cout << matlib->get_parameter_value(*iter, "permittivity") << std::endl;
    }
    else 
    {
      std::cout << "Material " << *iter << " doesn't have the required parameter .." << std::endl;
    }
  }
}


int main(int argc, char * argv[])
{
  std::string filename("../../database/materials.xml");
  {
    viennamaterials::library* mylib = new viennamaterials::pugixml(filename);
    test_material_library(mylib);
    delete mylib;
  }
  {
    viennamaterials::library* mylib = new viennamaterials::pugixml;
    mylib->read(filename);
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
