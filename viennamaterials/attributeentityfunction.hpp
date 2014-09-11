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
 * @brief Represents an attribute entity used as argument for a function attribute entity.
 *
 * This class adds index information to an attribute entity object.
 * The index is used to identify the argument position for the function call in the function backend.
 */
class attribute_entity_argument
{
public:
  /**
   * @brief Create an attribute_entity_argument object
   * @param index A integer value representing the index (position) of this attribute object in the argument list of the function in the function backend.
   *              Index 0 identifies the first argument.
   * @param attr_obj A smartpointer to an attribute_entity object which is used as argument for the called function in the function backend.
   */
  attribute_entity_argument(size_t index, attribute_handle& attr_obj);

  /**
   * @brief Get the attribute_entity object of this object
   * @return A smartpointer to the attribute_entity object
   */
  attribute_handle get_attribute();

  /**
   * @brief Get index of this object. The index is the position in the argument list of a function.
   * @return The index of this object
   */
  size_t            get_index();

private:
  size_t index_; /// Argument index for the function backend
  attribute_handle attribute_;
};

/**
 * @brief Represents an attribute XML element holding a function
 *
 * This object holds a function backend, zero or more xml_value_entity objects representing dependencies of the this function attribute and
 * zero or more attribute_entity objects given by <reference> XML element
 */
class attribute_entity_function : public attribute_entity
{
public:
  /**
   * @brief Create a function attribute.
   * @param unit The unit of this attribute object
   * @param entity_type The function type given as enum xml_attribute_type
   * @param backend A smartpointer to the function backend object
   * @param dependencies A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution which
   *                      must be entered by the simulator (frontend)
   * @param arguments A vector of smartpointers to objects of type attribute_entity_argument representing the required arguments for function execution
   *                      which are queried from the XML material database
   */
  attribute_entity_function(std::string const& unit, xml_attribute_type entity_type, function_backend_handle backend, std::vector<xml_value_entity_handle>& dependencies, std::vector<shared_ptr<attribute_entity_argument> > arguments);

  ~attribute_entity_function();

  /**
   * @brief Get the dependencies of the this attribute function object
   * @return A vector of smartpointers to objects of type xml_value_entity representing the required arguments to evaluate this object
   */
  std::vector<xml_value_entity_handle>  get_dependencies();

  /**
   * @brief Set the dependencies of the function attribute
   * @param args A vector of smartpointers to objects of type xml_value_entity representing the required arguments to evaluate this object.
   *              The vector given may hold more than the required arguments. The dependencies actually used are identified by the name information.
   */
  void                                  set_dependencies(std::vector<xml_value_entity_handle>& args);

  /**
   * @brief Evaluate the function of the function backend if this object represents a boolean function attribute
   * @param tag A tag of type tag_scalar_bool
   * @return The bool result of the function in the function backend
   */
  xml_bool                              eval_value(tag_scalar_bool tag);

  /**
   * @brief Evaluate the function of the function backend if this object represents a integer function attribute
   * @param tag A tag of type tag_scalar_int
   * @return The integer result of the function in the function backend
   */
  xml_int                               eval_value(tag_scalar_int tag);

  /**
   * @brief Evaluate the function of the function backend if this object represents a floating point number function attribute
   * @param tag A tag of type tag_scalar_float
   * @return The floating point result of the function in the function backend
   */
  xml_float                             eval_value(tag_scalar_float tag);


private:
  function_backend_handle backend_;
  std::vector<xml_value_entity_handle> dependencies_backend_;
  std::vector<shared_ptr<attribute_entity_argument> > arguments_;

  /**
   * @brief Evaluate all argument attribute objects and return the results as well as the dependencies of this function attribute object
   * @return A vector of smartpointers to objects of type xml_value_entity representing all dependencies for the function call in the function backend
   */
  std::vector<xml_value_entity_handle>  evaluate_arguments();
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITYFUNCTION_HPP_ */
