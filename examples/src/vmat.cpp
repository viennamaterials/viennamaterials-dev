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

/** 
  @brief A material accessor for specific XML markup. ViennaMaterials will replace 
         the placeholders, i.e., '%', with concrete keys as specified with the
         'make_entry()' method
*/
struct material_accessor : public viennamaterials::base_accessor
{
  result_type operator()()
  {
    return "/materials/material[id=\"%\"]";
  }
};

/** 
  @brief A parameter accessor for specific XML markup. ViennaMaterials will replace 
         the placeholders, i.e., '%', with concrete keys as specified with the
         'make_entry()' method
*/
struct parameter_accessor : public viennamaterials::base_accessor
{
  result_type operator()()
  {
    return "/parameter[name=\"%\"]";
  }
};

/** 
  @brief A data accessor for specific XML markup. ViennaMaterials will replace 
         the placeholders, i.e., '%', with concrete keys as specified with the
         'make_entry()' method
*/
struct data_accessor : public viennamaterials::base_accessor
{
  result_type operator()()
  {
    return "/%/text()";
  }
};

void test_material_library(viennamaterials::library* matlib)
{
  using namespace viennamaterials;
  std::cout << "--------------------------------" << std::endl;

  /** @brief Registering the accessors returns a unique handle .. */
  accessor_handle material  = matlib->register_accessor(new material_accessor);
  accessor_handle parameter = matlib->register_accessor(new parameter_accessor);
  accessor_handle data      = matlib->register_accessor(new data_accessor);

  /** @brief .. which is used to link a section with a concrete key. 
             This allows to setup a backend-independent query system.
             ViennaMaterials will add each entry according to the sequence of
             entries generated with 'make_entry' and automatically replaces the 
             placeholders contained in each string of the linked accessor with 
             the concrete key, for instance, in this case the pugixml backend is used.
             The following example performs thus as follows:
              1. "/materials/material[id=\"%\"]"
              2. "/parameter[name=\"%\"]"
              3. "/%/text()"
             evaluates to the following XPath query
              "/materials/material[id=\"Si"]/parameter[name=\"bandgap\"]/value/text()"
   */
  query Si_bandgap_query = make_query(make_entry(material , "Si"), 
                                      make_entry(parameter, "bandgap"),
                                      make_entry(data     , "value"));

  /** @brief Perform the actual query. If the uses knows in advance that the
             above query yields a numerical value, the query_value can be used, 
             performing an automatic conversion to double */
  viennamaterials::numeric value = matlib->query_value(Si_bandgap_query);
  std::cout << "value: " << value << std::endl;

  /** @brief Perform the actual query. If the uses does not know the result type
             of the query upon inspecting the result, the default string-evaluating 
             'query' method should be used. */
  viennamaterials::string value_string = matlib->query(Si_bandgap_query);
  std::cout << "value string: " << value_string << std::endl;
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
