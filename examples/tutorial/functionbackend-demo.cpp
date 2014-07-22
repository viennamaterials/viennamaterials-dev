/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

// ViennaMaterials includes
//
#include "viennamaterials/platform.hpp"

#include "python_backend.h"
#include "viennamaterials/attributefunctionproxy.h"

int main(int argc, char * argv[])
{
  if(argc != 2)
  {
    std::cerr << "Error - Usage: " << argv[0] << " input-material-file.xml" << std::endl;
    std::cerr << "Example:       " << argv[0] << " data/material_wip.xml" << std::endl;
    std::cerr << "Aborting .." << std::endl;
    return -1;
  }


  /** A string-path to a test material xml file **/
  std::string filename(argv[1]);

  /** Import the file and create a material library object **/
  viennamaterials::library_handle matlib = viennamaterials::generator(filename);





  std::string query = "/*/*[id='test-material']/*[id='parameter2']/function";

  viennamaterials::attribute_function_proxy python_demo;

  python_demo.init(matlib, query, viennamaterials::python);

  std::vector<viennamaterials::FunctionArgumentBase> args;
  python_demo.evaluate(args);

  python_demo.deinit();






  return EXIT_SUCCESS;
}
