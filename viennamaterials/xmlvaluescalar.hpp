/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef XMLVALUESCALARINTEGER_HPP_
#define XMLVALUESCALARINTEGER_HPP_

#include "xmlvalueentity.hpp"
#include "xmldatatypes.h"

namespace viennamaterials
{

/**
 * class xml_value_scalar_integer represents a XML scalar element of type integer
 */
class xml_value_scalar_integer : public xml_value_entity
{
public:
  void      set_value(xml_bool value);
  void      set_value(xml_int value);
  void      set_value(xml_float value);
  xml_bool  get_value(tag_scalar_bool tag);
  xml_int   get_value(tag_scalar_int tag);
  xml_float get_value(tag_scalar_float tag);

private:
  xml_int value;
};

/**
 * class xml_value_scalar_float represents a XML scalar element of type float
 */
class xml_value_scalar_float : public xml_value_entity
{
public:
  //TODO

private:
  xml_float value;
};

} /* namespace viennamaterials */
#endif /* XMLVALUESCALARINTEGER_HPP_ */
