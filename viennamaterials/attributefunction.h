/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef ATTRIBUTE_FUNCTION_H_
#define ATTRIBUTE_FUNCTION_H_

#include <string>
#include <vector>
#include <viennamaterials/platform.hpp>

namespace viennamaterials
{

enum xml_data_types //TODO: move into XML layout header
{
  boolean,
  integer,
  floating_point
};

enum xml_attribute_entity //TODO: move into XML layout header
{
  scalar,
  tensor,
  function
};

enum xml_code_lang
{
  python
};

class FunctionArgumentBase {};

template <typename T>
class FunctionArgument : public FunctionArgumentBase
{
public:
  std::string     quantity;
  xml_data_types  type;
  T               value;
};

class IAttributeFunction
{
public:
  virtual ~IAttributeFunction() {};
  virtual std::vector<FunctionArgumentBase>   init(viennamaterials::library_handle& lib, std::string& xpath_query) = 0;
  virtual void                                deinit() = 0;
  virtual FunctionArgumentBase                evaluate(std::vector<FunctionArgumentBase> args) = 0;

//protected:
  /* TODO xpath wrapper
   *
   * function specific wrappers
   *
   * FunctionArgumentBase query_arguments(xpath to function)
   */

  //FIXME: should be implemented in class pugixml

  long                  query_number_of_attributes(viennamaterials::library_handle& lib, std::string& xpath_query);
  std::string           query_attribute_name(viennamaterials::library_handle& lib, std::string& xpath_query, long position);
  std::string           query_attribute(viennamaterials::library_handle& lib, std::string& xpath_query, long position);
  std::string           query_attribute(viennamaterials::library_handle& lib, std::string& xpath_query, std::string attribute_name);

  //function specific xml wrappers

  long                  query_number_of_arguments(viennamaterials::library_handle& lib, std::string& xpath_query_to_function);
  xml_attribute_entity  query_return_type(viennamaterials::library_handle& lib, std::string& xpath_query_to_function);

};


} /* namespace viennamaterials */


#endif /* ATTRIBUTE_FUNCTION_H_ */
