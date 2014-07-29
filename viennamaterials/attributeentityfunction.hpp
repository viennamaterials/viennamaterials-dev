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

#include "attributeentity.hpp"

namespace viennamaterials
{

class attribute_entity_function: public attribute_entity
{
public:
  attribute_entity_function();
  attribute_entity_function(xml_attribute_type entity_type);

  xml_bool  eval(tag_scalar_bool tag);
  xml_int   eval(tag_scalar_int tag);
  xml_float eval(tag_scalar_float tag);
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITYFUNCTION_HPP_ */
