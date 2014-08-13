/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef ATTRIBUTEENTITYFUNCTION_HPP_
#define ATTRIBUTEENTITYFUNCTION_HPP_

#include "viennamaterials/attributeentity.hpp"
#include "viennamaterials/functionbackend.hpp"

namespace viennamaterials
{

typedef shared_ptr<function_backend> function_backend_handle;

/**
 * @brief Represents an attribute XML element holding a function
 */
class attribute_entity_function : public attribute_entity
{
public:
  /**
   * @brief Create a function attribute
   * @param entity_type The function type given as enum xml_attribute_type
   * @param backend A smartpointer to the function backend object
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   */
  attribute_entity_function(xml_attribute_type entity_type, function_backend_handle backend, std::vector<xml_value_entity_handle>& args);

  /**
   * @brief Get the dependencies of the function attribute
   * @return A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   */
  std::vector<xml_value_entity_handle>  get_dependencies();

  /**
   * @brief Set the dependencies of the function attribute
   * @param A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution
   */
  void                                  set_dependencies(std::vector<xml_value_entity_handle>& args);

  /**
   * @brief Evaluate the function of the function backend if this object represents a bool function attribute
   * @param tag A tag of type tag_scalar_bool
   * @return The bool result of the function in the function backend
   */
  xml_bool                              eval(tag_scalar_bool tag);

  /**
   * @brief Evaluate the function of the function backend if this object represents a integer function attribute
   * @param tag A tag of type tag_scalar_int
   * @return The integer result of the function in the function backend
   */
  xml_int                               eval(tag_scalar_int tag);

  /**
   * @brief Evaluate the function of the function backend if this object represents a floating point number function attribute
   * @param tag A tag of type tag_scalar_float
   * @return The floating point result of the function in the function backend
   */
  xml_float                             eval(tag_scalar_float tag);

private:
  function_backend_handle backend_;
  std::vector<xml_value_entity_handle> args_;
};

//TODO doxygen?
struct arg_comperator
{
  explicit arg_comperator(const std::string& name) { name_ = name; }
  inline bool operator()(const xml_value_entity_handle& obj) const
  {
    if(obj->get_name().compare(name_) == 0)
      return true;
    return false;
  }
private:
  std::string name_;
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITYFUNCTION_HPP_ */
