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

typedef viennamaterials::shared_ptr<viennamaterials::function_backend> function_backend_handle;

class attribute_entity_function: public attribute_entity
{
public:
  attribute_entity_function(xml_attribute_type entity_type, function_backend_handle backend, std::vector<xml_value_entity*> &args);

  std::vector<xml_value_entity*>  get_dependencies();
  void                            set_dependencies(std::vector<xml_value_entity*> &args);

  xml_bool  eval(tag_scalar_bool tag);
  xml_int   eval(tag_scalar_int tag);
  xml_float eval(tag_scalar_float tag);

private:
  function_backend_handle backend_;
  std::vector<xml_value_entity*> args_;
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITYFUNCTION_HPP_ */
