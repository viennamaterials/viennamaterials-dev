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

  /** Create broker object **/
  viennamaterials::broker broker(filename);

  std::string query;
  viennamaterials::attribute_handle attribute;

  std::cout << "########################  Scalar attributes  ########################" << std::endl;

  /** Load boolean scalar from material library **/
  query = "/*/*[id='test-material']/*[id='bool-scalar']";
  attribute = broker.query(query);
  if(attribute->is_scalar_bool())
    std::cout << "boolean scalar value:          " << attribute->evaluate<bool>() << std::endl;

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
    std::cout << "multiply function (defaults):  " << attribute->evaluate<double>() << std::endl;

    /// Set value for second function argument and evaluate
    std::vector<viennamaterials::xml_value_entity_handle> func_args = attribute->get_dependencies();
    func_args[1]->set_value(0.125);
    attribute->set_dependencies(func_args);
    std::cout << "multiply function (arg set):   " << attribute->evaluate<double>() << std::endl;
  }
  attribute.reset(); //FIXME

  /** Load function with integer return type from material library **/
  query = "/*/*[id='test-material']/*[id='int-function-add']";
  attribute = broker.query(query);
  if(attribute->is_function_int())
  {
    /// Using default values of function arguments
    std::cout << "add function (defaults):  " << attribute->evaluate<long>() << std::endl;

    /// Set value for second function argument and evaluate
    std::vector<viennamaterials::xml_value_entity_handle> func_args = attribute->get_dependencies();
    func_args[0]->set_value((long)8);
    attribute->set_dependencies(func_args);
    std::cout << "add function (arg set):   " << attribute->evaluate<long>() << std::endl;
  }
  attribute.reset(); //FIXME

  std::cout << "#####" << std::endl;

  /** Load function with references from material library **/
  query = "/*/*[id='test-material']/*[id='float-function-references']";
  attribute = broker.query(query);
  if(attribute->is_function_float())
  {
    /// Using default values of function arguments
//    std::cout << "function with references (defaults):  " << attribute->evaluate<double>() << std::endl;
    //FIXME: big troubles: multiple python instances do not work
  }


  //TODO: limit recursion of function argument (circular dependency)



  std::cout << "########################     End of demo     ########################" << std::endl;
  return EXIT_SUCCESS;
}
