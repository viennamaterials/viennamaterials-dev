/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/library_dyn.hpp"
#include "viennamaterials/kernels/pugixml_dyn.hpp"

#include "boost/shared_ptr.hpp"

template<typename MatLibPtr>
void process(MatLibPtr matlib, std::string const& filename)
{
  typedef typename vmat::LibraryDyn::numeric_type   NumericT;
  typedef typename vmat::LibraryDyn::keys_type      KeysT;

  // process an input file 
  //
  matlib->load(filename);
  
  // print the database to a stream (std::cout by default ..)
  //
  //matlib->dump(); // too much output, so it's deactivated ..
  
  // direclty access the numerical value of a parameter of a material
  //
  NumericT    value = matlib->getParameterValue("Si", "bandgap");
  
  // same with the unit
  //
  std::string unit  = matlib->getParameterUnit("Si", "bandgap");
  
  std::cout << "bandgap of Si: " << value << " " << unit << std::endl;
  
  // retrieve all ID keys of materials under a certain category
  //
  KeysT keys = matlib->getMaterialsOfCategory(vmat::key::oxide);
  
  // now traverse the result set ...
  //
  for(typename KeysT::iterator iter = keys.begin(); iter != keys.end(); iter++)
  {
    // .. check if the material has a certain parameter
    //
    if(matlib->hasParameter(*iter, "bandgap"))
    {
      // if so, print the numerical value ..
      //
      std::cout << matlib->getParameterValue(*iter, "bandgap") << std::endl;
    }
    else 
    {
      std::cout << "Material " << *iter << " doesn't have the required parameter .." << std::endl;
    }
  }
}


int main(int argc, char * argv[])
{
  if(argc != 2)
  {
    std::cout << "Error - usage: " << argv[0] << "material_file.xml" << std::endl;
    return -1;
  }

  // use plain pointers
  //
  std::cout << "-- Testing plain pointer utilization" << std::endl;
  vmat::LibraryDyn* matlib = new vmat::PugiXMLDyn;
  process(matlib, argv[1]);
  delete matlib;
  std::cout << std::endl;

  // or use shared pointers
  //
  std::cout << "-- Testing shared pointer utilization" << std::endl;  
  typedef boost::shared_ptr<vmat::LibraryDyn>   MatLibPtr;
  MatLibPtr matlib2 = MatLibPtr(new vmat::PugiXMLDyn);
  process(matlib2, argv[1]);

}

