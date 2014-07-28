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

//#include "viennamaterials/attributefunctionproxy.h"

//WIP:
#include "viennamaterials/xmlvalueentity.hpp"
#include "viennamaterials/xmlvaluescalar.hpp"

#include "viennamaterials/functionbackendpython.hpp"

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

//  viennamaterials::attribute_function_proxy python_demo;
//
//  python_demo.init(matlib, query, viennamaterials::python);
//
//  std::vector<viennamaterials::FunctionArgumentBase> args;
//  python_demo.evaluate(args);
//
//  python_demo.deinit();



  //testing area

//  viennamaterials::xml_value_scalar_integer testobj;
  viennamaterials::xml_value_entity* testobj = new viennamaterials::xml_value_scalar_integer;
  std::string name = "test object";
  testobj->set_name(name);
  testobj->set_type(viennamaterials::scalar_int);

  std::cout << "name: " << testobj->get_name() << std::endl;
  std::cout << "type: " << testobj->get_type() << std::endl;

  int* ptr = new int(12);
  testobj->set_value(ptr);

  std::cout << "value: " << *(int*)testobj->get_value_ptr() << std::endl;


  std::string code = "def func(x,y): \n print x*y \n return 2.0";
  std::string func_name = "func";
  viennamaterials::function_backend_python *demo = new viennamaterials::function_backend_python;
  demo->init(code, func_name);
  demo->evaluate();
  delete demo;



  std::cout << "end of program" << std::endl;
  return EXIT_SUCCESS;
}
