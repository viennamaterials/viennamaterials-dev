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

//WIP:
#include "viennamaterials/xmlvalueentity.hpp"
#include "viennamaterials/xmlvaluescalar.hpp"

#include "viennamaterials/functionbackendpython.hpp"

#include "viennamaterials/attributeentityfunction.hpp"

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



  //testing area

  viennamaterials::xml_value_entity* value_ptr = new viennamaterials::xml_value_scalar_integer;
  std::string name = "test object";
  value_ptr->set_name(name);
  value_ptr->set_type(viennamaterials::type_int);

  std::cout << "name: " << value_ptr->get_name() << std::endl;
  std::cout << "type: " << value_ptr->get_type() << std::endl;

  long val = 12;
  value_ptr->set_value(val);

  viennamaterials::tag_scalar_int tag;
  std::cout << "value: " << value_ptr->get_value(tag) << std::endl;


  std::cout << "python backend demo:" << std::endl;
  std::string code = "def func(x,y): \n print x*y \n return 2.0";
  std::string func_name = "func";
  viennamaterials::function_backend *demo = new viennamaterials::function_backend_python;
  demo->init(code, func_name);
  demo->eval(tag);


  std::cout << "attribute entity demo:" << std::endl;
  viennamaterials::attribute_entity* attr_ptr = new viennamaterials::attribute_entity_function(viennamaterials::function_int, demo);
  std::cout << "result: " << attr_ptr->eval(tag) << std::endl;

  if(attr_ptr->is_function_int()) /// using method name
    std::cout << "is_function_int" << std::endl;

  viennamaterials::tag_function_int func_tag;
  if(attr_ptr->is_function(func_tag))  /// using tag dispatching
    std::cout << "is_function" << std::endl;

  if(attr_ptr->is_type(viennamaterials::function_int))  /// using enum argument
    std::cout << "is_type" << std::endl;


  delete demo;
  std::cout << "end of program" << std::endl;
  return EXIT_SUCCESS;
}
