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

#include "viennamaterials/broker.hpp"

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


  std::string query = "/*/*[id='test-material']/*[id='parameter2']";



  /** Testing area **/

  /**
   * Xml_value_entity
   */
  viennamaterials::xml_value_entity* value_ptr = new viennamaterials::xml_value_scalar_integer;
  std::string name = "test object";
  value_ptr->set_name(name);

  std::cout << "name: " << value_ptr->get_name() << std::endl;
  std::cout << "type: " << value_ptr->get_type() << std::endl;

  long val = 12;
  value_ptr->set_value(val);

  viennamaterials::tag_scalar_int tag_int;
  std::cout << "value: " << value_ptr->get_value(tag_int) << std::endl;


  /**
   * Python function backend
   */

  /// Define python funtion
  std::cout << std::endl << "python backend demo:" << std::endl;
  std::string code = "def func(x,y): \n print x*y \n return 2.2";
  std::string func_name = "func";

  viennamaterials::function_backend *demo = new viennamaterials::function_backend_python;
  demo->init(code, func_name);

  /// Define arguments
  std::vector<viennamaterials::xml_value_entity*> args;
  viennamaterials::xml_value_entity* arg;
  arg = new viennamaterials::xml_value_scalar_float;
  arg->set_index(0);
  arg->set_value(3.0);
  args.push_back(arg);
  arg = new viennamaterials::xml_value_scalar_float;
  arg->set_index(1);
  arg->set_value(0.5);
  args.push_back(arg);

  viennamaterials::tag_scalar_float tag_float;
  double result = demo->eval(tag_float, args);
  std::cout << "result: " << result << std::endl;


  /**
   * Attribute entity
   */
  std::cout << std::endl << "attribute entity demo:" << std::endl;
  viennamaterials::attribute_entity* attr_ptr = new viennamaterials::attribute_entity_function(viennamaterials::function_int, demo, args);
  std::cout << "result: " << attr_ptr->eval(tag_int) << " <-- attribute entity used as int-function" << std::endl;

  if(attr_ptr->is_function_int()) /// using method name
    std::cout << "is_function_int" << std::endl;

  viennamaterials::tag_function_int func_tag;
  if(attr_ptr->is_function(func_tag))  /// using tag dispatching
    std::cout << "is_function" << std::endl;

  if(attr_ptr->is_type(viennamaterials::function_int))  /// using enum argument
    std::cout << "is_type" << std::endl;





  /**
   * Broker
   */
  std::cout << std::endl << "broker demo:" << std::endl;
  viennamaterials::broker broker_demo(filename);
  viennamaterials::attribute_entity *attribute = broker_demo.query(query);
  if(attribute->is_function_float())
  {
    double result = attribute->eval(tag_float);
    std::cout << "eval: " << result << std::endl;

    std::vector<viennamaterials::xml_value_entity*> func_args = attribute->get_dep();
    func_args[1]->set_value(0.125);
    attribute->set_dep(func_args);
    std::cout << "eval: " << attribute->eval(tag_float) << std::endl;
  }




  delete attr_ptr;
  delete demo;
  delete attribute;

  std::cout << std::endl << "end of program" << std::endl;
  return EXIT_SUCCESS;
}
