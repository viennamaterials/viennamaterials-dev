/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef ATTRIBUTEENTITY_HPP_
#define ATTRIBUTEENTITY_HPP_

#include "viennamaterials/xmldatatypes.h"

namespace viennamaterials
{

class attribute_entity
{
public:
  virtual ~attribute_entity() {};

  bool is_type(tag_scalar_bool tag);
  //TODO etc
  bool is_type(xml_attribute_type type);

  bool is_scalar();
  bool is_scalar(tag_scalar_bool tag);
  bool is_scalar(tag_scalar_int tag);
  bool is_scalar(tag_scalar_float tag);
  bool is_scalar_bool();
  bool is_scalar_int();
  bool is_scalar_float();
  bool is_tensor();
  bool is_function();
  bool is_function(tag_function_bool tag);
  bool is_function(tag_function_int tag);
  bool is_function(tag_function_float tag);
  bool is_function(tag_function_tensor tag);
  bool is_function_bool();
  bool is_function_int();
  bool is_function_float();
  bool is_function_tensor();

//  virtual void get_dep() = 0; //FIXME: return
//  virtual void set_dep(void) = 0; //FIXME: arg

  virtual xml_bool  eval(tag_scalar_bool tag)     = 0;
  virtual xml_int   eval(tag_scalar_int tag)      = 0;
  virtual xml_float eval(tag_scalar_float tag)    = 0;
//  virtual void      eval(tag_tensor tag)          = 0; //FIXME: return

protected:
  xml_attribute_type entity_type_;
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEENTITY_HPP_ */
