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

namespace viennamaterials
{

void* xml_value_scalar_integer::get_value_ptr()
{
  return new xml_int(value); //note: must be destructed by caller
}

void xml_value_scalar_integer::set_value(void* value)
{
  this->value = *(xml_int*)value;
}



void* xml_value_scalar_float::get_value_ptr()
{
  return new xml_float(value); //note: must be destructed by caller
}

void xml_value_scalar_float::set_value(void* value)
{
  this->value = *(xml_float*)value;
}

} /* namespace viennamaterials */
