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
#include "viennamaterials/library.hpp"

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

  /** Create broker object **/
  viennamaterials::library broker(filename);

  std::string query;
  viennamaterials::attribute_handle attribute;

  std::cout << "########################  Scalar attributes  ########################" << std::endl;

  /** Load boolean scalar from material library **/
  query = "/*/*[id='test-material']/*[id='bool-scalar']";
  attribute = broker.query(query);
  if(attribute->is_scalar_bool())
    std::cout << "boolean scalar value:          " << attribute->evaluate_value<bool>() << " " << attribute->get_unit() << std::endl;

  /** Load integer scalar from material library **/
  query = "/*/*[id='test-material']/*[id='int-scalar']";
  attribute = broker.query(query);
  if(attribute->is_scalar_int())
    std::cout << "integer scalar value:          " << attribute->evaluate<long>() << std::endl;

  /** Load floating point scalar from material library **/
  query = "/*/*[id='test-material']/*[id='float-scalar']";
  attribute = broker.query(query);
  if(attribute->is_scalar_float())
    std::cout << "floating point scalar value:   " << attribute->evaluate<double>() << std::endl;


  std::cout << "######################## Function attributes ########################" << std::endl;

  /** Load function with float return type from material library **/
  query = "/*/*[id='test-material']/*[id='float-function-multiply']";
  attribute = broker.query(query);
  if(attribute->is_function_float())
  {
    /// Using default values of function arguments
    std::cout << "multiply function (defaults):  " << attribute->evaluate_value<double>() << " " << attribute->get_unit() << std::endl;

    /// Set value of second function argument and evaluate
    std::vector<viennamaterials::xml_value_entity_handle> func_args = attribute->get_dependencies();
    func_args[1]->set_value(0.125);
    attribute->set_dependencies(func_args);
    std::cout << "multiply function (arg set):   " << attribute->evaluate_value<double>() << std::endl;
  }

  /** Load function with integer return type from material library **/
  query = "/*/*[id='test-material']/*[id='int-function-add']";
  viennamaterials::attribute_handle attribute_2 = broker.query(query);
  if(attribute_2->is_function_int())
  {
    /// Using default values of function arguments
    std::cout << "add function (defaults):  " << attribute_2->evaluate_value<long>() << std::endl;

    /// Set value of first function argument and evaluate
    std::vector<viennamaterials::xml_value_entity_handle> func_args = attribute_2->get_dependencies();
    long my_value = 8;
    func_args[0]->set_value(my_value);
    attribute_2->set_dependencies(func_args);
    std::cout << "add function (arg set):   " << attribute_2->evaluate_value<long>() << std::endl;
  }

  /** Load function with boolean return type from material library **/
  query = "/*/*[id='test-material']/*[id='bool-function-without-args']";
  viennamaterials::attribute_handle attribute_3 = broker.query(query);
  if(attribute_3->is_function_bool())
  {
    /// Using default values of function arguments
    std::cout << "bool function without args:  " << attribute_3->evaluate_value<bool>() << std::endl;
  }

  std::cout << "#####" << std::endl;

  /** Load function with references from material library **/
  query = "/*/*[id='test-material']/*[id='float-function-references']";
  viennamaterials::attribute_handle attribute_4 = broker.query(query);
  if(attribute_4->is_function_float())
  {
    /// Using default values of function arguments
    std::cout << "function with references (defaults):  " << attribute_4->evaluate_value<double>() << std::endl;

    std::vector<viennamaterials::xml_value_entity_handle> func_args = attribute_4->get_dependencies();
    std::cout << "number of dependencies: " << func_args.size() << std::endl;
  }


  //TODO: correct and check asserts in tests!!
  //TODO: xmlvalueentity --> setters as protected and use friend


  std::cout << "########################     End of demo     ########################" << std::endl;
  return EXIT_SUCCESS;
}
