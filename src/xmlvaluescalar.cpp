/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "../viennamaterials/xmlvaluescalar.hpp"
#include "../viennamaterials/exceptions.hpp"

namespace viennamaterials
{

void viennamaterials::xml_value_scalar_integer::set_value(tag_scalar_bool tag, xml_bool value)
{
  throw func_backend_attr_value_type_error();
}

void viennamaterials::xml_value_scalar_integer::set_value(tag_scalar_int tag, xml_int value)
{
  this->value = value;
}

void viennamaterials::xml_value_scalar_integer::set_value(tag_scalar_float tag, xml_float value)
{
  throw func_backend_attr_value_type_error();
}

xml_bool viennamaterials::xml_value_scalar_integer::get_value(tag_scalar_bool tag)
{
  throw func_backend_attr_value_type_error();
}

xml_int viennamaterials::xml_value_scalar_integer::get_value(tag_scalar_int tag)
{
  return value;
}

xml_float viennamaterials::xml_value_scalar_integer::get_value(tag_scalar_float tag)
{
  throw func_backend_attr_value_type_error();
}

} /* namespace viennamaterials */
