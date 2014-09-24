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

  /** Create matlib object **/
  viennamaterials::library matlib(filename);

  std::string query;
  viennamaterials::attribute_handle attribute;



  /** Load floating point scalar from material library **/
  query = "/*/*[id='test-material']/*[id='float-scalar']";
  attribute = matlib.query(query);
  if(attribute->is_scalar_float())
    std::cout << "floating point scalar value:   " << attribute->evaluate_value<double>() << std::endl;




  /** Load function with float return type from material library **/
  query = "/*/*[id='test-material']/*[id='float-function-multiply']";
  attribute = matlib.query(query);
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

  return EXIT_SUCCESS;
}
