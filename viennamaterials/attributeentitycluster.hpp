/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef ATTRIBUTEENTITYCLUSTER_HPP_
#define ATTRIBUTEENTITYCLUSTER_HPP_

#include "viennamaterials/attributeentity.hpp"
#include "viennamaterials/attributeentityfunction.hpp"

namespace viennamaterials
{

//TODO doxygen
class attribute_entity_argument
{
public:
  //TODO doxygen
  attribute_entity_argument(size_t index, attribute_handle& attr_obj);

  //TODO doxygen
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

//TODO doxygen
class attribute_entity_cluster : public attribute_entity
{
public:
  /**
   * @brief Create a cluster attribute. This object holds a function backend plus zero or more attribute entity objects given by <reference> XML element
   * @param entity_type The function type given as enum xml_attribute_type
   * @param backend A smartpointer to the function backend object
   * @param dependencies A vector of smartpointers to objects of type xml_value_entity representing the required arguments for function execution which must be entered by the simulator (frontend)
   * @param arguments A vector of smartpointers to objects of type attribute_entity_argument representing the required arguments for function execution which are queried from the XML material database
   */
  attribute_entity_cluster(xml_attribute_type entity_type, function_backend_handle backend, std::vector<xml_value_entity_handle>& dependencies, std::vector<shared_ptr<attribute_entity_argument> > arguments);

  /**
   * @brief Get the dependencies of the this attribute luster object
   * @return A vector of smartpointers to objects of type xml_value_entity representing the required arguments to evaluate this object
   */
  std::vector<xml_value_entity_handle>  get_dependencies();

  /**
   * @brief Set the dependencies of the function attribute
   * @param A vector of smartpointers to objects of type xml_value_entity representing the required arguments to evaluate this object
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
  std::vector<xml_value_entity_handle> dependencies_backend_;
  std::vector<shared_ptr<attribute_entity_argument> > arguments_;

  //TODO doxygen
  std::vector<xml_value_entity_handle>  evaluate_arguments();
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITYCLUSTER_HPP_ */
